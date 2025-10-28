#pragma once

#include "TokenizingTextBoxItem.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct TokenizingTextBoxItem : TokenizingTextBoxItemT<TokenizingTextBoxItem>
	{
		TextBox _autoSuggestTextBox{ nullptr };
	private:
		static constexpr std::wstring_view PART_ClearButton = L"PART_RemoveButton";

		static constexpr std::wstring_view PART_AutoSuggestBox = L"PART_AutoSuggestBox";
		static constexpr std::wstring_view PART_TokensCounter = L"PART_TokensCounter";
		static constexpr std::wstring_view QueryButton = L"QueryButton";

		Button _clearButton{ nullptr };

		AutoSuggestBox _autoSuggestBox{ nullptr };

		bool _isSelectedFocusOnFirstCharacter = false;

		bool _isSelectedFocusOnLastCharacter = false;

		winrt::event_token _buttonClickToken;

		winrt::event_token _autoSuggestBoxLoadedToken;

		winrt::event_token _autoSuggestBoxQuerySubmittedToken;
		winrt::event_token _autoSuggestBoxSuggestionChosenToken;
		winrt::event_token _autoSuggestBoxTextChangedToken;
		winrt::event_token _autoSuggestBoxPointerEnteredToken;
		winrt::event_token _autoSuggestBoxPointerExitedToken;
		winrt::event_token _autoSuggestBoxPointerCanceledToken;
		winrt::event_token _autoSuggestBoxPointerCaptureLostToken;
		winrt::event_token _autoSuggestBoxGotFocusToken;
		winrt::event_token _autoSuggestBoxLostFocusToken;

		winrt::event_token _autoSuggestTextBoxPreviewKeyDownToken;
		winrt::event_token _autoSuggestTextBoxTextChangingToken;
		winrt::event_token _autoSuggestTextBoxSelectionChangedToken;
		winrt::event_token _autoSuggestTextBoxSelectionChangingToken;

		winrt::event_token _tokenItemAddedToken;
		winrt::event_token _tokenItemRemovedToken;

	public:
		winrt::event_token _clearClickedToken;

		winrt::event_token _clearAllActionToken;
		winrt::event_token _gotFocusToken;
		winrt::event_token _lostFocusToken;

		wil::typed_event<Controls::TokenizingTextBoxItem, RoutedEventArgs> ClearClicked;

		wil::typed_event<Controls::TokenizingTextBoxItem, RoutedEventArgs> ClearAllAction;

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ClearButtonStyleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"ClearButtonStyle",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::Visibility::Collapsed) }
			);

		winrt::Microsoft::UI::Xaml::Style ClearButtonStyle()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Style>(GetValue(ClearButtonStyleProperty));
		}

		void ClearButtonStyle(winrt::Microsoft::UI::Xaml::Style const& value)
		{
			SetValue(ClearButtonStyleProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OwnerProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Owner",
				winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr }
			);

		winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox Owner() const
		{
			return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox>(GetValue(OwnerProperty));
		}

		void Owner(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox const& value)
		{
			SetValue(OwnerProperty, winrt::box_value(value));
		}

		TokenizingTextBoxItem();

		void OnApplyTemplate();

		wil::typed_event<Controls::TokenizingTextBoxItem, RoutedEventArgs> AutoSuggestTextBoxLoaded;

		wil::single_threaded_rw_property<bool> UseCharacterAsUser;

		void UpdateText(winrt::hstring const& text);
	private:

		void ClearButton_Click(IInspectable const& sender, RoutedEventArgs const& e);

		void TokenizingTextBoxItem_RightTapped(IInspectable const& sender, RightTappedRoutedEventArgs const& e);

		void TokenizingTextBoxItem_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		bool IsCaretAtStart() { return _autoSuggestTextBox && _autoSuggestTextBox.SelectionStart() == 0; }

		bool IsCaretAtEnd()
		{
			if (!_autoSuggestTextBox)
				return false;

			auto text = _autoSuggestTextBox.Text();
			auto start = _autoSuggestTextBox.SelectionStart();
			auto length = _autoSuggestTextBox.SelectionLength();

			return (start == static_cast<int>(text.size())) || (start + length == static_cast<int>(text.size()));
		}

		bool IsAllSelected()
		{
			if (!_autoSuggestTextBox)
				return false;

			auto text = _autoSuggestTextBox.Text();
			auto selected = _autoSuggestTextBox.SelectedText();

			return (!text.empty() && selected == text);
		}

		void OnApplyTemplateAutoSuggestBox(AutoSuggestBox const& box);

		IAsyncAction AutoSuggestBox_QuerySubmitted(AutoSuggestBox const& sender, AutoSuggestBoxQuerySubmittedEventArgs const& args);

		void AutoSuggestBox_SuggestionChosen(AutoSuggestBox const& sender, AutoSuggestBoxSuggestionChosenEventArgs const& args) const;

		void AutoSuggestBox_TextChanged(AutoSuggestBox const& sender, AutoSuggestBoxTextChangedEventArgs const& args);

		void AutoSuggestBox_PointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& e) const;

		void AutoSuggestBox_PointerExited(IInspectable const& sender, PointerRoutedEventArgs const& e) const;

		void AutoSuggestBox_LostFocus(IInspectable const& sender, RoutedEventArgs const& e) const;

		void AutoSuggestBox_GotFocus(IInspectable const& sender, RoutedEventArgs const& e) const;

		void OnASBLoaded(IInspectable const& sender, RoutedEventArgs const& e);

		void AutoSuggestTextBox_SelectionChanging(TextBox const& sender, TextBoxSelectionChangingEventArgs const& args);

		void AutoSuggestTextBox_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void UpdateTokensCounter(TokenizingTextBoxItem& ttbi);

		void UpdateQueryIconVisibility();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBoxItem : TokenizingTextBoxItemT<TokenizingTextBoxItem, implementation::TokenizingTextBoxItem>
	{
	};
}
