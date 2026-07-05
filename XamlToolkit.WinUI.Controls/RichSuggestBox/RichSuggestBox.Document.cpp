#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RichSuggestBox.h"
#include "RichSuggestTokenSelectedEventArgs.h"
#include "RichSuggestTokenPointerOverEventArgs.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RichSuggestBox::CreateSingleEdit(std::function<void()> const& editAction)
    {
        _ignoreChange = true;
        editAction();
        if (auto document = TextDocument())
        {
            document.EndUndoGroup();
            document.BeginUndoGroup();
        }
        _ignoreChange = false;
    }

    void RichSuggestBox::ExpandSelectionOnPartialTokenSelect(winrt::ITextSelection const& selection, ITextRange const& tokenRange)
    {
        switch (selection.Type())
        {
        case winrt::SelectionType::InsertionPoint:
            // Snap selection to token on click
            if (tokenRange.StartPosition() < selection.StartPosition() && selection.EndPosition() < tokenRange.EndPosition())
            {
                selection.Expand(winrt::TextRangeUnit::Link);
                InvokeTokenSelected(selection);
            }

            break;

        case winrt::SelectionType::Normal:
            // We do not want user to partially select a token since pasting to a partial token can break
            // the token tracking system, which can result in unwanted character formatting issues.
            if ((tokenRange.StartPosition() <= selection.StartPosition() && selection.EndPosition() < tokenRange.EndPosition()) ||
                (tokenRange.StartPosition() < selection.StartPosition() && selection.EndPosition() <= tokenRange.EndPosition()))
            {
                // TODO: Figure out how to expand selection without breaking selection flow (with Shift select or pointer sweep select)
                selection.Expand(winrt::TextRangeUnit::Link);
                InvokeTokenSelected(selection);
            }

            break;
        }
    }

    void RichSuggestBox::InvokeTokenSelected(winrt::ITextSelection const& selection)
    {
        winrt::XamlToolkit::WinUI::Controls::RichSuggestToken token{ nullptr };
        if (_tokenSelected || !TryGetTokenFromRange(selection, token) || token.RangeEnd() != selection.EndPosition())
        {
            return;
        }
		auto args = winrt::make_self<RichSuggestTokenSelectedEventArgs>(token, selection.GetClone());
        _tokenSelected(*this, *args);
    }

    void RichSuggestBox::InvokeTokenPointerOver(winrt::PointerPoint const& pointer)
    {
        auto pointerPosition = TransformToVisual(_richEditBox).TransformPoint(pointer.Position());
        auto padding = _richEditBox.Padding();
        pointerPosition.X += static_cast<float>(HorizontalOffset() - padding.Left);
        pointerPosition.Y += static_cast<float>(VerticalOffset() - padding.Top);
        if (auto document = TextDocument()) 
        {
            auto range = document.GetRangeFromPoint(pointerPosition, winrt::PointOptions::ClientCoordinates);
            auto linkRange = range.GetClone();
            range.Expand(winrt::TextRangeUnit::Character);
            winrt::Rect hitTestRect;
            int32_t hit;
            range.GetRect(winrt::PointOptions::None, hitTestRect, hit);
            hitTestRect.X -= hitTestRect.Width;
            hitTestRect.Width *= 2;
            winrt::XamlToolkit::WinUI::Controls::RichSuggestToken token{ nullptr };
            if (winrt::RectHelper::Contains(hitTestRect, pointerPosition) && linkRange.Expand(TextRangeUnit::Link) > 0 &&
                TryGetTokenFromRange(linkRange, token))
            {
				auto args = winrt::make<RichSuggestTokenPointerOverEventArgs>(token, linkRange, pointer);
                _tokenPointerOver(*this, args);
            }
        }
    }

    void RichSuggestBox::ValidateTokensInDocument()
    {
        for (const auto&[_, token] : _tokens)
        {
            token.Active(false);
        }
        if (auto textDocument = TextDocument())
        {
            ForEachLinkInDocument(textDocument, [this](auto& range) { ValidateTokenFromRange(range); });
        }
    }

    void RichSuggestBox::ValidateTokenFromRange(winrt::ITextRange const& range)
    {
        winrt::XamlToolkit::WinUI::Controls::RichSuggestToken token{ nullptr };
        if (range.Length() == 0 || !TryGetTokenFromRange(range, token))
        {
            return;
        }

        // Check for duplicate tokens. This can happen if the user copies and pastes the token multiple times.
        if (token.Active() && token.RangeStart() != range.StartPosition() && token.RangeEnd() != range.EndPosition())
        {
            winrt::guid guid = winrt::GuidHelper::CreateNewGuid();
            if (TryCommitSuggestionIntoDocument(range, token.DisplayText(), guid, CreateTokenFormat(range), false))
            {
                token = winrt::make<RichSuggestToken>(guid, token.DisplayText());
                token.Active(true);
                token.Item(token.Item());
                token.UpdateTextRange(range);
                _tokens.try_emplace(range.Link(), token);
            }

            return;
        }

        if (token.ToString() != range.Text())
        {
            range.Delete(winrt::TextRangeUnit::Story, 0);
            token.Active(false);
            return;
        }

        token.UpdateTextRange(range);
        token.Active(true);
    }

    bool RichSuggestBox::TryCommitSuggestionIntoDocument(winrt::ITextRange const& range, winrt::hstring const& displayText, winrt::guid const& id, winrt::ITextCharacterFormat const& format, bool addTrailingSpace) const
    {
        // We don't want to set text when the display text doesn't change since it may lead to unexpected caret move.
		winrt::hstring existingText;
        range.GetText(winrt::TextGetOptions::NoHidden, existingText);
        if (existingText != displayText)
        {
            range.SetText(winrt::TextSetOptions::Unhide, displayText);
        }

        auto formatBefore = range.CharacterFormat().GetClone();
        range.CharacterFormat().SetClone(format);
        PadRange(range, formatBefore);
        range.Link(winrt::format(LR"("{}")", winrt::to_hstring(id)));

        // In some rare case, setting Link can fail. Only observed when interacting with Undo/Redo feature.
        if (range.Link() != winrt::format(LR"("{}")", winrt::to_hstring(id)))
        {
            range.Delete(winrt::TextRangeUnit::Story, -1);
            return false;
        }

        if (addTrailingSpace)
        {
            auto clone = range.GetClone();
            clone.Collapse(false);
            clone.SetText(winrt::TextSetOptions::Unhide, L" ");
            clone.Collapse(false);
            if (auto document = TextDocument())
            {
                document.Selection().SetRange(clone.EndPosition(), clone.EndPosition());
            }
        }

        return true;
    }

    bool RichSuggestBox::TryExtractQueryFromSelection(winrt::hstring& prefix, winrt::hstring& query, winrt::ITextRange& range)
    {
        prefix = {};
        query = {};
        range = nullptr;
		auto selection = TextDocument().Selection();
        if (selection.Type() != winrt::SelectionType::InsertionPoint)
        {
            return false;
        }

        // Check if selection is on existing link (suggestion)
        auto expandCount = selection.GetClone().Expand(winrt::TextRangeUnit::Link);
        if (expandCount != 0)
        {
            return false;
        }

        auto selectionClone = selection.GetClone();
        selectionClone.MoveStart(winrt::TextRangeUnit::Word, -1);
        if (selectionClone.Length() == 0)
        {
            return false;
        }

        range = selectionClone;
        if (TryExtractQueryFromRange(selectionClone, prefix, query))
        {
            return true;
        }

        selectionClone.MoveStart(winrt::TextRangeUnit::Word, -1);
        if (TryExtractQueryFromRange(selectionClone, prefix, query))
        {
            return true;
        }

        range = nullptr;
        return false;
    }

    bool RichSuggestBox::TryExtractQueryFromRange(winrt::ITextRange const& range, winrt::hstring& prefix, winrt::hstring& query)
    {
        prefix = {};
        query = {};
        winrt::hstring possibleQuery;
        range.GetText(winrt::TextGetOptions::NoHidden, possibleQuery);
        if (possibleQuery.size() > 0 && std::wstring_view(Prefixes()).contains(possibleQuery[0]) &&
            !std::any_of(possibleQuery.begin(), possibleQuery.end(), ::iswspace) && range.Link().empty())
        {
            if (possibleQuery.size() == 1)
            {
                prefix = possibleQuery;
                return true;
            }

            prefix = winrt::hstring(&possibleQuery[0], 1);
            query = std::wstring_view(possibleQuery).substr(1);
            return true;
        }

        return false;
    }

    winrt::ITextCharacterFormat RichSuggestBox::CreateTokenFormat(winrt::ITextRange const& range) const
    {
        auto format = range.CharacterFormat().GetClone();
        if (auto background = TokenBackground())
        {
            format.BackgroundColor(background.Color());
        }

        if (auto foreground = TokenForeground())
        {
            format.ForegroundColor(foreground.Color());
        }

        return format;
    }
}
