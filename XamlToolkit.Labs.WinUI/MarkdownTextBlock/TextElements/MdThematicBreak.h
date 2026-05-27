// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Shapes;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdThematicBreak final : public IAddChild
    {
    private:
        Paragraph _paragraph;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdThematicBreak(MarkdownThemes const& themes)
        {
            InlineUIContainer inlineUIContainer;
            Line line;
            line.Stretch(Stretch::Fill);
            line.Stroke(themes.HorizontalRuleBrush()
                ? themes.HorizontalRuleBrush() : themes.BorderBrush());
            line.X2(1.0);
            line.StrokeThickness(themes.HorizontalRuleThickness());
            line.Margin(themes.HorizontalRuleMargin());
            inlineUIContainer.Child(line);
            _paragraph.Inlines().Append(inlineUIContainer);
        }
    };
}

