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

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdQuote(MarkdownTextBlock const& control)
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
            auto borderThickness = control.QuoteBorderThickness().Left > 0 ? control.QuoteBorderThickness().Left : 4;
            bar.Width(borderThickness);
            bar.Background(control.QuoteBorderBrush());
            bar.SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(0));
            bar.VerticalAlignment(winrt::VerticalAlignment::Stretch);
            bar.Margin(control.QuoteBarMargin());
            grid.Children().Append(bar);

            winrt::Grid rightGrid;
            rightGrid.Padding(control.QuotePadding());
            rightGrid.Background(control.QuoteBackground());
            rightGrid.CornerRadius(control.QuoteCornerRadius());
            rightGrid.Children().Append(_flowDocument.RichTextBlock());
            _flowDocument.RichTextBlock().Foreground(control.QuoteForeground());

            rightGrid.SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(1));
            grid.Children().Append(rightGrid);
            grid.Margin(control.QuoteMargin());

            inlineUIContainer.Child(grid);

            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}


