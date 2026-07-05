#pragma once

#include "TokenizingTextBox.g.h"
#include "InterspersedObservableVector.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <functional>
#include <optional>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::System;
	using namespace Windows::Foundation;
	using namespace Windows::ApplicationModel::DataTransfer;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Automation;
	using namespace Microsoft::UI::Xaml::Automation::Peers;
	using namespace Windows::UI::Core;
	using namespace Microsoft::UI::Input;
	using namespace Microsoft::UI::Dispatching;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	enum MoveDirection
	{
		Next,
		Previous
	};

	struct TokenizingTextBox : TokenizingTextBoxT<TokenizingTextBox>
	{
		static constexpr std::wstring_view PART_NormalState = L"Normal";
		static constexpr std::wstring_view PART_PointerOverState = L"PointerOver";
		static constexpr std::wstring_view PART_FocusedState = L"Focused";
		static constexpr std::wstring_view PART_UnfocusedState = L"Unfocused";
		static constexpr std::wstring_view PART_MaxReachedState = L"MaxReachedState";

		TokenizingTextBox();

		static bool IsShiftPressed();

		static bool IsControlPressed();

		bool PauseTokenClearOnFocus;

		bool IsClearingForClick;

		winrt::DependencyObject GetContainerForItemOverride() const;

		bool IsItemItsOwnContainerOverride(winrt::IInspectable const& item);

		void PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item);

		void ItemsSource_PropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		void TokenizingTextBox_ItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e);

		void TokenizingTextBox_PreviewKeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void OnTokenizingTextBoxPreviewKeyUp(winrt::VirtualKey key);

		void FocusPrimaryAutoSuggestBox();

		winrt::IAsyncAction TokenizingTextBox_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs e);

		winrt::IAsyncOperation<bool> OnTokenizingTextBoxPreviewKeyDown(winrt::VirtualKey key);

		void OnApplyTemplate();

		void RaiseQuerySubmitted(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxQuerySubmittedEventArgs const& args);

		void RaiseSuggestionChosen(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxSuggestionChosenEventArgs const& args);

		void RaiseTextChanged(winrt::AutoSuggestBox const& sender, winrt::AutoSuggestBoxTextChangedEventArgs const& args);

		void AddTokenItem(winrt::IInspectable& data, bool atEnd = false);

		winrt::IAsyncAction ClearAsync();

		void UpdateCurrentTextEdit(ITokenStringContainer const& edit);

		winrt::AutomationPeer OnCreateAutomationPeer();

		static const wil::single_threaded_property<winrt::DependencyProperty> AutoSuggestBoxStyleProperty;

		winrt::Style AutoSuggestBoxStyle() const 
		{ 
			return GetValue(AutoSuggestBoxStyleProperty()).try_as<winrt::Style>();
		}
		void AutoSuggestBoxStyle(winrt::Style const& value) const { SetValue(AutoSuggestBoxStyleProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> AutoSuggestBoxTextBoxStyleProperty;

		winrt::Style AutoSuggestBoxTextBoxStyle() const { return GetValue(AutoSuggestBoxTextBoxStyleProperty).try_as<winrt::Style>(); }
		void AutoSuggestBoxTextBoxStyle(winrt::Style const& value) const { SetValue(AutoSuggestBoxTextBoxStyleProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TextMemberPathProperty;

		winrt::hstring TextMemberPath() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextMemberPathProperty)); }
		void TextMemberPath(winrt::hstring const& value) const { SetValue(TextMemberPathProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TokenItemTemplateProperty;

		winrt::DataTemplate TokenItemTemplate() const { return GetValue(TokenItemTemplateProperty).try_as<winrt::DataTemplate>(); }
		void TokenItemTemplate(winrt::DataTemplate const& value) const { SetValue(TokenItemTemplateProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TokenItemTemplateSelectorProperty;

		winrt::DataTemplateSelector TokenItemTemplateSelector() const { return GetValue(TokenItemTemplateSelectorProperty).try_as<winrt::DataTemplateSelector>(); }
		void TokenItemTemplateSelector(winrt::DataTemplateSelector const& value) const { SetValue(TokenItemTemplateSelectorProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TokenDelimiterProperty;

		winrt::hstring TokenDelimiter() const { return winrt::unbox_value<winrt::hstring>(GetValue(TokenDelimiterProperty)); }
		void TokenDelimiter(winrt::hstring const& value) const { SetValue(TokenDelimiterProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TokenSpacingProperty;

		double TokenSpacing() const { return winrt::unbox_value_or(GetValue(TokenSpacingProperty), 0.0); }
		void TokenSpacing(double value) const { SetValue(TokenSpacingProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> PlaceholderTextProperty;

		winrt::hstring PlaceholderText() const { return winrt::unbox_value<winrt::hstring>(GetValue(PlaceholderTextProperty)); }
		void PlaceholderText(winrt::hstring const& value) const { SetValue(PlaceholderTextProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> QueryIconProperty;

		winrt::IconSource QueryIcon() const { return GetValue(QueryIconProperty).try_as<winrt::IconSource>(); }
		void QueryIcon(winrt::IconSource const& value) const { SetValue(QueryIconProperty, value); }

		static void OnTextPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static const wil::single_threaded_property<winrt::DependencyProperty> TextProperty;

		winrt::hstring Text() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextProperty)); }
		void Text(winrt::hstring const& value) const { SetValue(TextProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> SuggestedItemsSourceProperty;

		winrt::IInspectable SuggestedItemsSource() const { return GetValue(SuggestedItemsSourceProperty); }
		void SuggestedItemsSource(winrt::IInspectable const& value) const { SetValue(SuggestedItemsSourceProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> SuggestedItemTemplateProperty;

		winrt::DataTemplate SuggestedItemTemplate() const { return GetValue(SuggestedItemTemplateProperty).try_as<winrt::DataTemplate>(); }
		void SuggestedItemTemplate(winrt::DataTemplate const& value) const { SetValue(SuggestedItemTemplateProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> SuggestedItemTemplateSelectorProperty;

		winrt::DataTemplateSelector SuggestedItemTemplateSelector() const { return GetValue(SuggestedItemTemplateSelectorProperty).try_as<winrt::DataTemplateSelector>(); }
		void SuggestedItemTemplateSelector(winrt::DataTemplateSelector const& value) const { SetValue(SuggestedItemTemplateSelectorProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> SuggestedItemContainerStyleProperty;

		winrt::Style SuggestedItemContainerStyle() const { return GetValue(SuggestedItemContainerStyleProperty).try_as<winrt::Style>(); }
		void SuggestedItemContainerStyle(winrt::Style const& value) const { SetValue(SuggestedItemContainerStyleProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TabNavigateBackOnArrowProperty;

		bool TabNavigateBackOnArrow() const { return winrt::unbox_value<bool>(GetValue(TabNavigateBackOnArrowProperty)); }
		void TabNavigateBackOnArrow(bool value) const { SetValue(TabNavigateBackOnArrowProperty, winrt::box_value(value)); }

		static void OnMaximumTokensChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
		
		static const wil::single_threaded_property<winrt::DependencyProperty> MaximumTokensProperty;

		int32_t MaximumTokens() const { return winrt::unbox_value_or(GetValue(MaximumTokensProperty), 0); }
		void MaximumTokens(int32_t value) const { SetValue(MaximumTokensProperty, winrt::box_value(value)); }

		winrt::hstring SelectedTokenText() const
		{
			return PrepareSelectionForClipboard();
		}

		wil::typed_event<winrt::AutoSuggestBox, winrt::AutoSuggestBoxTextChangedEventArgs> TextChanged;

		wil::typed_event<winrt::AutoSuggestBox, winrt::AutoSuggestBoxSuggestionChosenEventArgs> SuggestionChosen;

		wil::typed_event<winrt::AutoSuggestBox, winrt::AutoSuggestBoxQuerySubmittedEventArgs> QuerySubmitted;

		winrt::event_token TokenItemAdding(winrt::TypedEventHandler<class_type, winrt::XamlToolkit::WinUI::Controls::TokenItemAddingEventArgs> const& handler);

		void TokenItemAdding(winrt::event_token const& token) noexcept;

		winrt::event_token TokenItemRemoving(winrt::TypedEventHandler<class_type, winrt::XamlToolkit::WinUI::Controls::TokenItemRemovingEventArgs> const& handler);

		void TokenItemRemoving(winrt::event_token const& token) noexcept;

		wil::typed_event<class_type, winrt::IInspectable> TokenItemAdded;

		wil::typed_event<class_type, winrt::IInspectable> TokenItemRemoved;

		void SelectAllTokensAndText();

		winrt::IAsyncAction AddTokenAsync(winrt::IInspectable data, std::optional<bool> atEnd = std::nullopt);

		bool SelectPreviousItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item);

		bool SelectNextItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item);

		void DeselectAllTokensAndText(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem = nullptr);

		winrt::IAsyncAction RemoveAllSelectedTokens();

	private:
		winrt::event<winrt::TypedEventHandler<class_type, TokenItemAddingEventArgs>> _tokenItemAdding;

		winrt::event<winrt::TypedEventHandler<class_type, TokenItemRemovingEventArgs>> _tokenItemRemoving;

		winrt::IAsyncAction TokenizingTextBox_CharacterReceived(winrt::UIElement const& sender, winrt::CharacterReceivedRoutedEventArgs const& args);

		winrt::IInspectable GetFocusedElement();

		void TokenizingTextBoxItem_GotFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void TokenizingTextBoxItem_LostFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		winrt::IAsyncOperation<bool> RemoveTokenAsync(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, winrt::IInspectable data);

		void GuardAgainstPlaceholderTextLayoutIssue();

		bool MoveFocusAndSelection(MoveDirection direction);

		winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem GetCurrentContainerItem();

		void ClearAllTextSelections(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem);

		bool SelectNewItem(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, int increment, std::function<bool(int)> testFunc);

		winrt::IAsyncAction TokenizingTextBoxItem_ClearAllAction(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, winrt::RoutedEventArgs const& args);

		winrt::IAsyncAction TokenizingTextBoxItem_ClearClicked(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, winrt::RoutedEventArgs const& args);

		void CopySelectedToClipboard();

		winrt::hstring PrepareSelectionForClipboard() const;

	private:
		winrt::Microsoft::UI::Dispatching::DispatcherQueue _dispatcherQueue{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::InterspersedObservableVector _innerItemsSource{ nullptr };
		ITokenStringContainer _currentTextEdit{ nullptr }; // Don't update this directly outside of initialization, use UpdateCurrentTextEdit Method - in future see https://github.com/dotnet/csharplang/issues/140#issuecomment-625012514
		ITokenStringContainer _lastTextEdit{ nullptr };
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBox : TokenizingTextBoxT<TokenizingTextBox, implementation::TokenizingTextBox>
	{
	};
}
