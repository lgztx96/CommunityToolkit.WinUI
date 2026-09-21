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
        winrt::Paragraph _paragraph;

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdThematicBreak(MarkdownTextBlock const& control)
        {
            winrt::InlineUIContainer inlineUIContainer;
            winrt::Line line;
            line.Stretch(winrt::Stretch::Fill);
            line.Stroke(control.HorizontalRuleBrush());
            line.X2(1.0);
            line.StrokeThickness(control.HorizontalRuleThickness());
            line.Margin(control.HorizontalRuleMargin());
            inlineUIContainer.Child(line);
            _paragraph.Inlines().Append(inlineUIContainer);
        }
    };
}

