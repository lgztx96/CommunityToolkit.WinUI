#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cstdlib>
#include <string>
#include <tuple>
#endif
#include "TokenizingTextBox.h"
#include "TokenizingTextBoxItem.h"
#include "../SettingsControls/Helpers/ControlHelper.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    bool TokenizingTextBox::MoveFocusAndSelection(MoveDirection direction)
    {
        bool retVal = false;
        
        if (auto currentContainerItem = GetCurrentContainerItem())
        {
            auto currentItem = ItemFromContainer(currentContainerItem);
            uint32_t previousIndex;
            auto items = Items();
            items.IndexOf(currentItem, previousIndex);
            uint32_t index = previousIndex;

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
                        winrt::FindNextElementOptions options;
                        options.SearchRoot(XamlRoot().Content());
                        winrt::FocusManager::TryMoveFocus(FocusNavigationDirection::Previous, options);
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
                if (auto newItem = ContainerFromIndex(index).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
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

                    newItem.Focus(winrt::FocusState::Keyboard);

                    // if no control keys are selected then the selection also becomes just this item
                    if (IsShiftPressed())
                    {
						const auto selectedItems = SelectedItems();
						const auto selectedIndex = SelectedIndex();
                        // What we do here depends on where the selection started
                        // if the previous item is between the start and new position then we add the new item to the selected range
                        // if the new item is between the start and the previous position then we remove the previous position
                        int newDistance = std::abs(selectedIndex - static_cast<int>(index));
                        int oldDistance = std::abs(selectedIndex - static_cast<int>(previousIndex));

                        if (newDistance > oldDistance)
                        {
                            selectedItems.Append(items.GetAt(index));
                        }
                        else
                        {
                            uint32_t selectIndex;
                            if (selectedItems.IndexOf(items.GetAt(previousIndex), selectIndex))
                            {
                                selectedItems.RemoveAt(selectIndex);
                            }
                            
                        }
                    }
                    else if (!IsControlPressed())
                    {
                        SelectedIndex(index);

                        auto selectedItems = SelectedItems();
                        // This looks like a bug in the underlying ListViewBase control.
                        // Might need to be reviewed if the base behavior is fixed
                        // When two consecutive items are selected and the navigation moves between them,
                        // the first time that happens the old focused item is not unselected
                        if (selectedItems.Size() > 1)
                        {
                            selectedItems.Clear();
                            SelectedIndex(index);
                        }
                    }

                    retVal = true;
                }
            }
        }

        return retVal;
    }

    winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem TokenizingTextBox::GetCurrentContainerItem()
    {
        if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
        {
            return winrt::FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>();
        }
        else
        {
            return winrt::FocusManager::GetFocusedElement().try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>();
        }
    }

    void TokenizingTextBox::SelectAllTokensAndText()
    {
        std::ignore = _dispatcherQueue.TryEnqueue(winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [=]()
        {
            ListViewExtensions::SelectAllSafe(*this);

            // need to synchronize the select all and the focus behavior on the text box
            // because there is no way to identify that the focus has been set from this point
            // to avoid instantly clearing the selection of tokens
            this->PauseTokenClearOnFocus = true;

            for (const auto& item : Items())
            {
                if (item.try_as<ITokenStringContainer>())
                {
                    // grab any selected text
                    if (auto pretoken = ContainerFromItem(item).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                    {
                        auto self = winrt::get_self<TokenizingTextBoxItem>(pretoken)->get_strong();
                        self->_autoSuggestTextBox.SelectionStart(0);
                        self->_autoSuggestTextBox.SelectionLength(self->_autoSuggestTextBox.Text().size());
                    }
                }
            }

            if (auto container = ContainerFromIndex(Items().Size() - 1).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
            {
                container.Focus(winrt::FocusState::Programmatic);
            }
        });
    }

    void TokenizingTextBox::DeselectAllTokensAndText(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem)
    {
        ListViewExtensions::DeselectAll(*this);
        ClearAllTextSelections(ignoreItem);
    }

    void TokenizingTextBox::ClearAllTextSelections(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem)
    {
        // Clear any selection in the text box
        for (const auto& item : Items())
        {
            if (item.try_as<ITokenStringContainer>())
            {
                if (auto container = ContainerFromItem(item).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
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

    bool TokenizingTextBox::SelectPreviousItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item)
    {
        return SelectNewItem(item, -1, [](int i) { return i > 0; });
    }

    bool TokenizingTextBox::SelectNextItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item)
    {
        return SelectNewItem(item, 1, [&](int i) { return i < static_cast<int>(Items().Size()) - 1; });
    }

    bool TokenizingTextBox::SelectNewItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, int increment, std::function<bool(int)> testFunc)
    {
        bool returnVal = false;

        // find the item in the list
        const auto currentIndex = IndexFromContainer(item);

        // Select previous token item (if there is one).
        if (testFunc(currentIndex))
        {
			auto items = Items();
            if (auto newItem = ContainerFromItem(items.GetAt(currentIndex + increment)).try_as<winrt::ListViewItem>())
            {
                newItem.Focus(winrt::FocusState::Keyboard);
                SelectedItems().Append(items.GetAt(currentIndex + increment));
                returnVal = true;
            }
        }

        return returnVal;
    }

    winrt::IAsyncAction TokenizingTextBox::TokenizingTextBoxItem_ClearAllAction(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, winrt::RoutedEventArgs const& args)
    {
        // find the first item selected
        int newSelectedIndex = -1;
        if (const auto selectedRanges = SelectedRanges(); selectedRanges.Size() > 0)
        {
            newSelectedIndex = selectedRanges.GetAt(0).FirstIndex() - 1;
        }

        co_await RemoveAllSelectedTokens();

        SelectedIndex(newSelectedIndex);

        if (newSelectedIndex == -1)
        {
            newSelectedIndex = Items().Size() - 1;
        }

        // focus the item prior to the first selected item
        if (auto container = ContainerFromIndex(newSelectedIndex).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
        {
            container.Focus(winrt::FocusState::Keyboard);
        }
    }

    winrt::IAsyncAction TokenizingTextBox::TokenizingTextBoxItem_ClearClicked(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, winrt::RoutedEventArgs const& args)
    {
         co_await RemoveTokenAsync(sender, nullptr);
    }

    winrt::IAsyncAction TokenizingTextBox::RemoveAllSelectedTokens()
    {
        while (true)
        {
            const auto selectedItems = SelectedItems();
            if (selectedItems.Size() == 0)
            {
                break;
            }

            if (auto container = ContainerFromItem(selectedItems.GetAt(0)).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
            {
                auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
                if (IndexFromContainer(container) != static_cast<int>(Items().Size()) - 1)
                {
                    // if its a text box, remove any selected text, and if its then empty remove the container, unless its focused
                    if (selectedItems.GetAt(0).try_as<ITokenStringContainer>())
                    {
                        const auto& asb = self->_autoSuggestTextBox;

                        // grab any selected text
						const auto selectionStart = asb.SelectionStart();
						const auto selectionLength = asb.SelectionLength();
                        const std::wstring_view sv = asb.Text();
                        std::wstring tempStr{ sv.substr(0, selectionStart) };
 
                        if ((selectionStart + selectionLength) < static_cast<int>(sv.size())) 
                        {
							tempStr += sv.substr(selectionStart + selectionLength);
                        }
    
                        if (tempStr.empty())
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
        winrt::DataPackage dataPackage;
        dataPackage.RequestedOperation(winrt::DataPackageOperation::Copy);

        auto tokenString = PrepareSelectionForClipboard();

        if (!tokenString.empty())
        {
            dataPackage.SetText(tokenString);
            winrt::Clipboard::SetContent(dataPackage);
        }
    }

    winrt::hstring TokenizingTextBox::PrepareSelectionForClipboard() const
    {
        std::wstring tokenString;
        bool addSeparator = false;

        // Copy all items if none selected (and no text selected)
		auto selectedItems = SelectedItems();
        for(const auto& item : selectedItems.Size() > 0 ? selectedItems : Items())
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
                if (auto pretoken = ContainerFromItem(item).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
                {
                    auto self = winrt::get_self<TokenizingTextBoxItem>(pretoken)->get_strong();
                    if (self->_autoSuggestTextBox) 
                    {
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
