#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <ranges>
#include <vector>
#endif
#include "RichSuggestBox.h"
#if __has_include("RichSuggestBox.g.cpp")
#include "RichSuggestBox.g.cpp"
#endif
#include "../../XamlToolkit.WinUI/common.h"

namespace winrt
{
	using namespace Windows::System;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	RichSuggestBox::RichSuggestBox()
	{
		_visibleTokens = winrt::single_threaded_observable_vector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken>();

		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		RegisterPropertyChangedCallback(Border::CornerRadiusProperty(), { this, &RichSuggestBox::OnCornerRadiusChanged });
		RegisterPropertyChangedCallback(PopupCornerRadiusProperty(), { this, &RichSuggestBox::OnCornerRadiusChanged });
		LostFocus({ this, &RichSuggestBox::OnLostFocusEvent });
		Loaded({ this, &RichSuggestBox::OnLoaded });

		_pointerPressedHandler = winrt::box_value(PointerEventHandler({ this, &RichSuggestBox::RichEditBox_OnPointerPressed }));
		_pointerMovedHandler = winrt::box_value(PointerEventHandler({ this, &RichSuggestBox::RichEditBox_OnPointerMoved }));
	}

	void RichSuggestBox::ClearUndoRedoSuggestionHistory()
	{
		TextDocument().ClearUndoRedoHistory();
		std::unique_lock<std::recursive_mutex> lock(_tokensLock);
		{
			if (_tokens.size() == 0)
			{
				return;
			}

			auto keysToDelete = _tokens
				| std::views::filter([](auto const& pair) { return !pair.second.Active(); })
				| std::views::transform([](auto const& pair) { return pair.first; })
				| std::ranges::to<std::vector>();
			for (const auto& key : keysToDelete)
			{
				_tokens.erase(key);
			}
		}
	}

	void RichSuggestBox::Clear()
	{
		std::unique_lock<std::recursive_mutex> lock(_tokensLock);

		_tokens.clear();
		_visibleTokens.Clear();
		auto document = TextDocument();
		auto selection = document.Selection();
		selection.Expand(TextRangeUnit::Story);
		selection.Delete(TextRangeUnit::Story, 0);
		document.ClearUndoRedoHistory();
	}

	void RichSuggestBox::AddTokens(IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens)
	{
		std::unique_lock<std::recursive_mutex> lock(_tokensLock);

		for (const auto& token : tokens)
		{
			_tokens.try_emplace(winrt::format(L"\"{}\"", winrt::to_hstring(token.Id())), token);
		}
	}

	void RichSuggestBox::Load(winrt::hstring const& rtf, IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens)
	{
		Clear();
		AddTokens(tokens);
		if (auto textDocument = TextDocument())
		{
			textDocument.SetText(TextSetOptions::FormatRtf, rtf);
		}
	}

	bool RichSuggestBox::TryGetTokenFromRange(ITextRange range, winrt::XamlToolkit::WinUI::Controls::RichSuggestToken& token)
	{
		token = nullptr;
		range = range.GetClone();
		if (range != nullptr && !range.Link().empty())
		{
			std::unique_lock<std::recursive_mutex> lock(_tokensLock);
			{
				if (auto iter = _tokens.find(range.Link()); iter != _tokens.end())
				{
					token = iter->second;
					return true;
				}
			}
		}

		return false;
	}

	Rect RichSuggestBox::GetRectFromRange(ITextRange const& range)
	{
		auto padding = _richEditBox.Padding();
		Rect rect;
		int32_t hit;
		range.GetRect(PointOptions::None, rect, hit);
		rect.X += static_cast<float>(padding.Left - HorizontalOffset());
		rect.Y += static_cast<float>(padding.Top - VerticalOffset());
		auto transform = _richEditBox.TransformToVisual(*this);
		return transform.TransformBounds(rect);
	}

	void RichSuggestBox::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		_suggestionPopup = GetTemplateChild(PartSuggestionsPopup).try_as<Popup>();
		_richEditBox = GetTemplateChild(PartRichEditBox).try_as<RichEditBox>();
		_suggestionsList = GetTemplateChild(PartSuggestionsList).try_as<ListViewBase>();
		_suggestionsContainer = GetTemplateChild(PartSuggestionsContainer).try_as<Border>();
		ConditionallyLoadElement(Header(), PartHeaderContentPresenter);
		ConditionallyLoadElement(Description(), PartDescriptionPresenter);

