// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdFlowDocument.h"

namespace winrt 
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdTableCell final : public IAddChild
    {
    private:
        winrt::Paragraph _paragraph;
        MdFlowDocument _flowDocument;
        bool _isHeader;
        int _columnIndex;
        int _rowIndex;
        winrt::Grid _container;

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        winrt::Grid Container() const
        {
            return _container;
        }

        int ColumnSpan() const
        {
            return 1; // _tableCell.ColumnSpan;
        }

        int RowSpan() const
        {
            return 1; //_tableCell.RowSpan;
        }

        int ColumnIndex() const
        {
            return _columnIndex;
        }

        int RowIndex() const
        {
            return _rowIndex;
        }

        MdTableCell(winrt::TextAlignment textAlignment, bool isHeader, int columnIndex, int rowIndex, MarkdownThemes const& themes)
        {
            _isHeader = isHeader;
            _columnIndex = columnIndex;
            _rowIndex = rowIndex;

            _flowDocument.RichTextBlock().TextWrapping(TextWrapping::Wrap);
            _flowDocument.RichTextBlock().TextAlignment(textAlignment);
            _flowDocument.RichTextBlock().HorizontalTextAlignment(textAlignment);
            _flowDocument.RichTextBlock().HorizontalAlignment([&]
            {  
                switch (textAlignment)
                {
                    case winrt::TextAlignment::Left:
                        return HorizontalAlignment::Left;
                    case winrt::TextAlignment::Center:
                        return HorizontalAlignment::Center;
                    case winrt::TextAlignment::Right:
                        return winrt::HorizontalAlignment::Right;
                    default:
                        return winrt::HorizontalAlignment::Left;
                }
            }());

            // Use themed table cell padding
            _container.Padding(themes.TableCellPadding());
            
            if (_isHeader)
            {
                _flowDocument.RichTextBlock().FontWeight(winrt::Microsoft::UI::Text::FontWeights::Bold());
            }
            _flowDocument.RichTextBlock().HorizontalAlignment([&]
            {
                switch (textAlignment)
                {
                    case winrt::TextAlignment::Left:
                        return winrt::HorizontalAlignment::Left;
                    case winrt::TextAlignment::Center:
                        return winrt::HorizontalAlignment::Center;
                    case winrt::TextAlignment::Right:
                        return winrt::HorizontalAlignment::Right;
                    default:
                        return winrt::HorizontalAlignment::Left;
                }
            }());
            _container.Children().Append(_flowDocument.RichTextBlock());
        }

        void AddChild(IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}
