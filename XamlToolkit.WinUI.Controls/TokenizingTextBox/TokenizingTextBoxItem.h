#pragma once

#include "TokenizingTextBoxItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Data;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct TokenizingTextBoxItem : TokenizingTextBoxItemT<TokenizingTextBoxItem>
	{
		winrt::TextBox _autoSuggestTextBox{ nullptr };

		winrt::event_token _clearClickedToken;

		winrt::event_token _clearAllActionToken;
		winrt::event_token _gotFocusToken;
		winrt::event_token _lostFocusToken;

		TokenizingTextBoxItem();

		void OnApplyTemplate();

		wil::typed_event<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem, winrt::RoutedEventArgs> AutoSuggestTextBoxLoaded;

		wil::typed_event<Controls::TokenizingTextBoxItem, RoutedEventArgs> ClearClicked;

		wil::typed_event<Controls::TokenizingTextBoxItem, RoutedEventArgs> ClearAllAction;

		wil::single_threaded_rw_property<bool> UseCharacterAsUser;

		void UpdateText(winrt::hstring const& text);

		static const wil::single_threaded_property<winrt::DependencyProperty> ClearButtonStyleProperty;

		winrt::Style ClearButtonStyle() const
		{
			return GetValue(ClearButtonStyleProperty()).try_as<winrt::Style>();
		}

		void ClearButtonStyle(winrt::Style const& value)
		{
			SetValue(ClearButtonStyleProperty(), value);
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> OwnerProperty;

		winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox Owner() const
		{
			return GetValue(OwnerProperty()).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox>();
		}

		void Owner(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox const& value)
		{
			SetValue(OwnerProperty(), value);
		}

	private:
		void ClearButton_Click(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void TokenizingTextBoxItem_RightTapped(winrt::IInspectable const& sender, winrt::RightTappedRoutedEventArgs const& e);

		void TokenizingTextBoxItem_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		bool IsCaretAtStart() const { return _autoSuggestTextBox && _autoSuggestTextBox.SelectionStart() == 0; }

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

		void OnApplyTemplateAutoSuggestBox(winrt::AutoSuggestBox const& box);

		winrt::IAsyncAction AutoSuggestBox_QuerySubmitted(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxQuerySubmittedEventArgs const& args);

		void AutoSuggestBox_SuggestionChosen(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxSuggestionChosenEventArgs const& args) const;

		void AutoSuggestBox_TextChanged(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxTextChangedEventArgs const& args);

		void AutoSuggestBox_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e) const;

		void AutoSuggestBox_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e) const;

		void AutoSuggestBox_LostFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e) const;

		void AutoSuggestBox_GotFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e) const;

		void OnASBLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void AutoSuggestTextBox_SelectionChanging(winrt::TextBox const& sender, winrt::TextBoxSelectionChangingEventArgs const& args);

		void AutoSuggestTextBox_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void UpdateTokensCounter(TokenizingTextBoxItem& ttbi);

		void UpdateQueryIconVisibility();

	private:
		static constexpr std::wstring_view PART_ClearButton = L"PART_RemoveButton";

		static constexpr std::wstring_view PART_AutoSuggestBox = L"PART_AutoSuggestBox";
		static constexpr std::wstring_view PART_TokensCounter = L"PART_TokensCounter";
		static constexpr std::wstring_view QueryButton = L"QueryButton";

		winrt::Button _clearButton{ nullptr };

		winrt::AutoSuggestBox _autoSuggestBox{ nullptr };

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
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBoxItem : TokenizingTextBoxItemT<TokenizingTextBoxItem, implementation::TokenizingTextBoxItem>
	{
	};
}
