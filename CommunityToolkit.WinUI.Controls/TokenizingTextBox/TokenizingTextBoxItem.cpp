#include "pch.h"
#include "TokenizingTextBoxItem.h"
#if __has_include("TokenizingTextBoxItem.g.cpp")
#include "TokenizingTextBoxItem.g.cpp"
#endif
#include "../CommunityToolkit.WinUI/common.h"
#include "TokenizingTextBox.h"
#include "StringExtensions.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	static inline winrt::com_ptr<implementation::TokenizingTextBox> OwnerAsImpl(
		winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox const& value)
	{
		return winrt::get_self<implementation::TokenizingTextBox>(value)->get_strong();
	}

	TokenizingTextBoxItem::TokenizingTextBoxItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// TODO: only add these if token?
		RightTapped({ this, &TokenizingTextBoxItem::TokenizingTextBoxItem_RightTapped });
		KeyDown({ this, &TokenizingTextBoxItem::TokenizingTextBoxItem_KeyDown });
	}

	void TokenizingTextBoxItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (auto suggestbox = GetTemplateChild(PART_AutoSuggestBox).try_as<AutoSuggestBox>())
		{
			OnApplyTemplateAutoSuggestBox(suggestbox);
		}

		if (_clearButton)
		{
			_clearButton.Click(_buttonClickToken);
		}

		_clearButton = GetTemplateChild(PART_ClearButton).try_as<Button>();

		if (_clearButton)
		{
			_buttonClickToken = _clearButton.Click({ get_weak(), &TokenizingTextBoxItem::ClearButton_Click});
		}
	}

	void TokenizingTextBoxItem::ClearButton_Click([[maybe_unused]] IInspectable const& sender, RoutedEventArgs const& e)
	{
		ClearClicked.invoke(*this, e);
	}

	void TokenizingTextBoxItem::TokenizingTextBoxItem_RightTapped([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RightTappedRoutedEventArgs const& e)
	{
		ContextFlyout().ShowAt(*this);
	}

	void TokenizingTextBoxItem::TokenizingTextBoxItem_KeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (!(Content().try_as<ITokenStringContainer>()))
		{
			// We only want to 'remove' our token if we're not a textbox.
			switch (e.Key())
			{
			case VirtualKey::Back:
			case VirtualKey::Delete:
			{
				ClearAllAction.invoke(*this, e);
				break;
			}
			}
		}
	}

	void TokenizingTextBoxItem::OnApplyTemplateAutoSuggestBox(AutoSuggestBox const& box)
	{
		if (_autoSuggestBox)
		{
			_autoSuggestBox.Loaded(_autoSuggestBoxLoadedToken);

			_autoSuggestBox.QuerySubmitted(_autoSuggestBoxQuerySubmittedToken);
			_autoSuggestBox.SuggestionChosen(_autoSuggestBoxSuggestionChosenToken);
			_autoSuggestBox.TextChanged(_autoSuggestBoxTextChangedToken);
			_autoSuggestBox.PointerEntered(_autoSuggestBoxPointerEnteredToken);
			_autoSuggestBox.PointerExited(_autoSuggestBoxPointerExitedToken);
			_autoSuggestBox.PointerCanceled(_autoSuggestBoxPointerCanceledToken);
			_autoSuggestBox.PointerCaptureLost(_autoSuggestBoxPointerCaptureLostToken);
			_autoSuggestBox.GotFocus(_autoSuggestBoxGotFocusToken);
			_autoSuggestBox.LostFocus(_autoSuggestBoxLostFocusToken);

			// Remove any previous QueryIcon
			_autoSuggestBox.QueryIcon(nullptr);
		}

		_autoSuggestBox = box;

		if (_autoSuggestBox)
		{
			_autoSuggestBoxLoadedToken = _autoSuggestBox.Loaded({ this, &TokenizingTextBoxItem::OnASBLoaded });

			_autoSuggestBoxQuerySubmittedToken = _autoSuggestBox.QuerySubmitted({ this, &TokenizingTextBoxItem::AutoSuggestBox_QuerySubmitted });
			_autoSuggestBoxSuggestionChosenToken = _autoSuggestBox.SuggestionChosen({ this, &TokenizingTextBoxItem::AutoSuggestBox_SuggestionChosen });
			_autoSuggestBoxTextChangedToken = _autoSuggestBox.TextChanged({ this, &TokenizingTextBoxItem::AutoSuggestBox_TextChanged });
			_autoSuggestBoxPointerEnteredToken = _autoSuggestBox.PointerEntered({ this, &TokenizingTextBoxItem::AutoSuggestBox_PointerEntered });
			_autoSuggestBoxPointerExitedToken = _autoSuggestBox.PointerExited({ this, &TokenizingTextBoxItem::AutoSuggestBox_PointerExited });
			_autoSuggestBoxPointerCanceledToken = _autoSuggestBox.PointerCanceled({ this, &TokenizingTextBoxItem::AutoSuggestBox_PointerExited });
			_autoSuggestBoxPointerCaptureLostToken = _autoSuggestBox.PointerCaptureLost({ this, &TokenizingTextBoxItem::AutoSuggestBox_PointerExited });
			_autoSuggestBoxGotFocusToken = _autoSuggestBox.GotFocus({ this, &TokenizingTextBoxItem::AutoSuggestBox_GotFocus });
			_autoSuggestBoxLostFocusToken = _autoSuggestBox.LostFocus({ this, &TokenizingTextBoxItem::AutoSuggestBox_LostFocus });

			// Setup a binding to the QueryIcon of the Parent if we're the last box.
			if (auto str = Content().try_as<ITokenStringContainer>())
			{
				// We need to set our initial text in all cases.
				_autoSuggestBox.Text(str.Text());

				// We only set/bind some properties on the last textbox to mimic the autosuggestbox look
				if (str.IsLast())
				{
					// Workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/2568
					if (auto fis = Owner().QueryIcon().try_as<FontIconSource>(); fis &&
						fis.ReadLocalValue(FontIconSource::FontSizeProperty()) == DependencyProperty::UnsetValue())
					{
						// This can be expensive, could we optimize?
						// Also, this is changing the FontSize on the IconSource (which could be shared?)
						if (auto value = FrameworkElementEx::TryFindResource(Owner(), winrt::box_value(L"TokenizingTextBoxIconFontSize")))
						{
							fis.FontSize(winrt::unbox_value_or<double>(value, 16));
						}
					}

					winrt::Microsoft::UI::Xaml::Data::Binding iconBinding;
					iconBinding.Source(Owner());
					iconBinding.Path(PropertyPath(L"QueryIcon"));
					winrt::Microsoft::UI::Xaml::Data::RelativeSource relativeSource;
					relativeSource.Mode(winrt::Microsoft::UI::Xaml::Data::RelativeSourceMode::TemplatedParent);
					iconBinding.RelativeSource(relativeSource);

					IconSourceElement iconSourceElement;
					iconSourceElement.SetBinding(IconSourceElement::IconSourceProperty(), iconBinding);
					_autoSuggestBox.QueryIcon(iconSourceElement);
				}
			}
		}
	}

	IAsyncAction TokenizingTextBoxItem::AutoSuggestBox_QuerySubmitted(AutoSuggestBox const& sender, AutoSuggestBoxQuerySubmittedEventArgs const& args)
	{
		auto owner = OwnerAsImpl(Owner());
		owner->RaiseQuerySubmitted(sender, args);

		IInspectable chosenItem{ nullptr };
		if (auto chosenSuggestion = args.ChosenSuggestion())
		{
			chosenItem = chosenSuggestion;
		}
		else if (!IsNullOrWhiteSpace(args.QueryText()))
		{
			chosenItem = winrt::box_value(args.QueryText());
		}

		if (chosenItem)
		{
			co_await owner->AddTokenAsync(chosenItem); // TODO: Need to pass index?
			sender.Text(L"");
			Owner().Text(L"");
			sender.Focus(FocusState::Programmatic);
		}
	}

	void TokenizingTextBoxItem::AutoSuggestBox_SuggestionChosen(AutoSuggestBox const& sender, AutoSuggestBoxSuggestionChosenEventArgs const& args) const
	{
		auto owner = OwnerAsImpl(Owner());
		owner->RaiseSuggestionChosen(sender, args);
	}

	// Called to update text by link:TokenizingTextBox.Properties.cs:TextPropertyChanged
	void TokenizingTextBoxItem::TokenizingTextBoxItem::UpdateText(hstring const& text)
	{
		if (_autoSuggestBox)
		{
			_autoSuggestBox.Text(text);
		}
		else
		{
			auto waitForLoadToken = std::make_shared<winrt::event_token>();
			auto WaitForLoad = [weakRef(get_weak()), text, waitForLoadToken]([[maybe_unused]] IInspectable const& s, [[maybe_unused]] RoutedEventArgs const& args)
				{
					if (auto tbRef = weakRef.get())
					{
						if (tbRef->_autoSuggestTextBox)
						{
							tbRef->_autoSuggestTextBox.Text(text);
						}

						tbRef->AutoSuggestTextBoxLoaded(*waitForLoadToken);
					}
				};

			*waitForLoadToken = AutoSuggestTextBoxLoaded(WaitForLoad);
		}
	}

	void TokenizingTextBoxItem::AutoSuggestBox_TextChanged(AutoSuggestBox const& sender, AutoSuggestBoxTextChangedEventArgs const& args)
	{
		auto text = sender.Text();
		if (text.empty())
		{
			return;
		}
		auto owner = OwnerAsImpl(Owner());
		if (text != owner->Text())
		{
			owner->Text(text); // Update parent text property, if different
		}

		// Override our programmatic manipulation as we're redirecting input for the user
		if (UseCharacterAsUser())
		{
			UseCharacterAsUser = false;

			args.Reason(AutoSuggestionBoxTextChangeReason::UserInput);
		}

		owner->RaiseTextChanged(sender, args);

		auto trimed = Trim(text);

		// Look for Token Delimiters to create new tokens when text changes.
		if (auto delimiter = owner->TokenDelimiter(); !delimiter.empty() && trimed.contains(delimiter))
		{
			bool lastDelimited = trimed[trimed.size() - 1] == owner->TokenDelimiter()[0];

			auto tokens = Split(trimed, std::wstring_view(owner->TokenDelimiter())) | std::ranges::to<std::vector>();

			int numberToProcess = lastDelimited ? static_cast<int>(tokens.size()) : static_cast<int>(tokens.size()) - 1;
			for (int position = 0; position < numberToProcess; position++)
			{
				auto token = tokens[position];
				token = Trim(token);
				if (token.size() > 0)
				{
					winrt::hstring tokenStr { token };
					owner->AddTokenAsync(winrt::box_value(tokenStr)); //// TODO: Pass Index?
				}
			}

			if (lastDelimited)
			{
				sender.Text(L"");
			}
			else
			{
				sender.Text(Trim(tokens[tokens.size() - 1]));
			}
		}
	}

	void TokenizingTextBoxItem::AutoSuggestBox_PointerEntered([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e) const
	{
		VisualStateManager::GoToState(Owner(), TokenizingTextBox::PART_PointerOverState, true);
	}

	void TokenizingTextBoxItem::AutoSuggestBox_PointerExited([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e) const
	{
		VisualStateManager::GoToState(Owner(), TokenizingTextBox::PART_NormalState, true);
	}

	void TokenizingTextBoxItem::AutoSuggestBox_LostFocus([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e) const
	{
		VisualStateManager::GoToState(Owner(), TokenizingTextBox::PART_UnfocusedState, true);
	}

	void TokenizingTextBoxItem::AutoSuggestBox_GotFocus([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e) const
	{
		auto owner = OwnerAsImpl(Owner());
		// Verify if the usual behavior of clearing token selection is required
		if (owner->PauseTokenClearOnFocus() == false && !TokenizingTextBox::IsShiftPressed())
		{
			// Clear any selected tokens
			ListViewExtensions::DeselectAll(*owner);
		}

		owner->PauseTokenClearOnFocus(false);

		VisualStateManager::GoToState(*owner, TokenizingTextBox::PART_FocusedState, true);
	}

	void TokenizingTextBoxItem::OnASBLoaded([[maybe_unused]] IInspectable const& sender, RoutedEventArgs const& e)
	{
		UpdateQueryIconVisibility();
		UpdateTokensCounter(*this);

		// Local function for Selection changed
		auto AutoSuggestTextBox_SelectionChanged = [this]([[maybe_unused]] IInspectable const& box, [[maybe_unused]] RoutedEventArgs const& args)
			{
				auto owner = OwnerAsImpl(Owner());
				if (!(IsAllSelected() || TokenizingTextBox::IsShiftPressed() || TokenizingTextBox::IsClearingForClick))
				{
					owner->DeselectAllTokensAndText(*this);
				}

				// Ensure flag is always reset
				owner->IsClearingForClick = false;
			};

		// local function for clearing selection on interaction with text box
		auto AutoSuggestTextBox_TextChangingAsync = [this](TextBox const& o, TextBoxTextChangingEventArgs const& args) -> IAsyncAction
			{
				auto owner = OwnerAsImpl(Owner());
				// remove any selected tokens.
				if (owner->SelectedItems().Size() > 1)
				{
					co_await owner->RemoveAllSelectedTokens();
				}
			};

		if (_autoSuggestTextBox)
		{
			_autoSuggestTextBox.PreviewKeyDown(_autoSuggestTextBoxPreviewKeyDownToken);
			_autoSuggestTextBox.TextChanging(_autoSuggestTextBoxTextChangingToken);
			_autoSuggestTextBox.SelectionChanged(_autoSuggestTextBoxSelectionChangedToken);
			_autoSuggestTextBox.SelectionChanging(_autoSuggestTextBoxSelectionChangingToken);
		}

		_autoSuggestTextBox = DependencyObjectEx::FindDescendant<TextBox>(_autoSuggestBox);

		if (_autoSuggestTextBox)
		{
			_autoSuggestTextBoxPreviewKeyDownToken = _autoSuggestTextBox.PreviewKeyDown({ this, &TokenizingTextBoxItem::AutoSuggestTextBox_PreviewKeyDown });
			_autoSuggestTextBoxTextChangingToken = _autoSuggestTextBox.TextChanging(AutoSuggestTextBox_TextChangingAsync);
			_autoSuggestTextBoxSelectionChangedToken = _autoSuggestTextBox.SelectionChanged(AutoSuggestTextBox_SelectionChanged);
			_autoSuggestTextBoxSelectionChangingToken = _autoSuggestTextBox.SelectionChanging({ this, &TokenizingTextBoxItem::AutoSuggestTextBox_SelectionChanging });

			AutoSuggestTextBoxLoaded.invoke(*this, e);
		}
	}

	void TokenizingTextBoxItem::AutoSuggestTextBox_SelectionChanging([[maybe_unused]] TextBox const& sender, TextBoxSelectionChangingEventArgs const& args)
	{
		_isSelectedFocusOnFirstCharacter = args.SelectionLength() > 0 && args.SelectionStart() == 0 && _autoSuggestTextBox.SelectionStart() > 0;
		_isSelectedFocusOnLastCharacter =
			//// see if we are NOW on the last character.
			//// test if the new selection includes the last character, and the current selection doesn't
			(args.SelectionStart() + args.SelectionLength() == static_cast<int>(_autoSuggestTextBox.Text().size())) &&
			(_autoSuggestTextBox.SelectionStart() + _autoSuggestTextBox.SelectionLength() != static_cast<int>(_autoSuggestTextBox.Text().size()));
	}

	void TokenizingTextBoxItem::AutoSuggestTextBox_PreviewKeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (IsCaretAtStart() &&
			(e.Key() == VirtualKey::Back ||
				e.Key() == VirtualKey::Left))
		{
			// if the back key is pressed and there is any selection in the text box then the text box can handle it
			if ((e.Key() == VirtualKey::Left && _isSelectedFocusOnFirstCharacter) ||
				_autoSuggestTextBox.SelectionLength() == 0)
			{
				auto owner = OwnerAsImpl(Owner());
				if (owner->SelectPreviousItem(*this))
				{
					if (!TokenizingTextBox::IsShiftPressed())
					{
						// Clear any text box selection
						_autoSuggestTextBox.SelectionLength(0);
					}

					e.Handled(true);
				}
			}
		}
		else if (IsCaretAtEnd() && e.Key() == VirtualKey::Right)
		{
			// if the back key is pressed and there is any selection in the text box then the text box can handle it
			if (_isSelectedFocusOnLastCharacter || _autoSuggestTextBox.SelectionLength() == 0)
			{
				auto owner = OwnerAsImpl(Owner());
				if (owner->SelectNextItem(*this))
				{
					if (!TokenizingTextBox::IsShiftPressed())
					{
						// Clear any text box selection
						_autoSuggestTextBox.SelectionLength(0);
					}

					e.Handled(true);
				}
			}
		}
		else if (e.Key() == VirtualKey::A && TokenizingTextBox::IsControlPressed())
		{
			auto owner = OwnerAsImpl(Owner());
			// Need to provide this shortcut from the textbox only, as ListViewBase will do it for us on token.
			owner->SelectAllTokensAndText();
		}
	}

	void TokenizingTextBoxItem::UpdateTokensCounter(TokenizingTextBoxItem& ttbi)
	{
		if (auto maxTokensCounter = DependencyObjectEx::FindDescendant(_autoSuggestBox, PART_TokensCounter).try_as<TextBlock>())
		{
			auto OnTokenCountChanged = [=](Controls::TokenizingTextBox const& ttb, [[maybe_unused]] IInspectable const& value = nullptr)
				{
					if (auto itemsSource = ttb.ItemsSource().try_as<InterspersedObservableVector>())
					{
						auto currentTokens = itemsSource->ItemsSource().Size();
						auto maxTokens = ttb.MaximumTokens();

						maxTokensCounter.Text(winrt::format(L"{}/{}", currentTokens, maxTokens));
						maxTokensCounter.Visibility(Visibility::Visible);

						maxTokensCounter.Foreground((static_cast<int>(currentTokens) >= maxTokens)
							? Application::Current().Resources().TryLookup(winrt::box_value(L"SystemFillColorCriticalBrush")).as<SolidColorBrush>()
							: Application::Current().Resources().TryLookup(winrt::box_value(L"TextFillColorSecondaryBrush")).as<SolidColorBrush>());
					}
				};

			ttbi.Owner().TokenItemAdded(ttbi._tokenItemAddedToken);
			ttbi.Owner().TokenItemRemoved(ttbi._tokenItemRemovedToken);

			if (auto str = Content().try_as<ITokenStringContainer>(); str && str.IsLast() && ttbi.Owner() && ttbi.Owner().ReadLocalValue(TokenizingTextBox::MaximumTokensProperty) != DependencyProperty::UnsetValue())
			{
				ttbi._tokenItemAddedToken = ttbi.Owner().TokenItemAdded(OnTokenCountChanged);
				ttbi._tokenItemRemovedToken = ttbi.Owner().TokenItemRemoved(OnTokenCountChanged);
				OnTokenCountChanged(ttbi.Owner());
			}
			else
			{
				maxTokensCounter.Visibility(Visibility::Collapsed);
				maxTokensCounter.Text(L"");
			}
		}
	}

	void TokenizingTextBoxItem::UpdateQueryIconVisibility()
	{
		if (auto queryButton = DependencyObjectEx::FindDescendant(_autoSuggestBox, QueryButton).try_as<Button>())
		{
			if (Owner().QueryIcon())
			{
				queryButton.Visibility(Visibility::Visible);
			}
			else
			{
				queryButton.Visibility(Visibility::Collapsed);
			}
		}
	}
}
