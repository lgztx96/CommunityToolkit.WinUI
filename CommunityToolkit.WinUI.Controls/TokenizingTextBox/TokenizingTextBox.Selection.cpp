#include "pch.h"
#include "TokenizingTextBox.h"
#include "TokenizingTextBoxItem.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    bool TokenizingTextBox::MoveFocusAndSelection(MoveDirection direction)
    {
        bool retVal = false;
        auto currentContainerItem = GetCurrentContainerItem();

        if (currentContainerItem != nullptr)
        {
            auto currentItem = ItemFromContainer(currentContainerItem);
            uint32_t previousIndex;
            auto items = Items();
            items.IndexOf(currentItem, previousIndex);
            auto index = previousIndex;

            if (direction == MoveDirection::Previous)
            {
                if (previousIndex > 0)
                {
                    index -= 1;
                }
                else
                {
                    if (TabNavigateBackOnArrow())
                    {
                        FindNextElementOptions options;
                        options.SearchRoot(XamlRoot().Content());
                        FocusManager::TryMoveFocus(FocusNavigationDirection::Previous, options);
                    }


                    retVal = true;
                }
            }
            else if (direction == MoveDirection::Next)
            {
                if (previousIndex < items.Size() - 1)
                {
                    index += 1;
                }
            }

            // Only do stuff if the index is actually changing
            if (index != previousIndex)
            {
                if (auto newItem = ContainerFromIndex(index).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                {
                    // Check for the new item being a text control.
                    // this must happen before focus is set to avoid seeing the caret
                    // jump in come cases
                    if (items.GetAt(index).try_as<ITokenStringContainer>() && !IsShiftPressed())
                    {
                        auto self = winrt::get_self<TokenizingTextBoxItem>(newItem)->get_strong();
                        self->_autoSuggestTextBox.SelectionLength(0);
                        self->_autoSuggestTextBox.SelectionStart(direction == MoveDirection::Next
                            ? 0
                            : self->_autoSuggestTextBox.Text().size());
                    }

                    newItem.Focus(FocusState::Keyboard);

                    // if no control keys are selected then the selection also becomes just this item
                    if (IsShiftPressed())
                    {
                        // What we do here depends on where the selection started
                        // if the previous item is between the start and new position then we add the new item to the selected range
                        // if the new item is between the start and the previous position then we remove the previous position
                        int newDistance = std::abs(SelectedIndex() - static_cast<int>(index));
                        int oldDistance = std::abs(SelectedIndex() - static_cast<int>(previousIndex));

                        if (newDistance > oldDistance)
                        {
                            SelectedItems().Append(items.GetAt(index));
                        }
                        else
                        {
                            uint32_t selectIndex;
                            if (SelectedItems().IndexOf(items.GetAt(previousIndex), selectIndex))
                            {
                                SelectedItems().RemoveAt(selectIndex);
                            }
                            
                        }
                    }
                    else if (!IsControlPressed())
                    {
                        SelectedIndex(index);

                        // This looks like a bug in the underlying ListViewBase control.
                        // Might need to be reviewed if the base behavior is fixed
                        // When two consecutive items are selected and the navigation moves between them,
                        // the first time that happens the old focused item is not unselected
                        if (SelectedItems().Size() > 1)
                        {
                            SelectedItems().Clear();
                            SelectedIndex(index);
                        }
                    }

                    retVal = true;
                }
            }
        }

        return retVal;
    }

    winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem TokenizingTextBox::GetCurrentContainerItem()
    {
        if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
        {
            return FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>();
        }
        else
        {
            return FocusManager::GetFocusedElement().try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>();
        }
    }

    void TokenizingTextBox::SelectAllTokensAndText() const
    {
        std::ignore = _dispatcherQueue.TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [=]()
        {
            ListViewExtensions::SelectAllSafe(*this);

            // need to synchronize the select all and the focus behavior on the text box
            // because there is no way to identify that the focus has been set from this point
            // to avoid instantly clearing the selection of tokens
            PauseTokenClearOnFocus = true;

            for (const auto& item : Items())
            {
                if (item.try_as<ITokenStringContainer>())
                {
                    // grab any selected text
                    if (auto pretoken = ContainerFromItem(item).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                    {
                        auto self = winrt::get_self<TokenizingTextBoxItem>(pretoken)->get_strong();
                        self->_autoSuggestTextBox.SelectionStart(0);
                        self->_autoSuggestTextBox.SelectionLength(self->_autoSuggestTextBox.Text().size());
                    }
                }
            }

            if (auto container = ContainerFromIndex(Items().Size() - 1).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
            {
                container.Focus(FocusState::Programmatic);
            }
        });
    }

    void TokenizingTextBox::DeselectAllTokensAndText(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem)
    {
        ListViewExtensions::DeselectAll(*this);
        ClearAllTextSelections(ignoreItem);
    }

    void TokenizingTextBox::ClearAllTextSelections(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem)
    {
        // Clear any selection in the text box
        for (auto item : Items())
        {
            if (item.try_as<ITokenStringContainer>())
            {
                if (auto container = ContainerFromItem(item).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                {
                    if (container != ignoreItem)
                    {
                        auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
                        self->_autoSuggestTextBox.SelectionLength(0);
                    }
                }
            }
        }
    }

    bool TokenizingTextBox::SelectPreviousItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item)
    {
        return SelectNewItem(item, -1, [](int i) { return i > 0; });
    }

    bool TokenizingTextBox::SelectNextItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item)
    {
        return SelectNewItem(item, 1, [&](int i) { return i < static_cast<int>(Items().Size()) - 1; });
    }

    bool TokenizingTextBox::SelectNewItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, int increment, std::function<bool(int)> testFunc)
    {
        bool returnVal = false;

        // find the item in the list
        auto currentIndex = IndexFromContainer(item);

        // Select previous token item (if there is one).
        if (testFunc(currentIndex))
        {
            if (auto newItem = ContainerFromItem(Items().GetAt(currentIndex + increment)).try_as<ListViewItem>())
            {
                newItem.Focus(FocusState::Keyboard);
                SelectedItems().Append(Items().GetAt(currentIndex + increment));
                returnVal = true;
            }
        }

        return returnVal;
    }

    IAsyncAction TokenizingTextBox::TokenizingTextBoxItem_ClearAllAction(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, RoutedEventArgs const& args)
    {
        // find the first item selected
        int newSelectedIndex = -1;

        if (SelectedRanges().Size() > 0)
        {
            newSelectedIndex = SelectedRanges().GetAt(0).FirstIndex() - 1;
        }

        co_await RemoveAllSelectedTokens();

        SelectedIndex(newSelectedIndex);

        if (newSelectedIndex == -1)
        {
            newSelectedIndex = Items().Size() - 1;
        }

        // focus the item prior to the first selected item
        if (auto container = ContainerFromIndex(newSelectedIndex).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
        {
            container.Focus(FocusState::Keyboard);
        }
    }

    IAsyncAction TokenizingTextBox::TokenizingTextBoxItem_ClearClicked(Controls::TokenizingTextBoxItem const& sender, RoutedEventArgs const& args)
    {
         co_await RemoveTokenAsync(sender, nullptr);
    }

    IAsyncAction TokenizingTextBox::RemoveAllSelectedTokens()
    {
        auto currentContainerItem = GetCurrentContainerItem();

        while (SelectedItems().Size() > 0)
        {
            auto selectedItems = SelectedItems();
            if (auto container = ContainerFromItem(selectedItems.GetAt(0)).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
            {
                auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
                if (IndexFromContainer(container) != static_cast<int>(Items().Size()) - 1)
                {
                   
                    // if its a text box, remove any selected text, and if its then empty remove the container, unless its focused
                    if (selectedItems.GetAt(0).try_as<ITokenStringContainer>())
                    {
                        const auto& asb = self->_autoSuggestTextBox;

                        // grab any selected text
                        std::wstring tempStr = asb.SelectionStart() == 0
                            ? std::wstring{}
                        : std::wstring{ std::wstring_view(asb.Text()).substr(
                            0,
                            asb.SelectionStart()) };
                        tempStr +=
                            asb.SelectionStart() +
                            asb.SelectionLength() < static_cast<int>(asb.Text().size())
                            ? std::wstring_view(asb.Text()).substr(
                                asb.SelectionStart() +
                                asb.SelectionLength())
                            : L"";

                        if (tempStr.size() == 0)
                        {
                            // Need to be careful not to remove the last item in the list
                            co_await RemoveTokenAsync(container, nullptr);
                        }
                        else
                        {
                            asb.Text(tempStr);
                        }
                    }
                    else
                    {
                        // if the item is a token just remove it.
                        co_await RemoveTokenAsync(container, nullptr);
                    }
                }
                else
                {
                    if (selectedItems.Size() == 1)
                    {
                        // at this point we have one selection and its the default textbox.
                        // stop the iteration here
                        break;
                    }
                }
            }
        }
    }

    void TokenizingTextBox::CopySelectedToClipboard()
    {
        Windows::ApplicationModel::DataTransfer::DataPackage dataPackage;
        dataPackage.RequestedOperation(Windows::ApplicationModel::DataTransfer::DataPackageOperation::Copy);

        auto tokenString = PrepareSelectionForClipboard();

        if (!tokenString.empty())
        {
            dataPackage.SetText(tokenString);
            Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);
        }
    }

    winrt::hstring TokenizingTextBox::PrepareSelectionForClipboard() const
    {
        std::wstring tokenString;
        bool addSeparator = false;

        // Copy all items if none selected (and no text selected)
        for(const auto& item : SelectedItems().Size() > 0 ? SelectedItems() : Items())
        {
            if (addSeparator)
            {
                tokenString.append(TokenDelimiter());
            }
            else
            {
                addSeparator = true;
            }

            if (item.try_as<ITokenStringContainer>())
            {
                // grab any selected text
                if (auto pretoken = ContainerFromItem(item).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                {
                    auto self = winrt::get_self<TokenizingTextBoxItem>(pretoken)->get_strong();
                    if (self->_autoSuggestTextBox != nullptr) {
                        winrt::hstring text = self->_autoSuggestTextBox.Text();
                        tokenString += std::wstring_view(text).substr(
                            self->_autoSuggestTextBox.SelectionStart(),
                            self->_autoSuggestTextBox.SelectionLength());
                    }
                }
            }
            else
            {
                if (auto stringable = item.try_as<IStringable>())
                {
                    tokenString += stringable.ToString();
                }
            }
        }

        return { tokenString.data(), static_cast<uint32_t>(tokenString.size()) };
    }
}