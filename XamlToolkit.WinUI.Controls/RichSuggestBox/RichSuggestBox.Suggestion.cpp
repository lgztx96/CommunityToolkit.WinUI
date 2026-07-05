#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <limits>
#endif
#include "RichSuggestBox.h"
#include "SuggestionRequestedEventArgs.h"
#include "SuggestionChosenEventArgs.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::IAsyncAction RichSuggestBox::RequestSuggestionsAsync(winrt::ITextRange& range)
	{
		winrt::hstring prefix;
		winrt::hstring query;
		std::shared_ptr<RichSuggestQuery> currentQuery = _currentQuery;
		auto queryFound = range == nullptr
			? TryExtractQueryFromSelection(prefix, query, range)
			: TryExtractQueryFromRange(range, prefix, query);

		if (queryFound &&
			prefix == (currentQuery ? currentQuery->Prefix : L"") &&
			query == (currentQuery ? currentQuery->QueryText : L"") &&
			range.EndPosition() == (currentQuery ? currentQuery->Range.EndPosition() : -1) &&
			_suggestionPopup.IsOpen())
		{
			co_return;
		}

		auto previousTokenSource = currentQuery ? currentQuery->CancellationTokenSource : std::nullopt;
		if (previousTokenSource)
		{
			previousTokenSource->cancel();
		}

		if (queryFound)
		{
			concurrency::cancellation_token_source tokenSource;
			_currentQuery = std::make_shared<RichSuggestQuery>();
			_currentQuery->Prefix = prefix;
			_currentQuery->QueryText = query;
			_currentQuery->Range = range;
			_currentQuery->CancellationTokenSource = tokenSource;

			auto cancellationToken = tokenSource.get_token();
			auto eventArgs = winrt::make_self<SuggestionRequestedEventArgs>();
			eventArgs->QueryText(query);
			eventArgs->Prefix(prefix);

			if (_suggestionRequested)
			{
				try
				{
					_suggestionRequested(*this, *eventArgs);
					auto action{ eventArgs->wait_for_deferrals() };
					cancellationToken.register_callback([action]() { action.Cancel(); });
					co_await action;
				}
				catch (...)
				{
					co_return;
				}
			}

			if (!eventArgs->Cancel())
			{
				_suggestionChoice = 0;
				ShowSuggestionsPopup(_suggestionsList ? _suggestionsList.Items() && _suggestionsList.Items().Size() > 0 : false);
			}
		}
		else
		{
			ShowSuggestionsPopup(false);
		}
	}

	winrt::IAsyncAction RichSuggestBox::CommitSuggestionAsync(winrt::IInspectable const& selectedItem)
	{
		std::shared_ptr<RichSuggestQuery> currentQuery = _currentQuery;
		winrt::ITextRange range{ nullptr };
		std::optional<winrt::hstring> prefix, query;

		if (currentQuery)
		{
			range = currentQuery->Range.GetClone();
			prefix = currentQuery->Prefix;
			query = currentQuery->QueryText;
		}

		winrt::guid id = winrt::GuidHelper::CreateNewGuid();
		// range has length of 0 at the end of the commit.
		// Checking length == 0 to avoid committing twice.
		if (!prefix.has_value() || !query.has_value() || range == nullptr || range.Length() == 0)
		{
			co_return;
		}

		auto textBefore = range.Text();
		auto format = CreateTokenFormat(range);

		auto eventArgs = winrt::make_self<SuggestionChosenEventArgs>();
		eventArgs->Id(id);
		eventArgs->Prefix(*prefix);
		eventArgs->QueryText(*query);
		eventArgs->SelectedItem(selectedItem);
		eventArgs->DisplayText(*query);
		eventArgs->Format(format);

		if (_suggestionChosen)
		{
			_suggestionChosen(*this, *eventArgs);
			co_await eventArgs->wait_for_deferrals();
		}

		const auto& text = eventArgs->DisplayText();

		// Since this operation is async, the document may have changed at this point.
		// Double check if the range still has the expected query.
		winrt::hstring testPrefix, testQuery;
		if (text.empty() || textBefore != range.Text() ||
			!TryExtractQueryFromRange(range, testPrefix, testQuery) ||
			testPrefix != prefix || testQuery != query)
		{
			co_return;
		}

		auto displayText = *prefix + text;

		auto RealizeToken = [&]()
		{
			if (TryCommitSuggestionIntoDocument(range, displayText, id, eventArgs->Format ? eventArgs->Format : format, true))
			{
				auto token = winrt::make<RichSuggestToken>(id, displayText);
				token.Active(true);
				token.Item(selectedItem);
				token.UpdateTextRange(range);
				_tokens.try_emplace(range.Link(), token);
			}
		};

		{
			std::unique_lock<std::recursive_mutex> lock(_tokensLock);
			CreateSingleEdit(RealizeToken);
		}
	}

	void RichSuggestBox::UpdateSuggestionsListSelectedItem(int choice)
	{
		auto itemsList = _suggestionsList.Items();
		if (itemsList == nullptr)
		{
			return;
		}

		_suggestionsList.SelectedItem(choice == 0 ? nullptr : itemsList.GetAt(choice - 1));
		_suggestionsList.ScrollIntoView(_suggestionsList.SelectedItem());
	}

	void RichSuggestBox::ShowSuggestionsPopup(bool show)
	{
		if (_suggestionPopup == nullptr)
		{
			return;
		}

		_suggestionPopup.IsOpen(show);
		if (!show)
		{
			_suggestionChoice = 0;
			_suggestionPopup.VerticalOffset(0);
			_suggestionPopup.HorizontalOffset(0);
			_suggestionsList.SelectedItem(nullptr);
			if (auto items = _suggestionsList.Items())
			{
				if (items.Size() > 0)
				{
					_suggestionsList.ScrollIntoView(items.GetAt(0));
				}
			}
			UpdateCornerRadii();
		}
	}

	void RichSuggestBox::UpdatePopupWidth()
	{
		if (_suggestionsContainer == nullptr)
		{
			return;
		}

		if (PopupPlacement() == SuggestionPopupPlacementMode::Attached)
		{
			_suggestionsContainer.MaxWidth(std::numeric_limits<double>::infinity());
			_suggestionsContainer.Width(_richEditBox.ActualWidth());
		}
		else
		{
			_suggestionsContainer.MaxWidth(_richEditBox.ActualWidth());
			_suggestionsContainer.Width(std::numeric_limits<double>::quiet_NaN());
		}
	}

	void RichSuggestBox::UpdatePopupOffset()
	{
		if (_suggestionsContainer == nullptr || _suggestionPopup == nullptr || _richEditBox == nullptr)
		{
			return;
		}

		winrt::Rect selectionRect;
		int32_t hit;
		_richEditBox.TextDocument().Selection().GetRect(winrt::PointOptions::None, selectionRect, hit);
		winrt::Thickness padding = _richEditBox.Padding();
		selectionRect.X -= static_cast<float>(HorizontalOffset());
		selectionRect.Y -= static_cast<float>(VerticalOffset());

		// Update horizontal offset
		if (PopupPlacement() == SuggestionPopupPlacementMode::Attached)
		{
			_suggestionPopup.HorizontalOffset(0);
		}
		else
		{
			double editBoxWidth = _richEditBox.ActualWidth() - padding.Left - padding.Right;
			if (_suggestionPopup.HorizontalOffset() == 0 && editBoxWidth > 0)
			{
				auto normalizedX = selectionRect.X / editBoxWidth;
				_suggestionPopup.HorizontalOffset((_richEditBox.ActualWidth() - _suggestionsContainer.ActualWidth()) * normalizedX);
			}
		}

		// Update vertical offset
		double downOffset = _richEditBox.ActualHeight();
		double upOffset = -_suggestionsContainer.ActualHeight();
		if (PopupPlacement() == SuggestionPopupPlacementMode::Floating)
		{
			auto rectBottom = winrt::RectHelper::GetBottom(selectionRect);
			auto rectTop = winrt::RectHelper::GetTop(selectionRect);
			downOffset = rectBottom + padding.Top + padding.Bottom;
			upOffset += rectTop;
		}

		if (_suggestionPopup.VerticalOffset() == 0)
		{
			if (IsElementOnScreen(_suggestionsContainer, 0.0, downOffset) &&
				(IsElementInsideWindow(_suggestionsContainer, 0.0, downOffset) ||
					!IsElementInsideWindow(_suggestionsContainer, 0.0, upOffset) ||
					!IsElementOnScreen(_suggestionsContainer, 0.0, upOffset)))
			{
				_suggestionPopup.VerticalOffset(downOffset);
				_popupOpenDown = true;
			}
			else
			{
				_suggestionPopup.VerticalOffset(upOffset);
				_popupOpenDown = false;
			}

			UpdateCornerRadii();
		}
		else
		{
			_suggestionPopup.VerticalOffset(_popupOpenDown ? downOffset : upOffset);
		}
	}

	/// <summary>
	/// Set corner radii so that inner corners, where suggestion list and text box connect, are square.
	/// This only applies when <see cref="PopupPlacement"/> is set to <see cref="SuggestionPopupPlacementMode.Attached"/>.
	/// </summary>
	/// https://docs.microsoft.com/en-us/windows/apps/design/style/rounded-corner#when-not-to-round
	void RichSuggestBox::UpdateCornerRadii()
	{
		if (_richEditBox == nullptr || _suggestionsContainer == nullptr ||
			!winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.Foundation.UniversalApiContract", 7))
		{
			return;
		}

		const auto cornerRadius = CornerRadius();
		const auto popupCornerRadius = PopupCornerRadius();
		_richEditBox.CornerRadius(cornerRadius);
		_suggestionsContainer.CornerRadius(popupCornerRadius);

		if (_suggestionPopup.IsOpen() && PopupPlacement() == SuggestionPopupPlacementMode::Attached)
		{
			if (_popupOpenDown)
			{
				_richEditBox.CornerRadius({ cornerRadius.TopLeft, cornerRadius.TopRight, 0, 0 });
				_suggestionsContainer.CornerRadius({ 0, 0, popupCornerRadius.BottomRight, popupCornerRadius.BottomLeft });
			}
			else
			{
				_richEditBox.CornerRadius({ 0, 0, cornerRadius.BottomRight, cornerRadius.BottomLeft });
				_suggestionsContainer.CornerRadius({ popupCornerRadius.TopLeft, popupCornerRadius.TopRight, 0, 0 });
			}
		}
	}
}
