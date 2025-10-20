// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Shapes;
    using namespace Microsoft::UI::Xaml::Media;

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

