#include "pch.h"
#include "TokenizingTextBox.h"
#if __has_include("TokenizingTextBox.g.cpp")
#include "TokenizingTextBox.g.cpp"
#endif
#include <wctype.h>
#include "TokenizingTextBoxItem.h"
#include "PretokenStringContainer.h"
#include "TokenizingTextBoxAutomationPeer.h"
#include "../CommunityToolkit.WinUI/common.h"
#include "StringExtensions.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	TokenizingTextBox::TokenizingTextBox()
	{
		// Setup our base state of our collection
		_innerItemsSource = winrt::make<InterspersedObservableVector>(winrt::single_threaded_observable_vector<IInspectable>()); // TODO: Test this still will let us bind to ItemsSource in XAML?
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

	void TokenizingTextBox::ItemsSource_PropertyChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		// If we're given a different ItemsSource, we need to wrap that collection in our helper class.
		auto itemsSource = ItemsSource();
		if (itemsSource && !itemsSource.try_as<InterspersedObservableVector>())
		{
			_innerItemsSource = winrt::make<InterspersedObservableVector>(itemsSource);
			auto maximumTokens = MaximumTokens();
			if (ReadLocalValue(MaximumTokensProperty) != DependencyProperty::UnsetValue() && static_cast<int>(_innerItemsSource.ItemsSource().Size()) >= maximumTokens)
			{
				// Reduce down to below the max as necessary.
				auto endCount = maximumTokens > 0 ? maximumTokens : 0;
				for (auto i = static_cast<int>(_innerItemsSource.ItemsSource().Size()) - 1; i >= endCount; --i)
				{
					_innerItemsSource.Remove(_innerItemsSource.GetAt(i));
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

	void TokenizingTextBox::TokenizingTextBox_ItemClick([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] ItemClickEventArgs const& e)
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

	void TokenizingTextBox::TokenizingTextBox_PreviewKeyUp([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		OnTokenizingTextBoxPreviewKeyUp(e.Key());
	}

	void TokenizingTextBox::OnTokenizingTextBoxPreviewKeyUp(VirtualKey key)
	{
		switch (key)
		{
		case VirtualKey::Escape:
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
		auto items = Items();
		if (items && items.Size() > 0)
		{
			if (auto container = ContainerFromIndex(items.Size() - 1).try_as<Controls::TokenizingTextBoxItem>())
			{
				container.Focus(FocusState::Programmatic);
			}
		}
	}

	IAsyncAction TokenizingTextBox::TokenizingTextBox_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		auto args = e;
		args.Handled(co_await OnTokenizingTextBoxPreviewKeyDown(e.Key()));
	}

	IAsyncOperation<bool> TokenizingTextBox::OnTokenizingTextBoxPreviewKeyDown(VirtualKey key)
	{
		// Global handlers on control regardless if focused on item or in textbox.
		switch (key)
		{
		case VirtualKey::C:
			if (IsControlPressed())
			{
				CopySelectedToClipboard();
				co_return true;
			}

			break;

		case VirtualKey::X:
			if (IsControlPressed())
			{
				CopySelectedToClipboard();

				// now clear all selected tokens and text, or all if none are selected
				co_await RemoveAllSelectedTokens();
			}

			break;

			// For moving between tokens
		case VirtualKey::Left:
			co_return MoveFocusAndSelection(MoveDirection::Previous);

		case VirtualKey::Right:
			co_return MoveFocusAndSelection(MoveDirection::Next);

		case VirtualKey::A:
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

		MenuFlyoutItem selectAllMenuItem;
		selectAllMenuItem.Text(L"Select all");

		selectAllMenuItem.Click({ get_weak(), [this](auto&, auto&) { SelectAllTokensAndText(); } });

		MenuFlyout menuFlyout;
		menuFlyout.Items().Append(selectAllMenuItem);

		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			menuFlyout.XamlRoot(XamlRoot());
		}

		ContextFlyout(menuFlyout);
	}

	void TokenizingTextBox::RaiseQuerySubmitted(AutoSuggestBox const& sender, AutoSuggestBoxQuerySubmittedEventArgs const& args)
	{
		QuerySubmitted.invoke(sender, args);
	}

	void TokenizingTextBox::RaiseSuggestionChosen(AutoSuggestBox const& sender, AutoSuggestBoxSuggestionChosenEventArgs const& args)
	{
		SuggestionChosen.invoke(sender, args);
	}

	void TokenizingTextBox::RaiseTextChanged(AutoSuggestBox const& sender, AutoSuggestBoxTextChangedEventArgs const& args)
	{
		TextChanged.invoke(sender, args);
	}

	IAsyncAction TokenizingTextBox::TokenizingTextBox_CharacterReceived(UIElement const& sender, CharacterReceivedRoutedEventArgs const& args)
	{
		auto container = ContainerFromItem(_currentTextEdit).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>();
		if (container == nullptr) co_return;
		auto self = winrt::get_self<TokenizingTextBoxItem>(container)->get_strong();
		auto character = args.Character();
		if (!(GetFocusedElement() == self->_autoSuggestTextBox || ::iswcntrl(character)))
		{
			if (SelectedItems().Size() > 0)
			{
				uint32_t index;
				_innerItemsSource.IndexOf(SelectedItems().First(), index);
				co_await RemoveAllSelectedTokens();

				std::ignore = this->DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [=]()
					{
						// If we're before the last textbox and it's empty, redirect focus to that one instead
						if (index == _innerItemsSource.Size() - 1 && IsNullOrWhiteSpace(_lastTextEdit.Text()))
						{
							if (auto lastContainer = ContainerFromItem(_lastTextEdit).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
							{
								auto lastSelf = winrt::get_self<TokenizingTextBoxItem>(lastContainer)->get_strong();
								lastSelf->UseCharacterAsUser(true); // Make sure we trigger a refresh of suggested items.

								_lastTextEdit.Text(winrt::hstring{ reinterpret_cast<const wchar_t*>(&character), 1 });

								UpdateCurrentTextEdit(_lastTextEdit);

								lastSelf->_autoSuggestTextBox.SelectionStart(1); // Set position to after our new character inserted

								lastSelf->_autoSuggestTextBox.Focus(FocusState::Keyboard);
							}
						}
						else
						{
							//// Otherwise, create a new textbox for this text.
							auto container = winrt::make<PretokenStringContainer>(false);
							container.Text((::iswspace(character) ? winrt::hstring{} : winrt::hstring{ character }));
							UpdateCurrentTextEdit(container); // Trim so that 'space' isn't inserted and can be used to insert a new box.

							_innerItemsSource.InsertAt(index, _currentTextEdit);

							std::ignore = DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal,
								[=]()
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
						auto position = selectionStart > text.size() ? text.size() : selectionStart;
						textToken.Text(text.substr(0, position) + static_cast<wchar_t>(character) +
							text.substr(position));

						lastSelf->_autoSuggestTextBox.SelectionStart(static_cast<int>(position) + 1); // Set position to after our new character inserted

						lastSelf->_autoSuggestTextBox.Focus(FocusState::Keyboard);
					}
				}
			}
		}
	}

	IInspectable TokenizingTextBox::GetFocusedElement()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			return FocusManager::GetFocusedElement(XamlRoot());
		}
		else
		{
			return FocusManager::GetFocusedElement();
		}
	}

	DependencyObject TokenizingTextBox::GetContainerForItemOverride() { return winrt::make<TokenizingTextBoxItem>(); }

	bool TokenizingTextBox::IsItemItsOwnContainerOverride(IInspectable const& item)
	{
		return item.try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>() != nullptr;
	}

	void TokenizingTextBox::PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);

		if (auto tokenItem = element.try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
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

			MenuFlyout menuFlyout;

			MenuFlyoutItem removeMenuItem;
			removeMenuItem.Text(L"Remove");

			removeMenuItem.Click({ get_weak(), [this, tokenItem](auto&, auto&) { TokenizingTextBoxItem_ClearClicked(tokenItem, nullptr); } });

			menuFlyout.Items().Append(removeMenuItem);

			if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
			{
				menuFlyout.XamlRoot(XamlRoot());
			}

			MenuFlyoutItem selectAllMenuItem;
			selectAllMenuItem.Text(L"Select all");

			selectAllMenuItem.Click({ get_weak(), [this](auto&, auto&) { SelectAllTokensAndText(); } });

			menuFlyout.Items().Append(selectAllMenuItem);

			itemSelf->ContextFlyout(menuFlyout);
		}
	}

	void TokenizingTextBox::TokenizingTextBoxItem_GotFocus(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		// Keep track of our currently focused textbox
		if (auto ttbi = sender.try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			if (auto text = ttbi.Content().try_as<ITokenStringContainer>())
			{
				UpdateCurrentTextEdit(text);
			}
		}
	}

	void TokenizingTextBox::TokenizingTextBoxItem_LostFocus(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		// Keep track of our currently focused textbox
		if (auto ttbi = sender.try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			if (auto text = ttbi.Content().try_as<ITokenStringContainer>(); text &&
				IsNullOrWhiteSpace(text.Text())
				&& text != _lastTextEdit) {

				// We're leaving an inner textbox that's blank, so we'll remove it
				_innerItemsSource.Remove(text);

				UpdateCurrentTextEdit(_lastTextEdit);

				GuardAgainstPlaceholderTextLayoutIssue();
			}
		}
	}

	void TokenizingTextBox::AddTokenItem(IInspectable& data, bool atEnd)
	{
		std::ignore = AddTokenAsync(data, atEnd);
	}

	IAsyncAction TokenizingTextBox::ClearAsync()
	{
		while (_innerItemsSource.Size() > 1)
		{
			auto item = _innerItemsSource.GetAt(0);
			if (auto container = ContainerFromItem(item).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
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

	IAsyncAction TokenizingTextBox::AddTokenAsync(IInspectable data, std::optional<bool> atEnd)
	{
		if (ReadLocalValue(MaximumTokensProperty) != DependencyProperty::UnsetValue() && (MaximumTokens() <= 0 || MaximumTokens() <= static_cast<int>(_innerItemsSource.ItemsSource().Size())))
		{
			// No tokens for you
			co_return;
		}

		if (auto str = data.try_as<hstring>())
		{
			auto tiaea = TokenItemAddingEventArgs(*str);
			TokenItemAdding.invoke(*this, tiaea);

			if (tiaea.Cancel())
			{
				co_return;
			}

			if (tiaea.Item())
			{
				data = tiaea.Item(); // Transformed by event implementor
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
			uint32_t index;
			_innerItemsSource.IndexOf(edit, index);

			// Insert our new data item at the location of our textbox
			_innerItemsSource.InsertAt(index, data);

			// Remove our textbox
			_innerItemsSource.Remove(edit);
		}

		// Focus back to our end box as Outlook does.
		if (auto last = ContainerFromItem(_lastTextEdit).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
		{
			auto self = winrt::get_self<TokenizingTextBoxItem>(last)->get_strong();
			self->_autoSuggestTextBox.Focus(FocusState::Keyboard);
		}

		TokenItemAdded.invoke(*this, data);

		GuardAgainstPlaceholderTextLayoutIssue();
	}

	void TokenizingTextBox::UpdateCurrentTextEdit(ITokenStringContainer const& edit)
	{
		_currentTextEdit = edit;

		Text(edit.Text()); // Update our text property.
	}

	AutomationPeer TokenizingTextBox::OnCreateAutomationPeer()
	{
		return winrt::make<TokenizingTextBoxAutomationPeer>(*this);
	}

	IAsyncOperation<bool> TokenizingTextBox::RemoveTokenAsync(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, IInspectable data)
	{
		if (data == nullptr)
		{
			data = ItemFromContainer(item);
		}

		auto tirea = TokenItemRemovingEventArgs(data, item);
		TokenItemRemoving.invoke(*this, tirea);

		if (tirea.Cancel())
		{
			co_return false;
		}

		_innerItemsSource.Remove(data);

		TokenItemRemoved.invoke(*this, data);

		GuardAgainstPlaceholderTextLayoutIssue();

		co_return true;
	}

	static void QueueCallbackForCompositionRendering(std::function<void()> callback)
	{
		try
		{
			auto renderingEventToken = std::make_shared<winrt::event_token>();
			*renderingEventToken = CompositionTarget::Rendering([renderingEventToken, callback](auto&, auto&)
				{
					// Detach event or Rendering will keep calling us back.
					CompositionTarget::Rendering(*renderingEventToken);

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

		if (placeholder && placeholder.Visibility() == Visibility::Visible)
		{
			placeholder.Visibility(Visibility::Collapsed);

			// After we ensure we've hid the control, make it visible again (this is imperceptible to the user).
			QueueCallbackForCompositionRendering([=]()
				{
					placeholder.Visibility(Visibility::Visible);
				});
		}
	}

	void TokenizingTextBox::OnTextPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto ttb = winrt::get_self<TokenizingTextBox>(control)->get_strong();

			if (auto newValue = e.NewValue().try_as<winrt::hstring>())
			{
				if (ttb->_currentTextEdit)
				{
					ttb->_currentTextEdit.Text(*newValue);
					if (auto item = ttb->ContainerFromItem(ttb->_currentTextEdit).try_as<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem>())
					{
						auto itemSelf = winrt::get_self<TokenizingTextBoxItem>(item)->get_strong();
						itemSelf->UpdateText(ttb->_currentTextEdit.Text());
					}
				}
			}
		}
	}

	void TokenizingTextBox::OnMaximumTokensChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto ttb = d.try_as<class_type>(); ttb
			&& ttb.ReadLocalValue(MaximumTokensProperty) != DependencyProperty::UnsetValue())
		{
			if (auto newMaxTokens = e.NewValue().try_as<int>())
			{
				auto ttbSelf = winrt::get_self<TokenizingTextBox>(ttb)->get_strong();
				auto tokenCount = static_cast<int>(ttbSelf->_innerItemsSource.ItemsSource().Size());
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
							ttbSelf->_innerItemsSource.Remove(token);
							ttbSelf->TokenItemRemoved.invoke(ttb, token);
						}
					}
				}
			}
		}
	}
}
