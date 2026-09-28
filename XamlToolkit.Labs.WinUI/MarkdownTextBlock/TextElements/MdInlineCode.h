// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/XamlToolkit.Labs.WinUI.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Documents;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdInlineCode final : public IAddChild
    {
        winrt::Span _span;
        MarkdownTextBlock _control;

    public:
        winrt::TextElement TextElement() const override
        {
            return _span;
        }

		MdInlineCode(XamlToolkit::Labs::WinUI::MarkdownTextBlock const& control) : _control(control) { }

        void Enter() override
        {
            _span.Foreground(_control.InlineCodeForeground());
            _span.FontWeight(_control.InlineCodeFontWeight());
		}

        void AddChild(IAddChild* child) override
        {
            if (auto inlineElement = child->TextElement().try_as<winrt::Inline>())
            {
				_span.Inlines().Append(inlineElement);
            }
        }
    };
}
