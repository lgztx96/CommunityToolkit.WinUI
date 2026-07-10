// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdFlowDocument.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdQuote final : public IAddChild
    {
    private:
        winrt::Paragraph _paragraph;
        MdFlowDocument _flowDocument;
        MarkdownThemes _themes;

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdQuote(MarkdownThemes const& themes) : _themes(themes)
        {
            winrt::InlineUIContainer inlineUIContainer;
            
            winrt::Grid grid;
            winrt::ColumnDefinition column;
            column.Width(winrt::GridLength(1, winrt::GridUnitType::Auto));
            grid.ColumnDefinitions().Append(column);
            column = winrt::ColumnDefinition();
            column.Width(winrt::GridLength(1, winrt::GridUnitType::Auto));
            grid.ColumnDefinitions().Append(column);

            winrt::Grid bar;
            auto borderThickness = _themes.QuoteBorderThickness().Left > 0 ? _themes.QuoteBorderThickness().Left : 4;
            bar.Width(borderThickness);
            if (!_themes.QuoteBorderBrush()) _themes.QuoteBorderBrush(winrt::SolidColorBrush(winrt::Microsoft::UI::Colors::Gray()));
            bar.Background(_themes.QuoteBorderBrush());
            bar.SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(0));
            bar.VerticalAlignment(winrt::VerticalAlignment::Stretch);
            bar.Margin(winrt::Thickness(0, 0, 4, 0));
            grid.Children().Append(bar);

            winrt::Grid rightGrid;
            rightGrid.Padding(_themes.QuotePadding());
            rightGrid.Background(_themes.QuoteBackground());
            rightGrid.CornerRadius(_themes.QuoteCornerRadius());
            rightGrid.Children().Append(_flowDocument.RichTextBlock());
            _flowDocument.RichTextBlock().Foreground(_themes.QuoteForeground());

            rightGrid.SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(1));
            grid.Children().Append(rightGrid);
            grid.Margin(_themes.QuoteMargin());

            inlineUIContainer.Child(grid);

            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}


