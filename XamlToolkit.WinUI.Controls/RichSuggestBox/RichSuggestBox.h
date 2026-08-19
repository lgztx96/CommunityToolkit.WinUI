#pragma once

#include "RichSuggestBox.g.h"
#include "RichSuggestQuery.h"
#include "RichSuggestToken.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
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
	using namespace Windows::ApplicationModel::DataTransfer;
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
		winrt::IObservableVector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> _visibleTokens;

		winrt::Popup _suggestionPopup{ nullptr };
		winrt::RichEditBox _richEditBox{ nullptr };
		winrt::ScrollViewer _scrollViewer{ nullptr };
		winrt::ListViewBase _suggestionsList{ nullptr };
		winrt::Border _suggestionsContainer{ nullptr };

		int _suggestionChoice{ false };
		bool _ignoreChange{ false };
		bool _popupOpenDown{ false };
		bool _textCompositionActive{ false };
		std::shared_ptr<RichSuggestQuery> _currentQuery;

		winrt::RichEditBox::SizeChanged_revoker _editBoxSizeChangedRevoker;
		winrt::RichEditBox::TextChanging_revoker _editBoxTextChangingRevoker;
		winrt::RichEditBox::TextChanged_revoker _editBoxTextChangedRevoker;
		winrt::RichEditBox::TextCompositionStarted_revoker _editBoxTextCompositionStartedRevoker;
		winrt::RichEditBox::TextCompositionChanged_revoker _editBoxTextCompositionChangedRevoker;
		winrt::RichEditBox::TextCompositionEnded_revoker _editBoxTextCompositionEndedRevoker;
		winrt::RichEditBox::SelectionChanging_revoker _editBoxSelectionChangingRevoker;
		winrt::RichEditBox::SelectionChanged_revoker _editBoxSelectionChangedRevoker;
		winrt::RichEditBox::Paste_revoker _editBoxPasteRevoker;
		winrt::RichEditBox::PreviewKeyDown_revoker _editBoxPreviewKeyDownRevoker;

		winrt::RichEditBox::ProcessKeyboardAccelerators_revoker _editBoxProcessKeyboardAcceleratorsRevoker;

		winrt::ListViewBase::ItemClick_revoker _listItemClickRevoker;
		winrt::ListViewBase::SizeChanged_revoker _listSizeChangedRevoker;
		winrt::ListViewBase::GotFocus_revoker _listGotFocusRevoker;

		winrt::IInspectable _pointerPressedHandler{ nullptr };
		winrt::IInspectable _pointerMovedHandler{ nullptr };

		static void OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnDescriptionChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnSuggestionPopupPlacementChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnPrefixesChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

	public:
		RichSuggestBox();

		void ClearUndoRedoSuggestionHistory();

		void Clear();

		void AddTokens(winrt::IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens);

		void Load(winrt::hstring const& rtf, winrt::IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens);

		bool TryGetTokenFromRange(winrt::ITextRange const& range, winrt::XamlToolkit::WinUI::Controls::RichSuggestToken& token);

		winrt::Rect GetRectFromRange(winrt::ITextRange const& range);

		void OnApplyTemplate();

