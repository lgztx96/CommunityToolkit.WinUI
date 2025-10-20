// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdInlineText.h"
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    using namespace winrt::Windows::UI::Text;

    class MdEmphasisInline final : public IAddChild
    {
    private:
        Span _span;

        bool _isBold;
        bool _isItalic;
        bool _isStrikeThrough;
        bool _isUnderline;

    public:

		MdEmphasisInline() : _isBold(false), _isItalic(false), _isStrikeThrough(false), _isUnderline(false)
        {
		}

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _span;
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            try
            {
                if (auto inlineText = dynamic_cast<const MdInlineText*>(child))
                {
                    _span.Inlines().Append(inlineText->TextElement().as<Run>());
                }
                else if (auto emphasisInline = dynamic_cast<const MdEmphasisInline*>(child))
                {
                    if (emphasisInline->_isBold) { SetBold(); }
                    if (emphasisInline->_isItalic) { SetItalic(); }
                    if (emphasisInline->_isStrikeThrough) { SetStrikeThrough(); }
                    if (emphasisInline->_isUnderline) { SetUnderline(); }
                    _span.Inlines().Append(emphasisInline->_span);
                }
            }
            catch (const winrt::hresult_error& ex)
            {
                throw winrt::hresult_illegal_method_call(std::format(L"Error in {}.{}: {}", L"MdEmphasisInline", L"AddChild", ex.message()).c_str());
            }
        }

        void SetBold()
        {
            _span.FontWeight(winrt::Microsoft::UI::Text::FontWeights::Bold());
            _isBold = true;
        }

        void SetItalic()
        {
            _span.FontStyle(FontStyle::Italic);
            _isItalic = true;
        }

        void SetStrikeThrough()
        {
			assert(!_isUnderline);
            _span.TextDecorations(Windows::UI::Text::TextDecorations::Strikethrough);
            _isStrikeThrough = true;
        }

        void SetUnderline()
        {
			assert(!_isStrikeThrough);
            _span.TextDecorations(Windows::UI::Text::TextDecorations::Underline);
            _isUnderline = true;
        }

        void SetSubscript()
        {
            _span.SetValue(Typography::VariantsProperty(), winrt::box_value(Microsoft::UI::Xaml::FontVariants::Subscript));
        }

        void SetSuperscript()
        {
            _span.SetValue(Typography::VariantsProperty(), winrt::box_value(Microsoft::UI::Xaml::FontVariants::Superscript));
        }
    };
}

