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

		RegisterPropertyChangedCallback(winrt::Border::CornerRadiusProperty(), { this, &RichSuggestBox::OnCornerRadiusChanged });
		RegisterPropertyChangedCallback(PopupCornerRadiusProperty(), { this, &RichSuggestBox::OnCornerRadiusChanged });
		LostFocus({ this, &RichSuggestBox::OnLostFocusEvent });
		Loaded({ this, &RichSuggestBox::OnLoaded });

		_pointerPressedHandler = winrt::box_value(winrt::PointerEventHandler({ this, &RichSuggestBox::RichEditBox_OnPointerPressed }));
		_pointerMovedHandler = winrt::box_value(winrt::PointerEventHandler({ this, &RichSuggestBox::RichEditBox_OnPointerMoved }));
	}

	void RichSuggestBox::ClearUndoRedoSuggestionHistory()
	{
		TextDocument().ClearUndoRedoHistory();
		std::unique_lock<std::recursive_mutex> lock(_tokensLock);
		{
			if (_tokens.empty())
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
		selection.Expand(winrt::TextRangeUnit::Story);
		selection.Delete(winrt::TextRangeUnit::Story, 0);
		document.ClearUndoRedoHistory();
	}

	void RichSuggestBox::AddTokens(winrt::IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens)
	{
		std::unique_lock<std::recursive_mutex> lock(_tokensLock);

		for (const auto& token : tokens)
		{
			_tokens.try_emplace(winrt::format(L"\"{}\"", winrt::to_hstring(token.Id())), token);
		}
	}

	void RichSuggestBox::Load(winrt::hstring const& rtf, winrt::IIterable<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> const& tokens)
	{
		Clear();
		AddTokens(tokens);
		if (auto textDocument = TextDocument())
		{
			textDocument.SetText(winrt::TextSetOptions::FormatRtf, rtf);
		}
	}

	bool RichSuggestBox::TryGetTokenFromRange(winrt::ITextRange const& range, winrt::XamlToolkit::WinUI::Controls::RichSuggestToken& token)
	{
		token = nullptr;
		if (range)
		{
			if (auto link = range.Link(); !link.empty())
			{
				std::unique_lock<std::recursive_mutex> lock(_tokensLock);
				if (auto iter = _tokens.find(link); iter != _tokens.end())
				{
					token = iter->second;
					return true;
				}
			}
		}

		return false;
	}

	winrt::Rect RichSuggestBox::GetRectFromRange(winrt::ITextRange const& range)
	{
		auto padding = _richEditBox.Padding();
		winrt::Rect rect;
		int32_t hit;
		range.GetRect(winrt::PointOptions::None, rect, hit);
		rect.X += static_cast<float>(padding.Left - HorizontalOffset());
		rect.Y += static_cast<float>(padding.Top - VerticalOffset());
		auto transform = _richEditBox.TransformToVisual(*this);
		return transform.TransformBounds(rect);
	}

	void RichSuggestBox::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		_suggestionPopup = GetTemplateChild(PartSuggestionsPopup).try_as<winrt::Popup>();
		_richEditBox = GetTemplateChild(PartRichEditBox).try_as<winrt::RichEditBox>();
		_suggestionsList = GetTemplateChild(PartSuggestionsList).try_as<winrt::ListViewBase>();
		_suggestionsContainer = GetTemplateChild(PartSuggestionsContainer).try_as<winrt::Border>();
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
			_richEditBox.RemoveHandler(winrt::UIElement::PointerMovedEvent(), _pointerMovedHandler);
			_richEditBox.RemoveHandler(winrt::UIElement::PointerPressedEvent(), _pointerPressedHandler);
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
			_richEditBox.AddHandler(winrt::UIElement::PointerMovedEvent(), _pointerMovedHandler, true);
			_richEditBox.AddHandler(winrt::UIElement::PointerPressedEvent(), _pointerPressedHandler, true);
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

	void RichSuggestBox::OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->ConditionallyLoadElement(e.NewValue(), PartHeaderContentPresenter);
	}

	void RichSuggestBox::OnDescriptionChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->ConditionallyLoadElement(e.NewValue(), PartDescriptionPresenter);
	}

	void RichSuggestBox::OnSuggestionPopupPlacementChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();
		view->UpdatePopupWidth();
	}

	void RichSuggestBox::OnPrefixesChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto view = winrt::get_self<RichSuggestBox>(d.as<class_type>())->get_strong();

		auto newValue = winrt::unbox_value<winrt::hstring>(e.NewValue());
		auto prefixes = EnforcePrefixesRequirements(newValue);

		if (newValue != prefixes)
		{
			view->SetValue(PrefixesProperty, winrt::box_value(prefixes));
		}
	}

	void RichSuggestBox::OnCornerRadiusChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		UpdateCornerRadii();
	}

	void RichSuggestBox::OnLoaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (_richEditBox) _scrollViewer = DependencyObjectEx::FindDescendant<ScrollViewer>(_richEditBox);
	}

	void RichSuggestBox::OnLostFocusEvent([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		ShowSuggestionsPopup(false);
	}

	winrt::fire_and_forget RichSuggestBox::SuggestionsList_ItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e)
	{
		auto selectedItem = e.ClickedItem();
		co_await CommitSuggestionAsync(selectedItem);
	}

	void RichSuggestBox::SuggestionsList_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
	{
		if (_suggestionPopup.IsOpen())
		{
			UpdatePopupOffset();
		}
	}

	void RichSuggestBox::SuggestionList_GotFocus([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (_richEditBox) _richEditBox.Focus(winrt::FocusState::Programmatic);
	}

	void RichSuggestBox::RichEditBox_OnPointerMoved([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
	{
		auto pointer = e.GetCurrentPoint(*this);
		if (_tokenPointerOver)
		{
			InvokeTokenPointerOver(pointer);
		}
	}

	void RichSuggestBox::RichEditBox_SelectionChanging([[maybe_unused]] winrt::RichEditBox const& sender, [[maybe_unused]] winrt::RichEditBoxSelectionChangingEventArgs const& args)
	{
		auto selection = TextDocument().Selection();
		auto selectionType = selection.Type();
		if (selectionType != winrt::SelectionType::InsertionPoint && selectionType != winrt::SelectionType::Normal)
		{
			return;
		}

		auto range = selection.GetClone();
		range.Expand(winrt::TextRangeUnit::Link);

		{
			std::unique_lock<std::recursive_mutex> lock(_tokensLock);
			if (!_tokens.contains(range.Link()))
			{
				return;
			}
		}

		ExpandSelectionOnPartialTokenSelect(selection, range);
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_SelectionChanged(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
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

		winrt::ITextRange range{ nullptr };
		co_await RequestSuggestionsAsync(range);
	}

	void RichSuggestBox::RichEditBox_OnPointerPressed([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
	{
		ShowSuggestionsPopup(false);
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_ProcessKeyboardAccelerators(winrt::UIElement const& sender, winrt::ProcessKeyboardAcceleratorEventArgs const& args)
	{
		auto itemsList = _suggestionsList.Items();
		if (!_suggestionPopup.IsOpen() || itemsList == nullptr || itemsList.Size() == 0)
		{
			co_return;
		}

		auto key = args.Key();
		switch (key)
		{
		case winrt::VirtualKey::Up:
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

		case winrt::VirtualKey::Down:
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

		case winrt::VirtualKey::Enter:
			if (auto item = _suggestionsList.SelectedItem())
			{
				args.Handled(true);
				co_await CommitSuggestionAsync(item);
			}
			break;

		case winrt::VirtualKey::Escape:
			args.Handled(true);
			ShowSuggestionsPopup(false);
			break;
		}
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		if (e.Key() == winrt::VirtualKey::Tab && _suggestionPopup.IsOpen())
		{
			if (const auto item = _suggestionsList.SelectedItem())
			{
				e.Handled(true);
				co_await CommitSuggestionAsync(item);
			}
		}
	}

	void RichSuggestBox::RichEditBox_TextChanging([[maybe_unused]] winrt::RichEditBox const& sender, [[maybe_unused]] winrt::RichEditBoxTextChangingEventArgs const& args)
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

	void RichSuggestBox::RichEditBox_TextChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		UpdateVisibleTokenList();
		TextChanged.invoke(*this, e);
	}

	void RichSuggestBox::RichEditBox_TextCompositionStarted([[maybe_unused]] winrt::RichEditBox const& sender, [[maybe_unused]] winrt::TextCompositionStartedEventArgs const& args)
	{
		_textCompositionActive = true;
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_TextCompositionChanged(winrt::RichEditBox const& sender, winrt::TextCompositionChangedEventArgs const& args)
	{
		auto startIndex = args.StartIndex();
		auto range = TextDocument().GetRange(startIndex == 0 ? 0 : startIndex - 1, startIndex + args.Length());
		co_await RequestSuggestionsAsync(range);
	}

	void RichSuggestBox::RichEditBox_TextCompositionEnded([[maybe_unused]] winrt::RichEditBox const& sender, [[maybe_unused]] winrt::TextCompositionEndedEventArgs const& args)
	{
		_textCompositionActive = false;
	}

	void RichSuggestBox::RichEditBox_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
	{
		UpdatePopupWidth();
		UpdatePopupOffset();
	}

	winrt::fire_and_forget RichSuggestBox::RichEditBox_Paste(winrt::IInspectable const& sender, winrt::TextControlPasteEventArgs const& e)
	{
		Paste.invoke(*this, e);

		auto textDocument = TextDocument();
		if (e.Handled() || textDocument == nullptr || ClipboardPasteFormat() != winrt::RichEditClipboardFormat::PlainText)
		{
			co_return;
		}

		e.Handled(true);
		auto dataPackageView = winrt::Clipboard::GetContent();
		if (dataPackageView.Contains(winrt::StandardDataFormats::Text()))
		{
			auto text = co_await dataPackageView.GetTextAsync();
			auto selection = textDocument.Selection();
			selection.SetText(winrt::TextSetOptions::Unhide, text);
			selection.Collapse(false);
		}
	}

	void RichSuggestBox::ConditionallyLoadElement(winrt::IInspectable const& property, std::wstring_view elementName)
	{
		if (property)
		{
			if (auto presenter = GetTemplateChild(elementName).try_as<winrt::UIElement>())
			{
				presenter.Visibility(winrt::Visibility::Visible);
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
			if (uint32_t index; _visibleTokens.IndexOf(elem, index))
			{
				_visibleTokens.RemoveAt(index);
			}
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