		if (_richEditBox)
		{
			_editBoxSizeChangedRevoker.revoke();
			_editBoxTextChangingRevoker.revoke();
			_editBoxTextChangedRevoker.revoke();
			_editBoxTextCompositionStartedRevoker.revoke();
			_editBoxTextCompositionChangedRevoker.revoke();
			_editBoxTextCompositionEndedRevoker.revoke();
			_editBoxSelectionChangingRevoker.revoke();
			_editBoxSelectionChangedRevoker.revoke();
			_editBoxPasteRevoker.revoke();
			_editBoxPreviewKeyDownRevoker.revoke();
			_richEditBox.RemoveHandler(UIElement::PointerMovedEvent(), _pointerMovedHandler);
			_richEditBox.RemoveHandler(UIElement::PointerPressedEvent(), _pointerPressedHandler);
			_editBoxProcessKeyboardAcceleratorsRevoker.revoke();

			_editBoxSizeChangedRevoker = _richEditBox.SizeChanged(winrt::auto_revoke, { this, &RichSuggestBox::RichEditBox_SizeChanged });
			_editBoxTextChangingRevoker = _richEditBox.TextChanging(winrt::auto_revoke, { this, &RichSuggestBox::RichEditBox_TextChanging });
			_editBoxTextChangedRevoker = _richEditBox.TextChanged(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_TextChanged });
			_editBoxTextCompositionStartedRevoker = _richEditBox.TextCompositionStarted(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_TextCompositionStarted });
			_editBoxTextCompositionChangedRevoker = _richEditBox.TextCompositionChanged(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_TextCompositionChanged });
			_editBoxTextCompositionEndedRevoker = _richEditBox.TextCompositionEnded(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_TextCompositionEnded });
			_editBoxSelectionChangingRevoker = _richEditBox.SelectionChanging(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_SelectionChanging });
			_editBoxSelectionChangedRevoker = _richEditBox.SelectionChanged(winrt::auto_revoke, { this,&RichSuggestBox::RichEditBox_SelectionChanged });
			_editBoxPasteRevoker = _richEditBox.Paste(winrt::auto_revoke, { this, &RichSuggestBox::RichEditBox_Paste });
			_editBoxPreviewKeyDownRevoker = _richEditBox.PreviewKeyDown(winrt::auto_revoke, { this, &RichSuggestBox::RichEditBox_PreviewKeyDown });
			_richEditBox.AddHandler(UIElement::PointerMovedEvent(), _pointerMovedHandler, true);
			_richEditBox.AddHandler(UIElement::PointerPressedEvent(), _pointerPressedHandler, true);
			_editBoxProcessKeyboardAcceleratorsRevoker = _richEditBox.ProcessKeyboardAccelerators(winrt::auto_revoke, { this, &RichSuggestBox::RichEditBox_ProcessKeyboardAccelerators });
		}

		if (_suggestionsList)
		{
			_listItemClickRevoker.revoke();
			_listSizeChangedRevoker.revoke();
			_listGotFocusRevoker.revoke();

			_listItemClickRevoker = _suggestionsList.ItemClick(winrt::auto_revoke, { this, &RichSuggestBox::SuggestionsList_ItemClick });
			_listSizeChangedRevoker = _suggestionsList.SizeChanged(winrt::auto_revoke, { this, &RichSuggestBox::SuggestionsList_SizeChanged });
			_listGotFocusRevoker = _suggestionsList.GotFocus(winrt::auto_revoke, { this,  &RichSuggestBox::SuggestionList_GotFocus });
		}
	}

	void RichSuggestBox::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->ConditionallyLoadElement(e.NewValue(), PartHeaderContentPresenter);
	}

	void RichSuggestBox::OnDescriptionChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->ConditionallyLoadElement(e.NewValue(), PartDescriptionPresenter);
	}

	void RichSuggestBox::OnSuggestionPopupPlacementChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->UpdatePopupWidth();
	}

	void RichSuggestBox::OnPrefixesChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();

		auto newValue = winrt::unbox_value<winrt::hstring>(e.NewValue());
		auto prefixes = EnforcePrefixesRequirements(newValue);

		if (newValue != prefixes)
		{
			view->SetValue(PrefixesProperty, winrt::box_value(prefixes));
		}
	}

	void RichSuggestBox::OnCornerRadiusChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		UpdateCornerRadii();
	}

	void RichSuggestBox::OnLoaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_richEditBox) _scrollViewer = DependencyObjectEx::FindDescendant<ScrollViewer>(_richEditBox);
	}

	void RichSuggestBox::OnLostFocusEvent([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		ShowSuggestionsPopup(false);
	}

	winrt::fire_and_forget RichSuggestBox::SuggestionsList_ItemClick(IInspectable const& sender, ItemClickEventArgs const& e)
	{
		auto selectedItem = e.ClickedItem();
		co_await CommitSuggestionAsync(selectedItem);
	}

	void RichSuggestBox::SuggestionsList_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		if (_suggestionPopup.IsOpen())
		{
			UpdatePopupOffset();
		}
	}

	void RichSuggestBox::SuggestionList_GotFocus([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_richEditBox) _richEditBox.Focus(FocusState::Programmatic);
	}

	void RichSuggestBox::RichEditBox_OnPointerMoved([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		auto pointer = e.GetCurrentPoint(*this);
		if (_tokenPointerOver)
		{
			InvokeTokenPointerOver(pointer);
		}
	}

	void RichSuggestBox::RichEditBox_SelectionChanging([[maybe_unused]] RichEditBox const& sender, [[maybe_unused]] RichEditBoxSelectionChangingEventArgs const& args)
	{
		auto selection = TextDocument().Selection();

		if (selection.Type() != SelectionType::InsertionPoint && selection.Type() != SelectionType::Normal)
		{
			return;
		}

		auto range = selection.GetClone();
		range.Expand(TextRangeUnit::Link);

		{
			std::unique_lock<std::recursive_mutex> lock(_tokensLock);
			if (!_tokens.contains(range.Link()))
			{
				return;
			}
		}

		ExpandSelectionOnPartialTokenSelect(selection, range);
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_SelectionChanged(IInspectable const& sender, RoutedEventArgs const& e)
	{
		SelectionChanged.invoke(*this, e);

		// During text composition changing (e.g. user typing with an IME),
		// SelectionChanged event is fired multiple times with each keystroke.
		// To reduce the number of suggestion requests, the request is made
		// in TextCompositionChanged handler instead.
		if (_textCompositionActive)
		{
			co_return;
		}

		ITextRange range{ nullptr };
		std::shared_ptr<RichSuggestQuery> query = _currentQuery;
		auto action{ RequestSuggestionsAsync(range) };
		if (query && query->Task == nullptr) 
		{
			query->Task = action;
		}
		co_await action;
	}

	void RichSuggestBox::RichEditBox_OnPointerPressed([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
	{
		ShowSuggestionsPopup(false);
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_ProcessKeyboardAccelerators(UIElement const& sender, ProcessKeyboardAcceleratorEventArgs const& args)
	{
		auto itemsList = _suggestionsList.Items();
		if (!_suggestionPopup.IsOpen() || itemsList == nullptr || itemsList.Size() == 0)
		{
			co_return;
		}

		auto key = args.Key();
		switch (key)
		{
		case VirtualKey::Up:
			if (itemsList.Size() == 1)
			{
				args.Handled(true);
				UpdateSuggestionsListSelectedItem(1);
				break;
			}
			args.Handled(true);
			_suggestionChoice = _suggestionChoice <= 0 ? itemsList.Size() : _suggestionChoice - 1;
			UpdateSuggestionsListSelectedItem(_suggestionChoice);
			break;

		case VirtualKey::Down:
			if (itemsList.Size() == 1)
			{
				args.Handled(true);
				UpdateSuggestionsListSelectedItem(1);
				break;
			}
			args.Handled(true);
			_suggestionChoice = _suggestionChoice >= static_cast<int>(itemsList.Size()) ? 0 : _suggestionChoice + 1;
			UpdateSuggestionsListSelectedItem(_suggestionChoice);
			break;

		case VirtualKey::Enter:
			if (auto item = _suggestionsList.SelectedItem())
			{
				args.Handled(true);
				co_await CommitSuggestionAsync(item);
			}
			break;

		case VirtualKey::Escape:
			args.Handled(true);
			ShowSuggestionsPopup(false);
			break;
		}
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (e.Key() == VirtualKey::Tab && _suggestionPopup.IsOpen() && _suggestionsList.SelectedItem())
		{
			e.Handled(true);
			co_await CommitSuggestionAsync(_suggestionsList.SelectedItem());
		}
	}

	void RichSuggestBox::RichEditBox_TextChanging([[maybe_unused]] RichEditBox const& sender, RichEditBoxTextChangingEventArgs const& args)
	{
		if (_ignoreChange || !args.IsContentChanging())
		{
			return;
		}

		{
			std::unique_lock<std::recursive_mutex> lock(_tokensLock);
			CreateSingleEdit([this]() { ValidateTokensInDocument(); });
		}
	}

	void RichSuggestBox::RichEditBox_TextChanged([[maybe_unused]] IInspectable const& sender, RoutedEventArgs const& e)
	{
		UpdateVisibleTokenList();
		TextChanged.invoke(*this, e);
	}

	void RichSuggestBox::RichEditBox_TextCompositionStarted([[maybe_unused]] RichEditBox const& sender, [[maybe_unused]] TextCompositionStartedEventArgs const& args)
	{
		_textCompositionActive = true;
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_TextCompositionChanged(RichEditBox const& sender, TextCompositionChangedEventArgs const& args)
	{
		auto range = TextDocument().GetRange(args.StartIndex() == 0 ? 0 : args.StartIndex() - 1, args.StartIndex() + args.Length());
		std::shared_ptr<RichSuggestQuery> query = _currentQuery;
		auto action{ RequestSuggestionsAsync(range) };
		if (query->Task == nullptr)
		{
			query->Task = action;
		}
		co_await action;
	}

	void RichSuggestBox::RichEditBox_TextCompositionEnded([[maybe_unused]] RichEditBox const& sender, [[maybe_unused]] TextCompositionEndedEventArgs const& args)
	{
		_textCompositionActive = false;
	}

	void RichSuggestBox::RichEditBox_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		UpdatePopupWidth();
		UpdatePopupOffset();
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_Paste(IInspectable const& sender, TextControlPasteEventArgs const& e)
	{
		Paste.invoke(*this, e);

		if (e.Handled() || TextDocument() == nullptr || ClipboardPasteFormat() != RichEditClipboardFormat::PlainText)
		{
			co_return;
		}

		e.Handled(true);
		auto dataPackageView = winrt::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
		if (dataPackageView.Contains(winrt::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text()))
		{
			auto text = co_await dataPackageView.GetTextAsync();
			TextDocument().Selection().SetText(TextSetOptions::Unhide, text);
			TextDocument().Selection().Collapse(false);
		}
	}

	void RichSuggestBox::ConditionallyLoadElement(IInspectable const& property, std::wstring_view elementName)
	{
		if (property)
		{
			if (auto presenter = GetTemplateChild(elementName).try_as<UIElement>())
			{
				presenter.Visibility(Visibility::Visible);
			}
		}
	}

	void RichSuggestBox::UpdateVisibleTokenList()
	{

		std::unique_lock<std::recursive_mutex> lock(_tokensLock);
		std::vector<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> toBeRemoved;

		for (const auto& elem : _visibleTokens)
		{
			if (!elem.Active() || !_tokens.contains(winrt::format(L"\"{}\"", winrt::to_hstring(elem.Id()))))
			{
				toBeRemoved.emplace_back(elem);
			}
		}

		for (const auto& elem : toBeRemoved)
		{
			if (uint32_t index; _visibleTokens.IndexOf(elem, index)) _visibleTokens.RemoveAt(index);
		}

		auto toBeAdded = _tokens
			| std::views::values
			| std::views::filter([&](auto const& token)
				{
					uint32_t index;
					return token.Active() && !_visibleTokens.IndexOf(token, index);
				})
			| std::ranges::to<std::vector>();

		for (const auto& elem : toBeAdded)
		{
			_visibleTokens.Append(elem);
		}
	}
}
