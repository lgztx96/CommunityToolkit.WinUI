#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#endif
#include "TokenizingTextBox.h"
#if __has_include("TokenizingTextBox.g.cpp")
#include "TokenizingTextBox.g.cpp"
#endif
#include "TokenizingTextBoxItem.h"
#include "PretokenStringContainer.h"
#include "TokenizingTextBoxAutomationPeer.h"
#include "TokenItemAddingEventArgs.h"
#include "TokenItemRemovingEventArgs.h"
#include "../XamlToolkit.WinUI/common.h"
#include "StringExtensions.h"
#include "../SettingsControls/Helpers/ControlHelper.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	bool TokenizingTextBox::IsShiftPressed()
	{
		const auto shiftState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Shift);
		return (static_cast<uint32_t>(shiftState) 
			& static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down))
			== static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down);
	}

	bool TokenizingTextBox::IsControlPressed()
	{
		const auto ctrlState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Control);
		return (static_cast<uint32_t>(ctrlState) 
			& static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down))
			== static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down);
	}

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::AutoSuggestBoxStyleProperty =
		winrt::DependencyProperty::Register(
			L"AutoSuggestBoxStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::AutoSuggestBoxTextBoxStyleProperty =
		winrt::DependencyProperty::Register(
			L"AutoSuggestBoxTextBoxStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TextMemberPathProperty =
		winrt::DependencyProperty::Register(
			L"TextMemberPath",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TokenItemTemplateProperty =
		winrt::DependencyProperty::Register(
			L"TokenItemTemplate",
			winrt::xaml_typename<winrt::DataTemplate>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TokenItemTemplateSelectorProperty =
		winrt::DependencyProperty::Register(
			L"TokenItemTemplateSelector",
			winrt::xaml_typename<winrt::DataTemplateSelector>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TokenDelimiterProperty =
		winrt::DependencyProperty::Register(
			L"TokenDelimiter",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(L" ") });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TokenSpacingProperty =
		winrt::DependencyProperty::Register(
			L"TokenSpacing",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::PlaceholderTextProperty =
		winrt::DependencyProperty::Register(
			L"PlaceholderText",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(L"") });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::QueryIconProperty =
		winrt::DependencyProperty::Register(
			L"QueryIcon",
			winrt::xaml_typename<winrt::IconSource>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TextProperty =
		winrt::DependencyProperty::Register(
			L"Text",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(L""), &TokenizingTextBox::OnTextPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::SuggestedItemsSourceProperty =
		winrt::DependencyProperty::Register(
			L"SuggestedItemsSource",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::SuggestedItemTemplateProperty =
		winrt::DependencyProperty::Register(
			L"SuggestedItemTemplate",
			winrt::xaml_typename<winrt::DataTemplate>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::SuggestedItemTemplateSelectorProperty =
		winrt::DependencyProperty::Register(
			L"SuggestedItemTemplateSelector",
			winrt::xaml_typename<winrt::DataTemplateSelector>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::SuggestedItemContainerStyleProperty =
		winrt::DependencyProperty::Register(
			L"SuggestedItemContainerStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::TabNavigateBackOnArrowProperty =
		winrt::DependencyProperty::Register(
			L"TabNavigateBackOnArrow",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(false) });

	const wil::single_threaded_property<winrt::DependencyProperty> TokenizingTextBox::MaximumTokensProperty =
		winrt::DependencyProperty::Register(
			L"MaximumTokens",
			winrt::xaml_typename<int32_t>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr, &TokenizingTextBox::OnMaximumTokensChanged });

	TokenizingTextBox::TokenizingTextBox()
	{
		// Setup our base state of our collection
		_innerItemsSource = winrt::make<InterspersedObservableVector>(winrt::single_threaded_observable_vector<winrt::IInspectable>()); // TODO: Test this still will let us bind to ItemsSource in XAML?
		_currentTextEdit = _lastTextEdit = winrt::make<PretokenStringContainer>(true);
		_innerItemsSource.Insert(_innerItemsSource.Size(), _currentTextEdit);
		ItemsSource(_innerItemsSource);
		//// TODO: Consolidate with callback below for ItemsSourceProperty changed?

		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// TODO: Do we want to support ItemsSource better? Need to investigate how that works with adding...
		RegisterPropertyChangedCallback(ItemsControl::ItemsSourceProperty(), { this, &TokenizingTextBox::ItemsSource_PropertyChanged });
		PreviewKeyDown({ this, &TokenizingTextBox::TokenizingTextBox_PreviewKeyDown });
		PreviewKeyUp({ this, &TokenizingTextBox::TokenizingTextBox_PreviewKeyUp });
		CharacterReceived({ this, &TokenizingTextBox::TokenizingTextBox_CharacterReceived });
		ItemClick({ this, &TokenizingTextBox::TokenizingTextBox_ItemClick });

		_dispatcherQueue = DispatcherQueue();
	}

	void TokenizingTextBox::ItemsSource_PropertyChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		// If we're given a different ItemsSource, we need to wrap that collection in our helper class.
		if (auto itemsSource = ItemsSource(); itemsSource && !itemsSource.try_as<InterspersedObservableVector>())
		{
			_innerItemsSource = winrt::make<InterspersedObservableVector>(itemsSource);
			int32_t maximumTokens = MaximumTokens();
			int32_t itemSize = static_cast<int>(_innerItemsSource.ItemsSource().Size());
			if (ReadLocalValue(MaximumTokensProperty()) != winrt::DependencyProperty::UnsetValue() && itemSize >= maximumTokens)
			{
				// Reduce down to below the max as necessary.
				int32_t endCount = maximumTokens > 0 ? maximumTokens : 0;
				for (auto i = itemSize - 1; i >= endCount; --i)
				{
					_innerItemsSource.RemoveAt(i);
				}
			}

			// Add our text box at the end of items and set its default value to our initial text, fix for #4749
			auto container = winrt::make<PretokenStringContainer>(true);
			container.Text(Text());
			_currentTextEdit = _lastTextEdit = container;
			_innerItemsSource.Insert(_innerItemsSource.Size(), _currentTextEdit);
			ItemsSource(_innerItemsSource);
		}
	}

	void TokenizingTextBox::TokenizingTextBox_ItemClick([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ItemClickEventArgs const& e)
	{
		// If the user taps an item in the list, make sure to clear any text selection as required
		// Note, token selection is cleared by the listview default behavior
		if (!IsControlPressed())
		{
			// Set class state flag to prevent click item being immediately deselected
			IsClearingForClick = true;
			ClearAllTextSelections(nullptr);
		}
	}

	void TokenizingTextBox::TokenizingTextBox_PreviewKeyUp([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		OnTokenizingTextBoxPreviewKeyUp(e.Key());
	}

	void TokenizingTextBox::OnTokenizingTextBoxPreviewKeyUp(winrt::VirtualKey key)
	{
		switch (key)
		{
		case winrt::VirtualKey::Escape:
		{
			// Clear any selection and place the focus back into the text box
			DeselectAllTokensAndText();
			FocusPrimaryAutoSuggestBox();
			break;
		}
		}
	}

	/// <summary>
	/// Set the focus to the last item in the collection
	/// </summary>
	void TokenizingTextBox::FocusPrimaryAutoSuggestBox()
	{
		if (auto items = Items(); items && items.Size() > 0)
		{
			if (auto container = ContainerFromIndex(items.Size() - 1)
				.try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
			{
				container.Focus(winrt::FocusState::Programmatic);
			}
		}
	}

	winrt::IAsyncAction TokenizingTextBox::TokenizingTextBox_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs e)
	{
		e.Handled(co_await OnTokenizingTextBoxPreviewKeyDown(e.Key()));
	}

	winrt::IAsyncOperation<bool> TokenizingTextBox::OnTokenizingTextBoxPreviewKeyDown(winrt::VirtualKey key)
	{
		// Global handlers on control regardless if focused on item or in textbox.
		switch (key)
		{
		case winrt::VirtualKey::C:
			if (IsControlPressed())
			{
				CopySelectedToClipboard();
				co_return true;
			}

			break;

		case winrt::VirtualKey::X:
			if (IsControlPressed())
			{
				CopySelectedToClipboard();

				// now clear all selected tokens and text, or all if none are selected
				co_await RemoveAllSelectedTokens();
			}

			break;

			// For moving between tokens
		case winrt::VirtualKey::Left:
			co_return MoveFocusAndSelection(MoveDirection::Previous);

		case winrt::VirtualKey::Right:
			co_return MoveFocusAndSelection(MoveDirection::Next);

		case winrt::VirtualKey::A:
			// modify the select-all behavior to ensure the text in the edit box gets selected.
			if (IsControlPressed())
			{
				SelectAllTokensAndText();
				co_return true;
			}

			break;
		}

		co_return false;
	}

	void TokenizingTextBox::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		winrt::MenuFlyoutItem selectAllMenuItem;
		selectAllMenuItem.Text(L"Select all");

		selectAllMenuItem.Click({ get_weak(), [this](auto&, auto&) { SelectAllTokensAndText(); } });

		winrt::MenuFlyout menuFlyout;
		menuFlyout.Items().Append(selectAllMenuItem);

		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			menuFlyout.XamlRoot(XamlRoot());
		}

		ContextFlyout(menuFlyout);
	}

	void TokenizingTextBox::RaiseQuerySubmitted(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxQuerySubmittedEventArgs const& args)
	{
		QuerySubmitted.invoke(sender, args);
	}

	void TokenizingTextBox::RaiseSuggestionChosen(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxSuggestionChosenEventArgs const& args)
	{
		SuggestionChosen.invoke(sender, args);
	}

	void TokenizingTextBox::RaiseTextChanged(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxTextChangedEventArgs const& args)
	{
		TextChanged.invoke(sender, args);
	}

	winrt::IAsyncAction TokenizingTextBox::TokenizingTextBox_CharacterReceived(winrt::UIElement const& sender, winrt::CharacterReceivedRoutedEventArgs const& args)
	{
		auto container = ContainerFromItem(_currentTextEdit).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>();
		if (container == nullptr) co_return;
		auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
		auto character = args.Character();
		if (!(GetFocusedElement() == self->_autoSuggestTextBox || ::iswcntrl(character)))
		{
			auto selectedItems = SelectedItems();
			if (selectedItems.Size() > 0)
			{
				uint32_t index;
				_innerItemsSource.IndexOf(selectedItems.First(), index);
				co_await RemoveAllSelectedTokens();

				std::ignore = this->DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [=]()
				{
					// If we're before the last textbox and it's empty, redirect focus to that one instead
					if (index == _innerItemsSource.Size() - 1 && IsNullOrWhiteSpace(_lastTextEdit.Text()))
					{
						if (auto lastContainer = ContainerFromItem(_lastTextEdit).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
						{
							auto lastSelf = winrt::get_self<TokenizingTextBoxItem>(lastContainer)->get_strong();
							lastSelf->UseCharacterAsUser(true); // Make sure we trigger a refresh of suggested items.

							_lastTextEdit.Text(winrt::hstring{ reinterpret_cast<const wchar_t*>(&character), 1 });

							UpdateCurrentTextEdit(_lastTextEdit);

							lastSelf->_autoSuggestTextBox.SelectionStart(1); // Set position to after our new character inserted

							lastSelf->_autoSuggestTextBox.Focus(winrt::FocusState::Keyboard);
						}
					}
					else
					{
						//// Otherwise, create a new textbox for this text.
						auto container = winrt::make<PretokenStringContainer>(false);
						container.Text((::iswspace(character) ? L"" : winrt::hstring{ character }));
						UpdateCurrentTextEdit(container); // Trim so that 'space' isn't inserted and can be used to insert a new box.

						_innerItemsSource.InsertAt(index, _currentTextEdit);

						std::ignore = DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [=]()
						{
							if (auto newContainer = ContainerFromIndex(index).try_as<Controls::TokenizingTextBoxItem>()) // Should be our last text box
							{
								auto newSelf = winrt::get_self<TokenizingTextBoxItem>(newContainer)->get_strong();
								newSelf->UseCharacterAsUser(true); // Make sure we trigger a refresh of suggested items.
								auto waitForLoadToken = std::make_shared<winrt::event_token>();
								auto WaitForLoad = [newContainerWeak(winrt::make_weak(newContainer)), waitForLoadToken]([[maybe_unused]] IInspectable const& s, [[maybe_unused]] RoutedEventArgs const& eargs)
								{
									if (auto container = newContainerWeak.get())
									{
										auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
										if (self->_autoSuggestTextBox)
										{
											self->_autoSuggestTextBox.SelectionStart(1); // Set position to after our new character inserted

											self->_autoSuggestTextBox.Focus(FocusState::Keyboard);
										}

										self->AutoSuggestTextBoxLoaded(*waitForLoadToken);
									}
								};

								*waitForLoadToken = newSelf->AutoSuggestTextBoxLoaded(WaitForLoad);
							}
						});
					}
				});
			}
			else
			{
				// If no items are selected, send input to the last active string container.
				// This code is only fires during an edgecase where an item is in the process of being deleted and the user inputs a character before the focus has been redirected to a string container.
				if (auto textToken = _innerItemsSource.GetAt(_innerItemsSource.Size() - 1).try_as<ITokenStringContainer>())
				{
					if (auto last = ContainerFromIndex(Items().Size() - 1).try_as<Controls::TokenizingTextBoxItem>()) // Should be our last text box
					{
						auto lastSelf = winrt::get_self<TokenizingTextBoxItem>(last)->get_strong();
						std::wstring text{ lastSelf->_autoSuggestTextBox.Text() };
						auto selectionStart = lastSelf->_autoSuggestTextBox.SelectionStart();
						auto position = selectionStart > static_cast<int>(text.size()) ? text.size() : selectionStart;
						textToken.Text(
							text.substr(0, position) 
							+ static_cast<wchar_t>(character)
							+ text.substr(position));

						lastSelf->_autoSuggestTextBox.SelectionStart(static_cast<int>(position) + 1); // Set position to after our new character inserted

						lastSelf->_autoSuggestTextBox.Focus(winrt::FocusState::Keyboard);
					}
				}
			}
		}
	}

	winrt::IInspectable TokenizingTextBox::GetFocusedElement()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			return winrt::FocusManager::GetFocusedElement(XamlRoot());
		}
		else
		{
			return winrt::FocusManager::GetFocusedElement();
		}
	}

	winrt::DependencyObject TokenizingTextBox::GetContainerForItemOverride() const { return winrt::make<TokenizingTextBoxItem>(); }

	bool TokenizingTextBox::IsItemItsOwnContainerOverride(winrt::IInspectable const& item)
	{
		return static_cast<bool>(item.try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>());
	}

	void TokenizingTextBox::PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);

		if (auto tokenItem = element.try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			auto itemSelf = winrt::get_self<TokenizingTextBoxItem>(tokenItem)->get_strong();
			itemSelf->Owner(*this);

			itemSelf->ContentTemplateSelector(TokenItemTemplateSelector());
			itemSelf->ContentTemplate(TokenItemTemplate());

			itemSelf->ClearClicked(itemSelf->_clearClickedToken);
			itemSelf->ClearClicked({ this, &TokenizingTextBox::TokenizingTextBoxItem_ClearClicked });

			itemSelf->ClearAllAction(itemSelf->_clearAllActionToken);
			itemSelf->ClearAllAction({ this, &TokenizingTextBox::TokenizingTextBoxItem_ClearAllAction });

			itemSelf->GotFocus(itemSelf->_gotFocusToken);
			itemSelf->GotFocus({ this, &TokenizingTextBox::TokenizingTextBoxItem_GotFocus });

			itemSelf->LostFocus(itemSelf->_lostFocusToken);
			itemSelf->LostFocus({ this, &TokenizingTextBox::TokenizingTextBoxItem_LostFocus });

			winrt::MenuFlyout menuFlyout;
			winrt::MenuFlyoutItem removeMenuItem;
			removeMenuItem.Text(L"Remove");
			removeMenuItem.Click({ get_weak(), [this, tokenItem](auto&, auto&) { TokenizingTextBoxItem_ClearClicked(tokenItem, nullptr); } });
			menuFlyout.Items().Append(removeMenuItem);

			if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
			{
				menuFlyout.XamlRoot(XamlRoot());
			}

			winrt::MenuFlyoutItem selectAllMenuItem;
			selectAllMenuItem.Text(L"Select all");
			selectAllMenuItem.Click({ get_weak(), [this](auto&, auto&) { SelectAllTokensAndText(); } });
			menuFlyout.Items().Append(selectAllMenuItem);

			itemSelf->ContextFlyout(menuFlyout);
		}
	}

	void TokenizingTextBox::TokenizingTextBoxItem_GotFocus(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// Keep track of our currently focused textbox
		if (auto ttbi = sender.try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			if (auto text = ttbi.Content().try_as<ITokenStringContainer>())
			{
				UpdateCurrentTextEdit(text);
			}
		}
	}

	void TokenizingTextBox::TokenizingTextBoxItem_LostFocus(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// Keep track of our currently focused textbox
		if (auto ttbi = sender.try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			if (auto text = ttbi.Content().try_as<ITokenStringContainer>(); text &&
				IsNullOrWhiteSpace(text.Text())
				&& text != _lastTextEdit) 
			{
				// We're leaving an inner textbox that's blank, so we'll remove it
				if (uint32_t index; _innerItemsSource.IndexOf(text, index))
				{
					_innerItemsSource.RemoveAt(index);
				}

				UpdateCurrentTextEdit(_lastTextEdit);

				GuardAgainstPlaceholderTextLayoutIssue();
			}
		}
	}

	void TokenizingTextBox::AddTokenItem(winrt::IInspectable& data, bool atEnd)
	{
		std::ignore = AddTokenAsync(data, atEnd);
	}

	winrt::IAsyncAction TokenizingTextBox::ClearAsync()
	{
		while (_innerItemsSource.Size() > 1)
		{
			auto item = _innerItemsSource.GetAt(0);
			if (auto container = ContainerFromItem(item).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
			{
				if (!co_await RemoveTokenAsync(container, item))
				{
					// if a removal operation fails then stop the clear process
					break;
				}
			}
		}

		// Clear the active pretoken string.
		// Setting the text property directly avoids a delay when setting the text in the autosuggest box.
		Text(L"");
	}

	winrt::IAsyncAction TokenizingTextBox::AddTokenAsync(winrt::IInspectable data, std::optional<bool> atEnd)
	{
		if (ReadLocalValue(MaximumTokensProperty()) != winrt::DependencyProperty::UnsetValue() && (MaximumTokens() <= 0 || MaximumTokens() <= static_cast<int>(_innerItemsSource.ItemsSource().Size())))
		{
			// No tokens for you
			co_return;
		}

		if (auto str = data.try_as<winrt::hstring>(); str && _tokenItemAdding)
		{
			auto tiaea = winrt::make_self<TokenItemAddingEventArgs>(*str);

			_tokenItemAdding(*this, *tiaea);

			co_await tiaea->wait_for_deferrals();

			if (tiaea->Cancel())
			{
				co_return;
			}

			if (tiaea->Item())
			{
				data = tiaea->Item(); // Transformed by event implementor
			}
		}

		// If we've been typing in the last box, just add this to the end of our collection
		if (atEnd == true || _currentTextEdit == _lastTextEdit)
		{
			_innerItemsSource.InsertAt(_innerItemsSource.Size() - 1, data);
		}
		else
		{
			// Otherwise, we'll insert before our current box
			const auto& edit = _currentTextEdit;

			if (uint32_t index; _innerItemsSource.IndexOf(edit, index))
			{
				// Insert our new data item at the location of our textbox
				_innerItemsSource.InsertAt(index, data);
			}

			// Remove our textbox
			if (uint32_t index; _innerItemsSource.IndexOf(edit, index)) 
			{ 
				_innerItemsSource.RemoveAt(index);
			}
		}

		// Focus back to our end box as Outlook does.
		if (auto last = ContainerFromItem(_lastTextEdit).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			auto self = winrt::get_self<TokenizingTextBoxItem>(last)->get_strong();
			self->_autoSuggestTextBox.Focus(winrt::FocusState::Keyboard);
		}

		TokenItemAdded.invoke(*this, data);

		GuardAgainstPlaceholderTextLayoutIssue();
	}

	void TokenizingTextBox::UpdateCurrentTextEdit(ITokenStringContainer const& edit)
	{
		_currentTextEdit = edit;

		Text(edit.Text()); // Update our text property.
	}

	winrt::AutomationPeer TokenizingTextBox::OnCreateAutomationPeer()
	{
		return winrt::make<TokenizingTextBoxAutomationPeer>(*this);
	}

	winrt::IAsyncOperation<bool> TokenizingTextBox::RemoveTokenAsync(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, winrt::IInspectable data)
	{
		if (data == nullptr)
		{
			data = ItemFromContainer(item);
		}

		if (_tokenItemRemoving) 
		{
			auto tirea = winrt::make_self<TokenItemRemovingEventArgs>(data, item);

			_tokenItemRemoving(*this, *tirea);

			co_await tirea->wait_for_deferrals();

			if (tirea->Cancel())
			{
				co_return false;
			}
		}

		if (uint32_t index; _innerItemsSource.IndexOf(data, index)) 
		{
			_innerItemsSource.RemoveAt(index);
		}

		TokenItemRemoved.invoke(*this, data);

		GuardAgainstPlaceholderTextLayoutIssue();

		co_return true;
	}

	static void QueueCallbackForCompositionRendering(std::function<void()> callback)
	{
		try
		{
			auto renderingEventToken = std::make_shared<winrt::event_token>();
			*renderingEventToken = winrt::CompositionTarget::Rendering([renderingEventToken, callback](auto&, auto&)
			{
				// Detach event or Rendering will keep calling us back.
				winrt::CompositionTarget::Rendering(*renderingEventToken);

				callback();
			});
		}
		catch (winrt::hresult_error& e)
		{
			// DirectUI::CompositionTarget::add_Rendering can fail with RPC_E_WRONG_THREAD if called while the Xaml Core is being shutdown,
			// and there is evidence from Watson that such calls are made in real apps (see Bug 13554197).
			// Since the core is being shutdown, we no longer care about whatever work we wanted to defer to CT.Rendering, so ignore this error.
			if (e.to_abi() != RPC_E_WRONG_THREAD) { throw; }
		}
	}

	void TokenizingTextBox::GuardAgainstPlaceholderTextLayoutIssue()
	{
		// If the *PlaceholderText is visible* on the last AutoSuggestBox, it can incorrectly layout itself
		// when the *ASB has focus*. We think this is an optimization in the platform, but haven't been able to
		// isolate a straight-reproduction of this issue outside of this control (though we have eliminated
		// most Toolkit influences like ASB/TextBox Style, the InterspersedObservableCollection, etc...).
		// The only Toolkit component involved here should be WrapPanel (which is a straight-forward Panel).
		// We also know the ASB itself is adjusting it's size correctly, it's the inner component.
		//
		// To combat this issue:
		//   We toggle the visibility of the Placeholder ContentControl in order to force it's layout to update properly
		auto placeholder = DependencyObjectEx::FindDescendant(ContainerFromItem(_lastTextEdit), L"PlaceholderTextContentPresenter");

		if (placeholder && placeholder.Visibility() == winrt::Visibility::Visible)
		{
			placeholder.Visibility(winrt::Visibility::Collapsed);

			// After we ensure we've hid the control, make it visible again (this is imperceptible to the user).
			QueueCallbackForCompositionRendering([=]()
			{
				placeholder.Visibility(winrt::Visibility::Visible);
			});
		}
	}

	void TokenizingTextBox::OnTextPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto ttb = winrt::get_self<TokenizingTextBox>(control)->get_strong();

			if (auto newValue = e.NewValue().try_as<winrt::hstring>())
			{
				if (ttb->_currentTextEdit)
				{
					ttb->_currentTextEdit.Text(*newValue);
					if (auto item = ttb->ContainerFromItem(ttb->_currentTextEdit).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
					{
						auto itemSelf = winrt::get_self<TokenizingTextBoxItem>(item)->get_strong();
						itemSelf->UpdateText(ttb->_currentTextEdit.Text());
					}
				}
			}
		}
	}

	void TokenizingTextBox::OnMaximumTokensChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto ttb = d.try_as<class_type>(); 
			ttb && ttb.ReadLocalValue(MaximumTokensProperty) != winrt::DependencyProperty::UnsetValue())
		{
			if (auto newMaxTokens = e.NewValue().try_as<int>())
			{
				auto ttbSelf = winrt::get_self<TokenizingTextBox>(ttb)->get_strong();
				int tokenCount = static_cast<int>(ttbSelf->_innerItemsSource.ItemsSource().Size());
				if (tokenCount > 0 && tokenCount > newMaxTokens.value())
				{
					int tokensToRemove = tokenCount - std::max<int>(*newMaxTokens, 0);

					// Start at the end, remove any extra tokens.
					for (auto i = tokenCount; i > tokenCount - tokensToRemove; --i)
					{
						auto token = ttbSelf->_innerItemsSource.ItemsSource().GetAt(i - 1);

						if (token)
						{
							// Force remove the items. No warning and no option to cancel.
							if (uint32_t index; ttbSelf->_innerItemsSource.IndexOf(token, index))
								ttbSelf->_innerItemsSource.RemoveAt(index);

							ttbSelf->TokenItemRemoved.invoke(ttb, token);
						}
					}
				}
			}
		}
	}

	winrt::event_token TokenizingTextBox::TokenItemAdding(
		TypedEventHandler<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox,
		winrt::XamlToolkit::WinUI::Controls::TokenItemAddingEventArgs> const& handler)
	{
		return _tokenItemAdding.add(handler);
	}

	void TokenizingTextBox::TokenItemAdding(winrt::event_token const& token) noexcept
	{
		_tokenItemAdding.remove(token);
	}

	winrt::event_token TokenizingTextBox::TokenItemRemoving(
		TypedEventHandler<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox,
		winrt::XamlToolkit::WinUI::Controls::TokenItemRemovingEventArgs> const& handler)
	{
		return _tokenItemRemoving.add(handler);
	}

	void TokenizingTextBox::TokenItemRemoving(winrt::event_token const& token) noexcept
	{
		_tokenItemRemoving.remove(token);
	}
}
