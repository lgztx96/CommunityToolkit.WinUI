#pragma once

#include "RichSuggestBox.g.h"
#include "RichSuggestQuery.h"
#include "RichSuggestToken.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <functional>
#include <memory>
#include <mutex>
#include <string_view>
#include <unordered_map>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Text;
	using namespace Microsoft::UI::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RichSuggestBox : RichSuggestBoxT<RichSuggestBox>
	{
	private:
		static constexpr std::wstring_view PartRichEditBox = L"RichEditBox";
		static constexpr std::wstring_view PartSuggestionsPopup = L"SuggestionsPopup";
		static constexpr std::wstring_view PartSuggestionsList = L"SuggestionsList";
		static constexpr std::wstring_view PartSuggestionsContainer = L"SuggestionsContainer";
		static constexpr std::wstring_view PartHeaderContentPresenter = L"HeaderContentPresenter";
		static constexpr std::wstring_view PartDescriptionPresenter = L"DescriptionPresenter";

		std::recursive_mutex _tokensLock;
		std::unordered_map<winrt::hstring, winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> _tokens;
		winrt::Windows::Foundation::Collections::IObservableVector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> _visibleTokens;

		Popup _suggestionPopup{ nullptr };
		RichEditBox _richEditBox{ nullptr };
		ScrollViewer _scrollViewer{ nullptr };
		ListViewBase _suggestionsList{ nullptr };
		Border _suggestionsContainer{ nullptr };

		int _suggestionChoice;
		bool _ignoreChange;
		bool _popupOpenDown;
		bool _textCompositionActive;
		std::shared_ptr<RichSuggestQuery> _currentQuery;

		RichEditBox::SizeChanged_revoker _editBoxSizeChangedRevoker;
		RichEditBox::TextChanging_revoker _editBoxTextChangingRevoker;
		RichEditBox::TextChanged_revoker _editBoxTextChangedRevoker;
		RichEditBox::TextCompositionStarted_revoker _editBoxTextCompositionStartedRevoker;
		RichEditBox::TextCompositionChanged_revoker _editBoxTextCompositionChangedRevoker;
		RichEditBox::TextCompositionEnded_revoker _editBoxTextCompositionEndedRevoker;
		RichEditBox::SelectionChanging_revoker _editBoxSelectionChangingRevoker;
		RichEditBox::SelectionChanged_revoker _editBoxSelectionChangedRevoker;
		RichEditBox::Paste_revoker _editBoxPasteRevoker;
		RichEditBox::PreviewKeyDown_revoker _editBoxPreviewKeyDownRevoker;

		RichEditBox::ProcessKeyboardAccelerators_revoker _editBoxProcessKeyboardAcceleratorsRevoker;

		ListViewBase::ItemClick_revoker _listItemClickRevoker;
		ListViewBase::SizeChanged_revoker _listSizeChangedRevoker;
		ListViewBase::GotFocus_revoker _listGotFocusRevoker;

		IInspectable _pointerPressedHandler{ nullptr };
		IInspectable _pointerMovedHandler{ nullptr };

		static void OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnDescriptionChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnSuggestionPopupPlacementChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnPrefixesChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:
		RichSuggestBox();

		void ClearUndoRedoSuggestionHistory();

		void Clear();

		void AddTokens(IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens);

		void Load(winrt::hstring const& rtf, IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens);

		bool TryGetTokenFromRange(ITextRange range, winrt::XamlToolkit::WinUI::Controls::RichSuggestToken& token);

		Rect GetRectFromRange(ITextRange const& range);

		void OnApplyTemplate();

#pragma region Events
		winrt::event<winrt::Windows::Foundation::TypedEventHandler<class_type, SuggestionRequestedEventArgs>> _suggestionRequested;

		winrt::event_token SuggestionRequested(
			winrt::Windows::Foundation::TypedEventHandler<class_type, SuggestionRequestedEventArgs> const& handler)
		{
			return _suggestionRequested.add(handler);
		}

		void SuggestionRequested(winrt::event_token const& token)
		{
			_suggestionRequested.remove(token);
		}

		winrt::event<winrt::Windows::Foundation::TypedEventHandler<class_type, SuggestionChosenEventArgs>> _suggestionChosen;

		winrt::event_token SuggestionChosen(
			winrt::Windows::Foundation::TypedEventHandler<class_type, SuggestionChosenEventArgs> const& handler)
		{
			return _suggestionChosen.add(handler);
		}

		void SuggestionChosen(winrt::event_token const& token)
		{
			_suggestionChosen.remove(token);
		}

		winrt::event<winrt::Windows::Foundation::TypedEventHandler<class_type, RichSuggestTokenSelectedEventArgs>> _tokenSelected;

		winrt::event_token TokenSelected(
			winrt::Windows::Foundation::TypedEventHandler<class_type, RichSuggestTokenSelectedEventArgs> const& handler)
		{
			return _tokenSelected.add(handler);
		}

		void TokenSelected(winrt::event_token const& token)
		{
			_tokenSelected.remove(token);
		}

		winrt::event<winrt::Windows::Foundation::TypedEventHandler<class_type, RichSuggestTokenPointerOverEventArgs>> _tokenPointerOver;

		winrt::event_token TokenPointerOver(
			winrt::Windows::Foundation::TypedEventHandler<class_type, RichSuggestTokenPointerOverEventArgs> const& handler)
		{
			return _tokenPointerOver.add(handler);
		}

		void TokenPointerOver(winrt::event_token const& token)
		{
			_tokenPointerOver.remove(token);
		}

		wil::typed_event<class_type, RoutedEventArgs> TextChanged;

		wil::typed_event<class_type, RoutedEventArgs> SelectionChanged;

		wil::typed_event<class_type, TextControlPasteEventArgs> Paste;
#pragma endregion

#pragma region Properties
		static inline const wil::single_threaded_property<DependencyProperty> PlaceholderTextProperty =
			DependencyProperty::Register(
				L"PlaceholderText",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(L"") });

		static inline const wil::single_threaded_property<DependencyProperty> RichEditBoxStyleProperty =
			DependencyProperty::Register(
				L"RichEditBoxStyle",
				winrt::xaml_typename<struct Style>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> HeaderProperty =
			DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<IInspectable>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr, &RichSuggestBox::OnHeaderChanged });

		static inline const wil::single_threaded_property<DependencyProperty> HeaderTemplateProperty =
			DependencyProperty::Register(
				L"HeaderTemplate",
				winrt::xaml_typename<DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> DescriptionProperty =
			DependencyProperty::Register(
				L"Description",
				winrt::xaml_typename<IInspectable>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr, &RichSuggestBox::OnDescriptionChanged });

		static inline const wil::single_threaded_property<DependencyProperty> PopupPlacementProperty =
			DependencyProperty::Register(
				L"PopupPlacement",
				winrt::xaml_typename<SuggestionPopupPlacementMode>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(SuggestionPopupPlacementMode::Floating), &RichSuggestBox::OnSuggestionPopupPlacementChanged });

		static inline const wil::single_threaded_property<DependencyProperty> PopupCornerRadiusProperty =
			DependencyProperty::Register(
				L"PopupCornerRadius",
				winrt::xaml_typename<struct CornerRadius>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::CornerRadius{ 0, 0, 0, 0}) });

		static inline const wil::single_threaded_property<DependencyProperty> PopupHeaderProperty =
			DependencyProperty::Register(
				L"PopupHeader",
				winrt::xaml_typename<IInspectable>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> PopupHeaderTemplateProperty =
			DependencyProperty::Register(
				L"PopupHeaderTemplate",
				winrt::xaml_typename<DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> PopupFooterProperty =
			DependencyProperty::Register(
				L"PopupFooter",
				winrt::xaml_typename<IInspectable>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> PopupFooterTemplateProperty =
			DependencyProperty::Register(
				L"PopupFooterTemplate",
				winrt::xaml_typename<DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> TokenBackgroundProperty =
			DependencyProperty::Register(
				L"TokenBackground",
				winrt::xaml_typename<SolidColorBrush>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> TokenForegroundProperty =
			DependencyProperty::Register(
				L"TokenForeground",
				winrt::xaml_typename<SolidColorBrush>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<DependencyProperty> PrefixesProperty =
			DependencyProperty::Register(
				L"Prefixes",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(L""), &RichSuggestBox::OnPrefixesChanged });

		static inline const wil::single_threaded_property<DependencyProperty> ClipboardPasteFormatProperty =
			DependencyProperty::Register(
				L"ClipboardPasteFormat",
				winrt::xaml_typename<RichEditClipboardFormat>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(RichEditClipboardFormat::AllFormats) });

		static inline const wil::single_threaded_property<DependencyProperty> ClipboardCopyFormatProperty =
			DependencyProperty::Register(
				L"ClipboardCopyFormat",
				winrt::xaml_typename<RichEditClipboardFormat>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(RichEditClipboardFormat::AllFormats) });

		static inline const wil::single_threaded_property<DependencyProperty> DisabledFormattingAcceleratorsProperty =
			DependencyProperty::Register(
				L"DisabledFormattingAccelerators",
				winrt::xaml_typename<DisabledFormattingAccelerators>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(DisabledFormattingAccelerators::None) });

		hstring PlaceholderText() const { return winrt::unbox_value<hstring>(GetValue(PlaceholderTextProperty)); }
		void PlaceholderText(hstring const& value) { SetValue(PlaceholderTextProperty, winrt::box_value(value)); }

		winrt::Microsoft::UI::Xaml::Style RichEditBoxStyle() const { return winrt::unbox_value<struct Style>(GetValue(RichEditBoxStyleProperty)); }
		void RichEditBoxStyle(winrt::Microsoft::UI::Xaml::Style const& value) { SetValue(RichEditBoxStyleProperty, value); }

		IInspectable Header() const { return GetValue(HeaderProperty); }
		void Header(IInspectable const& value) { SetValue(HeaderProperty, value); }

		DataTemplate HeaderTemplate() const { return winrt::unbox_value<DataTemplate>(GetValue(HeaderTemplateProperty)); }
		void HeaderTemplate(DataTemplate const& value) { SetValue(HeaderTemplateProperty, value); }

		IInspectable Description() const { return GetValue(DescriptionProperty); }
		void Description(IInspectable const& value) { SetValue(DescriptionProperty, value); }

		SuggestionPopupPlacementMode PopupPlacement() const { return winrt::unbox_value<SuggestionPopupPlacementMode>(GetValue(PopupPlacementProperty)); }
		void PopupPlacement(SuggestionPopupPlacementMode const& value) { SetValue(PopupPlacementProperty, winrt::box_value(value)); }

		winrt::Microsoft::UI::Xaml::CornerRadius PopupCornerRadius() const { return winrt::unbox_value<struct CornerRadius>(GetValue(PopupCornerRadiusProperty)); }
		void PopupCornerRadius(winrt::Microsoft::UI::Xaml::CornerRadius const& value) { SetValue(PopupCornerRadiusProperty, winrt::box_value(value)); }

		IInspectable PopupHeader() const
		{
			return GetValue(PopupHeaderProperty);
		}

		void PopupHeader(IInspectable const& value)
		{
			SetValue(PopupHeaderProperty, value);
		}

		DataTemplate PopupHeaderTemplate() const
		{
			return GetValue(PopupHeaderTemplateProperty).try_as<DataTemplate>();
		}

		void PopupHeaderTemplate(DataTemplate const& value)
		{
			SetValue(PopupHeaderTemplateProperty, value);
		}

		IInspectable PopupFooter() const
		{
			return GetValue(PopupFooterProperty);
		}

		void PopupFooter(IInspectable const& value)
		{
			SetValue(PopupFooterProperty, value);
		}

		DataTemplate PopupFooterTemplate() const
		{
			return GetValue(PopupFooterTemplateProperty).try_as<DataTemplate>();
		}

		void PopupFooterTemplate(DataTemplate const& value)
		{
			SetValue(PopupFooterTemplateProperty, value);
		}

		SolidColorBrush TokenBackground() const
		{
			return GetValue(TokenBackgroundProperty).try_as<SolidColorBrush>();
		}

		void TokenBackground(SolidColorBrush const& value)
		{
			SetValue(TokenBackgroundProperty, value);
		}

		SolidColorBrush TokenForeground() const
		{
			return GetValue(TokenForegroundProperty).try_as<SolidColorBrush>();
		}

		void TokenForeground(SolidColorBrush const& value)
		{
			SetValue(TokenForegroundProperty, value);
		}

		hstring Prefixes() const
		{
			return winrt::unbox_value<winrt::hstring>(GetValue(PrefixesProperty));
		}

		void Prefixes(hstring const& value)
		{
			SetValue(PrefixesProperty, winrt::box_value(value));
		}

		RichEditClipboardFormat ClipboardPasteFormat() const
		{
			return unbox_value<RichEditClipboardFormat>(GetValue(ClipboardPasteFormatProperty));
		}

		void ClipboardPasteFormat(RichEditClipboardFormat const& value)
		{
			SetValue(ClipboardPasteFormatProperty, winrt::box_value(value));
		}

		RichEditClipboardFormat ClipboardCopyFormat() const
		{
			return unbox_value<RichEditClipboardFormat>(GetValue(ClipboardCopyFormatProperty));
		}

		void ClipboardCopyFormat(RichEditClipboardFormat const& value)
		{
			SetValue(ClipboardCopyFormatProperty, winrt::box_value(value));
		}

		winrt::Microsoft::UI::Xaml::Controls::DisabledFormattingAccelerators DisabledFormattingAccelerators() const
		{
			return winrt::unbox_value<enum DisabledFormattingAccelerators>(GetValue(DisabledFormattingAcceleratorsProperty));
		}

		void DisabledFormattingAccelerators(winrt::Microsoft::UI::Xaml::Controls::DisabledFormattingAccelerators const& value)
		{
			SetValue(DisabledFormattingAcceleratorsProperty, winrt::box_value(value));
		}

		RichEditTextDocument TextDocument() { return _richEditBox ? _richEditBox.TextDocument() : nullptr; }

		double HorizontalOffset() { return _scrollViewer ? _scrollViewer.HorizontalOffset() : 0; }

		double VerticalOffset() { return _scrollViewer ? _scrollViewer.VerticalOffset() : 0; }

		IObservableVector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> Tokens() { return _visibleTokens; }
#pragma endregion

	private:

		void OnCornerRadiusChanged(DependencyObject const& sender, DependencyProperty const& dp);

		void OnLoaded(IInspectable const& sender, RoutedEventArgs const& e);

		void OnLostFocusEvent(IInspectable const& sender, RoutedEventArgs const& e);

		winrt::fire_and_forget SuggestionsList_ItemClick(IInspectable const& sender, ItemClickEventArgs const& e);

		void SuggestionsList_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void SuggestionList_GotFocus(IInspectable const& sender, RoutedEventArgs const& e);

		void RichEditBox_OnPointerMoved(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void RichEditBox_SelectionChanging(RichEditBox const& sender, RichEditBoxSelectionChangingEventArgs const& args);

		winrt::fire_and_forget RichEditBox_SelectionChanged(IInspectable const& sender, RoutedEventArgs const& e);

		void RichEditBox_OnPointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);

		winrt::fire_and_forget RichEditBox_ProcessKeyboardAccelerators(UIElement const& sender, ProcessKeyboardAcceleratorEventArgs const& args);

		winrt::fire_and_forget RichEditBox_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void RichEditBox_TextChanging(RichEditBox const& sender, RichEditBoxTextChangingEventArgs const& args);

		void RichEditBox_TextChanged(IInspectable const& sender, RoutedEventArgs const& e);

		void RichEditBox_TextCompositionStarted(RichEditBox const& sender, TextCompositionStartedEventArgs const& args);

		winrt::fire_and_forget RichEditBox_TextCompositionChanged(RichEditBox const& sender, TextCompositionChangedEventArgs const& args);

		void RichEditBox_TextCompositionEnded(RichEditBox const& sender, TextCompositionEndedEventArgs const& args);

		void RichEditBox_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		winrt::fire_and_forget RichEditBox_Paste(IInspectable const& sender, TextControlPasteEventArgs const& e);

		void ConditionallyLoadElement(IInspectable const& property, std::wstring_view elementName);

		void UpdateVisibleTokenList();

#pragma region Helpers
		static bool IsElementOnScreen(FrameworkElement const& element, double offsetX = 0, double offsetY = 0);

		static bool IsElementInsideWindow(FrameworkElement const& element, double offsetX = 0, double offsetY = 0);

		static winrt::hstring EnforcePrefixesRequirements(winrt::hstring const& value);

		static void PadRange(ITextRange const& range, ITextCharacterFormat const& format);

		static void ForEachLinkInDocument(RichEditTextDocument const& document, std::function<void(ITextRange const&)> const& action);
#pragma endregion

#pragma region Document
		void CreateSingleEdit(std::function<void()> const& editAction);

		void ExpandSelectionOnPartialTokenSelect(ITextSelection const& selection, ITextRange const& tokenRange);

		void InvokeTokenSelected(ITextSelection const& selection);

		void InvokeTokenPointerOver(PointerPoint const& pointer);

		void ValidateTokensInDocument();

		void ValidateTokenFromRange(ITextRange const& range);

		bool TryCommitSuggestionIntoDocument(ITextRange const& range, winrt::hstring const& displayText, winrt::guid const& id, ITextCharacterFormat const& format, bool addTrailingSpace);

		bool TryExtractQueryFromSelection(winrt::hstring& prefix, winrt::hstring& query, ITextRange& range);

		bool TryExtractQueryFromRange(ITextRange const& range, winrt::hstring& prefix, winrt::hstring& query);

		ITextCharacterFormat CreateTokenFormat(ITextRange const& range) const;
#pragma endregion

#pragma region Suggestion
		IAsyncAction RequestSuggestionsAsync(ITextRange& range);

		IAsyncAction CommitSuggestionAsync(IInspectable const& selectedItem);

		void UpdateSuggestionsListSelectedItem(int choice);

		void ShowSuggestionsPopup(bool show);

		void UpdatePopupWidth();

		void UpdatePopupOffset();

		void UpdateCornerRadii();
#pragma endregion
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct RichSuggestBox : RichSuggestBoxT<RichSuggestBox, implementation::RichSuggestBox>
	{
	};
}
