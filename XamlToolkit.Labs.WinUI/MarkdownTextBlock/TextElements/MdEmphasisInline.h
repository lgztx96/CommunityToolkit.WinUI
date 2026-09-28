// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdInlineText.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Windows.UI.Text.h>
#include <format>
#include <optional>
#endif

namespace winrt
{
	using namespace Windows::UI::Text;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdEmphasisInline final : public IAddChild
    {
    private:
        winrt::Span _span;

        bool _isBold;
        bool _isItalic;
        bool _isStrikeThrough;
        bool _isUnderline;
        bool _isSubscript;
        bool _isSuperscript;
        winrt::FontWeight _boldFontWeight{ winrt::FontWeights::Bold() };

    public:

		MdEmphasisInline() : _isBold(false), _isItalic(false), _isStrikeThrough(false), _isUnderline(false),
            _isSubscript(false), _isSuperscript(false)
        {
		}

        winrt::TextElement TextElement() const override
        {
            return _span;
        }

        void AddChild(IAddChild* child) override
        {
            try
            {
                if (auto emphasisInline = dynamic_cast<const MdEmphasisInline*>(child))
                {
                    // Nested emphasis is merged into this span instead of being nested inside it,
                    // so the flags (and the weight configured for bold) have to carry over.
                    if (emphasisInline->_isBold) { SetBold(emphasisInline->_boldFontWeight); }
                    if (emphasisInline->_isItalic) { SetItalic(); }
                    if (emphasisInline->_isStrikeThrough) { SetStrikeThrough(); }
                    if (emphasisInline->_isUnderline) { SetUnderline(); }
                    if (emphasisInline->_isSubscript) { SetSubscript(); }
                    if (emphasisInline->_isSuperscript) { SetSuperscript(); }

                    _span.Inlines().Append(emphasisInline->_span);
                }
                else if (auto inlineChild = child->TextElement().try_as<winrt::Inline>())
                {
                    // Anything else an emphasis can hold (text runs, code, links, images, line
                    // breaks) is a plain inline as far as XAML is concerned.
                    _span.Inlines().Append(inlineChild);
                }
            }
            catch (const winrt::hresult_error& ex)
            {
                throw winrt::hresult_illegal_method_call(std::format(L"Error in {}.{}: {}", L"MdEmphasisInline", L"AddChild", ex.message()).c_str());
            }
        }

        void SetBold(std::optional<winrt::FontWeight> const& fontWeight = std::nullopt)
        {
            _boldFontWeight = fontWeight ? *fontWeight : winrt::FontWeights::Bold();
            _span.FontWeight(_boldFontWeight);
            _isBold = true;
        }

        void SetItalic()
        {
            _span.FontStyle(winrt::FontStyle::Italic);
            _isItalic = true;
        }

        void SetStrikeThrough()
        {
			assert(!_isUnderline);
            _span.TextDecorations(winrt::TextDecorations::Strikethrough);
            _isStrikeThrough = true;
        }

        void SetUnderline()
        {
			assert(!_isStrikeThrough);
            _span.TextDecorations(winrt::TextDecorations::Underline);
            _isUnderline = true;
        }

        void SetSubscript()
        {
            _span.SetValue(winrt::Typography::VariantsProperty(), winrt::box_value(winrt::FontVariants::Subscript));
            _isSubscript = true;
        }

        void SetSuperscript()
        {
            _span.SetValue(winrt::Typography::VariantsProperty(), winrt::box_value(winrt::FontVariants::Superscript));
            _isSuperscript = true;
        }
    };
}