#pragma region Events
		winrt::event<winrt::TypedEventHandler<class_type, SuggestionRequestedEventArgs>> _suggestionRequested;

		winrt::event_token SuggestionRequested(winrt::TypedEventHandler<class_type, SuggestionRequestedEventArgs> const& handler)
		{
			return _suggestionRequested.add(handler);
		}

		void SuggestionRequested(winrt::event_token const& token)
		{
			_suggestionRequested.remove(token);
		}

		winrt::event<winrt::TypedEventHandler<class_type, SuggestionChosenEventArgs>> _suggestionChosen;

		winrt::event_token SuggestionChosen(winrt::TypedEventHandler<class_type, SuggestionChosenEventArgs> const& handler)
		{
			return _suggestionChosen.add(handler);
		}

		void SuggestionChosen(winrt::event_token const& token)
		{
			_suggestionChosen.remove(token);
		}

		winrt::event<winrt::TypedEventHandler<class_type, RichSuggestTokenSelectedEventArgs>> _tokenSelected;

		winrt::event_token TokenSelected(winrt::TypedEventHandler<class_type, RichSuggestTokenSelectedEventArgs> const& handler)
		{
			return _tokenSelected.add(handler);
		}

		void TokenSelected(winrt::event_token const& token)
		{
			_tokenSelected.remove(token);
		}

		winrt::event<winrt::TypedEventHandler<class_type, RichSuggestTokenPointerOverEventArgs>> _tokenPointerOver;

		winrt::event_token TokenPointerOver(winrt::TypedEventHandler<class_type, RichSuggestTokenPointerOverEventArgs> const& handler)
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
		static inline const wil::single_threaded_property<winrt::DependencyProperty> PlaceholderTextProperty =
			winrt::DependencyProperty::Register(
				L"PlaceholderText",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(L"") });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> RichEditBoxStyleProperty =
			winrt::DependencyProperty::Register(
				L"RichEditBoxStyle",
				winrt::xaml_typename<winrt::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty =
			winrt::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &RichSuggestBox::OnHeaderChanged });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderTemplateProperty =
			winrt::DependencyProperty::Register(
				L"HeaderTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> DescriptionProperty =
			winrt::DependencyProperty::Register(
				L"Description",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &RichSuggestBox::OnDescriptionChanged });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupPlacementProperty =
			winrt::DependencyProperty::Register(
				L"PopupPlacement",
				winrt::xaml_typename<SuggestionPopupPlacementMode>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(SuggestionPopupPlacementMode::Floating), &RichSuggestBox::OnSuggestionPopupPlacementChanged });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupCornerRadiusProperty =
			winrt::DependencyProperty::Register(
				L"PopupCornerRadius",
				winrt::xaml_typename<winrt::CornerRadius>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::CornerRadius{ 0, 0, 0, 0}) });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupHeaderProperty =
			winrt::DependencyProperty::Register(
				L"PopupHeader",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupHeaderTemplateProperty =
			winrt::DependencyProperty::Register(
				L"PopupHeaderTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupFooterProperty =
			winrt::DependencyProperty::Register(
				L"PopupFooter",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PopupFooterTemplateProperty =
			winrt::DependencyProperty::Register(
				L"PopupFooterTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> TokenBackgroundProperty =
			winrt::DependencyProperty::Register(
				L"TokenBackground",
				winrt::xaml_typename<winrt::SolidColorBrush>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> TokenForegroundProperty =
			winrt::DependencyProperty::Register(
				L"TokenForeground",
				winrt::xaml_typename<winrt::SolidColorBrush>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PrefixesProperty =
			winrt::DependencyProperty::Register(
				L"Prefixes",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(L""), &RichSuggestBox::OnPrefixesChanged });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ClipboardPasteFormatProperty =
			winrt::DependencyProperty::Register(
				L"ClipboardPasteFormat",
				winrt::xaml_typename<winrt::RichEditClipboardFormat>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(RichEditClipboardFormat::AllFormats) });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ClipboardCopyFormatProperty =
			winrt::DependencyProperty::Register(
				L"ClipboardCopyFormat",
				winrt::xaml_typename<winrt::RichEditClipboardFormat>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(RichEditClipboardFormat::AllFormats) });

		static inline const wil::single_threaded_property<winrt::DependencyProperty> DisabledFormattingAcceleratorsProperty =
			winrt::DependencyProperty::Register(
				L"DisabledFormattingAccelerators",
				winrt::xaml_typename<DisabledFormattingAccelerators>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(DisabledFormattingAccelerators::None) });

		winrt::hstring PlaceholderText() const { return winrt::unbox_value<winrt::hstring>(GetValue(PlaceholderTextProperty())); }
		void PlaceholderText(winrt::hstring const& value) { SetValue(PlaceholderTextProperty(), winrt::box_value(value)); }

		winrt::Style RichEditBoxStyle() const { return winrt::unbox_value<struct Style>(GetValue(RichEditBoxStyleProperty())); }
		void RichEditBoxStyle(winrt::Style const& value) { SetValue(RichEditBoxStyleProperty(), value); }

		winrt::IInspectable Header() const { return GetValue(HeaderProperty()); }
		void Header(winrt::IInspectable const& value) { SetValue(HeaderProperty(), value); }

		winrt::DataTemplate HeaderTemplate() const { return winrt::unbox_value<winrt::DataTemplate>(GetValue(HeaderTemplateProperty())); }
		void HeaderTemplate(winrt::DataTemplate const& value) { SetValue(HeaderTemplateProperty(), value); }

		winrt::IInspectable Description() const { return GetValue(DescriptionProperty()); }
		void Description(winrt::IInspectable const& value) { SetValue(DescriptionProperty(), value); }

		SuggestionPopupPlacementMode PopupPlacement() const { return winrt::unbox_value<SuggestionPopupPlacementMode>(GetValue(PopupPlacementProperty())); }
		void PopupPlacement(SuggestionPopupPlacementMode const& value) { SetValue(PopupPlacementProperty(), winrt::box_value(value)); }

		winrt::Microsoft::UI::Xaml::CornerRadius PopupCornerRadius() const { return winrt::unbox_value<struct CornerRadius>(GetValue(PopupCornerRadiusProperty())); }
		void PopupCornerRadius(winrt::Microsoft::UI::Xaml::CornerRadius const& value) { SetValue(PopupCornerRadiusProperty(), winrt::box_value(value)); }

		winrt::IInspectable PopupHeader() const
		{
			return GetValue(PopupHeaderProperty());
		}

		void PopupHeader(winrt::IInspectable const& value)
		{
			SetValue(PopupHeaderProperty(), value);
		}

		winrt::DataTemplate PopupHeaderTemplate() const
		{
			return GetValue(PopupHeaderTemplateProperty()).try_as<winrt::DataTemplate>();
		}

		void PopupHeaderTemplate(winrt::DataTemplate const& value)
		{
			SetValue(PopupHeaderTemplateProperty(), value);
		}

		winrt::IInspectable PopupFooter() const
		{
			return GetValue(PopupFooterProperty());
		}

		void PopupFooter(winrt::IInspectable const& value)
		{
			SetValue(PopupFooterProperty(), value);
		}

		winrt::DataTemplate PopupFooterTemplate() const
		{
			return GetValue(PopupFooterTemplateProperty()).try_as<winrt::DataTemplate>();
		}

		void PopupFooterTemplate(winrt::DataTemplate const& value)
		{
			SetValue(PopupFooterTemplateProperty(), value);
		}

		winrt::SolidColorBrush TokenBackground() const
		{
			return GetValue(TokenBackgroundProperty()).try_as<winrt::SolidColorBrush>();
		}

		void TokenBackground(winrt::SolidColorBrush const& value)
		{
			SetValue(TokenBackgroundProperty(), value);
		}

		winrt::SolidColorBrush TokenForeground() const
		{
			return GetValue(TokenForegroundProperty()).try_as<winrt::SolidColorBrush>();
		}

		void TokenForeground(winrt::SolidColorBrush const& value)
		{
			SetValue(TokenForegroundProperty(), value);
		}

		winrt::hstring Prefixes() const
		{
			return winrt::unbox_value<winrt::hstring>(GetValue(PrefixesProperty()));
		}

		void Prefixes(winrt::hstring const& value)
		{
			SetValue(PrefixesProperty(), winrt::box_value(value));
		}

		winrt::RichEditClipboardFormat ClipboardPasteFormat() const
		{
			return winrt::unbox_value<winrt::RichEditClipboardFormat>(GetValue(ClipboardPasteFormatProperty()));
		}

		void ClipboardPasteFormat(winrt::RichEditClipboardFormat const& value)
		{
			SetValue(ClipboardPasteFormatProperty(), winrt::box_value(value));
		}

		winrt::RichEditClipboardFormat ClipboardCopyFormat() const
		{
			return winrt::unbox_value<winrt::RichEditClipboardFormat>(GetValue(ClipboardCopyFormatProperty()));
		}

		void ClipboardCopyFormat(winrt::RichEditClipboardFormat const& value)
		{
			SetValue(ClipboardCopyFormatProperty(), winrt::box_value(value));
		}

		winrt::DisabledFormattingAccelerators DisabledFormattingAccelerators() const
		{
			return winrt::unbox_value<enum DisabledFormattingAccelerators>(GetValue(DisabledFormattingAcceleratorsProperty()));
		}

		void DisabledFormattingAccelerators(winrt::DisabledFormattingAccelerators const& value)
		{
			SetValue(DisabledFormattingAcceleratorsProperty(), winrt::box_value(value));
		}

		winrt::RichEditTextDocument TextDocument() const { return _richEditBox ? _richEditBox.TextDocument() : nullptr; }

		double HorizontalOffset() const { return _scrollViewer ? _scrollViewer.HorizontalOffset() : 0; }

		double VerticalOffset() const { return _scrollViewer ? _scrollViewer.VerticalOffset() : 0; }

		winrt::IObservableVector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> Tokens() const { return _visibleTokens; }
#pragma endregion

	private:

		void OnCornerRadiusChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		void OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnLostFocusEvent(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		winrt::fire_and_forget SuggestionsList_ItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e);

		void SuggestionsList_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		void SuggestionList_GotFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void RichEditBox_OnPointerMoved(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		void RichEditBox_SelectionChanging(winrt::RichEditBox const& sender, winrt::RichEditBoxSelectionChangingEventArgs const& args);

		winrt::fire_and_forget RichEditBox_SelectionChanged(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void RichEditBox_OnPointerPressed(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		winrt::fire_and_forget RichEditBox_ProcessKeyboardAccelerators(winrt::UIElement const& sender, winrt::ProcessKeyboardAcceleratorEventArgs const& args);

		winrt::fire_and_forget RichEditBox_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void RichEditBox_TextChanging(winrt::RichEditBox const& sender, winrt::RichEditBoxTextChangingEventArgs const& args);

		void RichEditBox_TextChanged(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void RichEditBox_TextCompositionStarted(winrt::RichEditBox const& sender, winrt::TextCompositionStartedEventArgs const& args);

		winrt::fire_and_forget RichEditBox_TextCompositionChanged(winrt::RichEditBox const& sender, winrt::TextCompositionChangedEventArgs const& args);

		void RichEditBox_TextCompositionEnded(winrt::RichEditBox const& sender, winrt::TextCompositionEndedEventArgs const& args);

		void RichEditBox_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		winrt::fire_and_forget RichEditBox_Paste(winrt::IInspectable const& sender, winrt::TextControlPasteEventArgs const& e);

		void ConditionallyLoadElement(winrt::IInspectable const& property, std::wstring_view elementName);

		void UpdateVisibleTokenList();

#pragma region Helpers
		static bool IsElementOnScreen(winrt::FrameworkElement const& element, double offsetX = 0, double offsetY = 0);

		static bool IsElementInsideWindow(winrt::FrameworkElement const& element, double offsetX = 0, double offsetY = 0);

		static winrt::hstring EnforcePrefixesRequirements(winrt::hstring const& value);

		static void PadRange(winrt::ITextRange const& range, winrt::ITextCharacterFormat const& format);

		static void ForEachLinkInDocument(winrt::RichEditTextDocument const& document, std::function<void(winrt::ITextRange const&)> const& action);
#pragma endregion

#pragma region Document
		void CreateSingleEdit(std::function<void()> const& editAction);

		void ExpandSelectionOnPartialTokenSelect(winrt::ITextSelection const& selection, winrt::ITextRange const& tokenRange);

		void InvokeTokenSelected(winrt::ITextSelection const& selection);

		void InvokeTokenPointerOver(winrt::PointerPoint const& pointer);

		void ValidateTokensInDocument();

		void ValidateTokenFromRange(winrt::ITextRange const& range);

		bool TryCommitSuggestionIntoDocument(winrt::ITextRange const& range, winrt::hstring const& displayText, winrt::guid const& id, winrt::ITextCharacterFormat const& format, bool addTrailingSpace) const;

		bool TryExtractQueryFromSelection(winrt::hstring& prefix, winrt::hstring& query, winrt::ITextRange& range);

		bool TryExtractQueryFromRange(winrt::ITextRange const& range, winrt::hstring& prefix, winrt::hstring& query);

		winrt::ITextCharacterFormat CreateTokenFormat(winrt::ITextRange const& range) const;
#pragma endregion

#pragma region Suggestion
		winrt::IAsyncAction RequestSuggestionsAsync(winrt::ITextRange& range);

		winrt::IAsyncAction CommitSuggestionAsync(winrt::IInspectable const& selectedItem);

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
