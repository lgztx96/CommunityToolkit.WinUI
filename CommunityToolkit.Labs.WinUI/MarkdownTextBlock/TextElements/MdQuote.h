// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdFlowDocument.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    class MdQuote final : public IAddChild
    {
    private:
        Paragraph _paragraph;
        MdFlowDocument _flowDocument;
        MarkdownThemes _themes;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdQuote(MarkdownThemes const& themes) : _themes(themes)
        {
            InlineUIContainer inlineUIContainer;
            
            Grid grid;
            ColumnDefinition column;
            column.Width(GridLength(1, GridUnitType::Auto));
            grid.ColumnDefinitions().Append(column);
            column = ColumnDefinition();
            column.Width(GridLength(1, GridUnitType::Auto));
            grid.ColumnDefinitions().Append(column);

            Grid bar;
            auto borderThickness = _themes.QuoteBorderThickness().Left > 0 ? _themes.QuoteBorderThickness().Left : 4;
            bar.Width(borderThickness);
            if (!_themes.QuoteBorderBrush()) _themes.QuoteBorderBrush(SolidColorBrush(Microsoft::UI::Colors::Gray()));
            bar.Background(_themes.QuoteBorderBrush());
            bar.SetValue(Grid::ColumnProperty(), winrt::box_value(0));
            bar.VerticalAlignment(VerticalAlignment::Stretch);
            bar.Margin(Thickness(0, 0, 4, 0));
            grid.Children().Append(bar);

            Grid rightGrid;
            rightGrid.Padding(_themes.QuotePadding());
            rightGrid.Background(_themes.QuoteBackground());
            rightGrid.CornerRadius(_themes.QuoteCornerRadius());
            rightGrid.Children().Append(_flowDocument.RichTextBlock());
            _flowDocument.RichTextBlock().Foreground(_themes.QuoteForeground());

            rightGrid.SetValue(Grid::ColumnProperty(), winrt::box_value(1));
            grid.Children().Append(rightGrid);
            grid.Margin(_themes.QuoteMargin());

            inlineUIContainer.Child(grid);

            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}


