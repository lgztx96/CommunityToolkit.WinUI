#pragma once

#include "TokenizingTextBox.g.h"
#include <wil/common.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "InterspersedObservableVector.h"
#include <functional>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace Microsoft::UI::Input;
	using namespace Windows::UI::Core;
	using namespace Microsoft::UI::Dispatching;

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

		static bool IsShiftPressed()
		{
			const auto shiftState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Shift);
			return (static_cast<uint32_t>(shiftState) & static_cast<uint32_t>(CoreVirtualKeyStates::Down))
				== static_cast<uint32_t>(CoreVirtualKeyStates::Down);
		}

		static bool IsControlPressed()
		{
			const auto ctrlState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Control);
			return (static_cast<uint32_t>(ctrlState) & static_cast<uint32_t>(CoreVirtualKeyStates::Down)) 
				== static_cast<uint32_t>(CoreVirtualKeyStates::Down);
		}

		static inline wil::single_threaded_rw_property<bool> PauseTokenClearOnFocus;

		Microsoft::UI::Dispatching::DispatcherQueue _dispatcherQueue{ nullptr };
		Controls::InterspersedObservableVector _innerItemsSource{ nullptr };
		ITokenStringContainer _currentTextEdit{ nullptr }; // Don't update this directly outside of initialization, use UpdateCurrentTextEdit Method - in future see https://github.com/dotnet/csharplang/issues/140#issuecomment-625012514
		ITokenStringContainer _lastTextEdit{ nullptr };

	public:
		TokenizingTextBox();

		static inline bool IsClearingForClick;

		DependencyObject GetContainerForItemOverride();

		bool IsItemItsOwnContainerOverride(IInspectable const& item);

		void PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item);

		void ItemsSource_PropertyChanged(DependencyObject const& sender, DependencyProperty const& dp);

		void TokenizingTextBox_ItemClick(IInspectable const& sender, ItemClickEventArgs const& e);

		void TokenizingTextBox_PreviewKeyUp(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void OnTokenizingTextBoxPreviewKeyUp(VirtualKey key);

		void FocusPrimaryAutoSuggestBox();

		IAsyncAction TokenizingTextBox_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		IAsyncOperation<bool> OnTokenizingTextBoxPreviewKeyDown(VirtualKey key);

		void OnApplyTemplate();

		void RaiseQuerySubmitted(AutoSuggestBox const& sender, AutoSuggestBoxQuerySubmittedEventArgs const& args);

		void RaiseSuggestionChosen(AutoSuggestBox const& sender, AutoSuggestBoxSuggestionChosenEventArgs const& args);

		void RaiseTextChanged(AutoSuggestBox const& sender, AutoSuggestBoxTextChangedEventArgs const& args);

		void AddTokenItem(IInspectable& data, bool atEnd = false);

		IAsyncAction ClearAsync();

		void UpdateCurrentTextEdit(ITokenStringContainer const& edit);

		AutomationPeer OnCreateAutomationPeer();

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AutoSuggestBoxStyleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"AutoSuggestBoxStyle",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Style AutoSuggestBoxStyle() const { return GetValue(AutoSuggestBoxStyleProperty).try_as<winrt::Microsoft::UI::Xaml::Style>(); }
		void AutoSuggestBoxStyle(winrt::Microsoft::UI::Xaml::Style const& value) const { SetValue(AutoSuggestBoxStyleProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AutoSuggestBoxTextBoxStyleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"AutoSuggestBoxTextBoxStyle",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Style AutoSuggestBoxTextBoxStyle() const { return GetValue(AutoSuggestBoxTextBoxStyleProperty).try_as<winrt::Microsoft::UI::Xaml::Style>(); }
		void AutoSuggestBoxTextBoxStyle(winrt::Microsoft::UI::Xaml::Style const& value) const { SetValue(AutoSuggestBoxTextBoxStyleProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TextMemberPathProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TextMemberPath",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::hstring TextMemberPath() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextMemberPathProperty)); }
		void TextMemberPath(winrt::hstring const& value) const { SetValue(TextMemberPathProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TokenItemTemplateProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TokenItemTemplate",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::DataTemplate TokenItemTemplate() const { return GetValue(TokenItemTemplateProperty).try_as<winrt::Microsoft::UI::Xaml::DataTemplate>(); }
		void TokenItemTemplate(winrt::Microsoft::UI::Xaml::DataTemplate const& value) const { SetValue(TokenItemTemplateProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TokenItemTemplateSelectorProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TokenItemTemplateSelector",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector TokenItemTemplateSelector() const { return GetValue(TokenItemTemplateSelectorProperty).try_as<winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector>(); }
		void TokenItemTemplateSelector(winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector const& value) const { SetValue(TokenItemTemplateSelectorProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TokenDelimiterProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TokenDelimiter",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L" ") });

		winrt::hstring TokenDelimiter() const { return winrt::unbox_value<winrt::hstring>(GetValue(TokenDelimiterProperty)); }
		void TokenDelimiter(winrt::hstring const& value) const { SetValue(TokenDelimiterProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TokenSpacingProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TokenSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		double TokenSpacing() const { return winrt::unbox_value_or(GetValue(TokenSpacingProperty), 0.0); }
		void TokenSpacing(double value) const { SetValue(TokenSpacingProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> PlaceholderTextProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"PlaceholderText",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L"") });

		winrt::hstring PlaceholderText() const { return winrt::unbox_value<winrt::hstring>(GetValue(PlaceholderTextProperty)); }
		void PlaceholderText(winrt::hstring const& value) const { SetValue(PlaceholderTextProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> QueryIconProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"QueryIcon",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconSource>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Controls::IconSource QueryIcon() const { return GetValue(QueryIconProperty).try_as<winrt::Microsoft::UI::Xaml::Controls::IconSource>(); }
		void QueryIcon(winrt::Microsoft::UI::Xaml::Controls::IconSource const& value) const { SetValue(QueryIconProperty, value); }

		static void OnTextPropertyChanged(winrt::Microsoft::UI::Xaml::DependencyObject const& d, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TextProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Text",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L""), &TokenizingTextBox::OnTextPropertyChanged });

		winrt::hstring Text() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextProperty)); }
		void Text(winrt::hstring const& value) const { SetValue(TextProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SuggestedItemsSourceProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"SuggestedItemsSource",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Windows::Foundation::IInspectable SuggestedItemsSource() const { return GetValue(SuggestedItemsSourceProperty); }
		void SuggestedItemsSource(winrt::Windows::Foundation::IInspectable const& value) const { SetValue(SuggestedItemsSourceProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SuggestedItemTemplateProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"SuggestedItemTemplate",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::DataTemplate SuggestedItemTemplate() const { return GetValue(SuggestedItemTemplateProperty).try_as<winrt::Microsoft::UI::Xaml::DataTemplate>(); }
		void SuggestedItemTemplate(winrt::Microsoft::UI::Xaml::DataTemplate const& value) const { SetValue(SuggestedItemTemplateProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SuggestedItemTemplateSelectorProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"SuggestedItemTemplateSelector",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector SuggestedItemTemplateSelector() const { return GetValue(SuggestedItemTemplateSelectorProperty).try_as<winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector>(); }
		void SuggestedItemTemplateSelector(winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector const& value) const { SetValue(SuggestedItemTemplateSelectorProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SuggestedItemContainerStyleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"SuggestedItemContainerStyle",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Style SuggestedItemContainerStyle() const { return GetValue(SuggestedItemContainerStyleProperty).try_as<winrt::Microsoft::UI::Xaml::Style>(); }
		void SuggestedItemContainerStyle(winrt::Microsoft::UI::Xaml::Style const& value) const { SetValue(SuggestedItemContainerStyleProperty, value); }

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TabNavigateBackOnArrowProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TabNavigateBackOnArrow",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(false) });

		bool TabNavigateBackOnArrow() const { return winrt::unbox_value<bool>(GetValue(TabNavigateBackOnArrowProperty)); }
		void TabNavigateBackOnArrow(bool value) const { SetValue(TabNavigateBackOnArrowProperty, winrt::box_value(value)); }

		static void OnMaximumTokensChanged(winrt::Microsoft::UI::Xaml::DependencyObject const& d, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
		
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MaximumTokensProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"MaximumTokens",
				winrt::xaml_typename<int32_t>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &TokenizingTextBox::OnMaximumTokensChanged });

		int32_t MaximumTokens() const { return winrt::unbox_value_or(GetValue(MaximumTokensProperty), 0); }
		void MaximumTokens(int32_t value) const { SetValue(MaximumTokensProperty, winrt::box_value(value)); }

		winrt::hstring SelectedTokenText() const
		{
			return PrepareSelectionForClipboard();
		}

		wil::typed_event<AutoSuggestBox, AutoSuggestBoxTextChangedEventArgs> TextChanged;

		wil::typed_event<AutoSuggestBox, AutoSuggestBoxSuggestionChosenEventArgs> SuggestionChosen;

		wil::typed_event<AutoSuggestBox, AutoSuggestBoxQuerySubmittedEventArgs> QuerySubmitted;

		wil::typed_event<Controls::TokenizingTextBox, TokenItemAddingEventArgs> TokenItemAdding;

		wil::typed_event<Controls::TokenizingTextBox, IInspectable> TokenItemAdded;

		wil::typed_event<Controls::TokenizingTextBox, TokenItemRemovingEventArgs> TokenItemRemoving;

		wil::typed_event<Controls::TokenizingTextBox, IInspectable> TokenItemRemoved;

		void SelectAllTokensAndText() const;

		IAsyncAction AddTokenAsync(IInspectable data, std::optional<bool> atEnd = std::nullopt);

		bool SelectPreviousItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item);

		bool SelectNextItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item);

		void DeselectAllTokensAndText(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem = nullptr);

		IAsyncAction RemoveAllSelectedTokens();

	private:
		IAsyncAction TokenizingTextBox_CharacterReceived(UIElement const& sender, CharacterReceivedRoutedEventArgs const& args);

		IInspectable GetFocusedElement();

		void TokenizingTextBoxItem_GotFocus(IInspectable const& sender, RoutedEventArgs const& e);

		void TokenizingTextBoxItem_LostFocus(IInspectable const& sender, RoutedEventArgs const& e);

		IAsyncOperation<bool> RemoveTokenAsync(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, IInspectable data);

		void GuardAgainstPlaceholderTextLayoutIssue();

		bool MoveFocusAndSelection(MoveDirection direction);

		winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem GetCurrentContainerItem();

		void ClearAllTextSelections(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& ignoreItem);

		bool SelectNewItem(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& item, int increment, std::function<bool(int)> testFunc);

		IAsyncAction TokenizingTextBoxItem_ClearAllAction(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, RoutedEventArgs const& args);

		IAsyncAction TokenizingTextBoxItem_ClearClicked(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& sender, RoutedEventArgs const& args);

		void CopySelectedToClipboard();

		winrt::hstring PrepareSelectionForClipboard() const;
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBox : TokenizingTextBoxT<TokenizingTextBox, implementation::TokenizingTextBox>
	{
	};
}
