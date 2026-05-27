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
        Paragraph _paragraph;
        MdFlowDocument _flowDocument;
        bool _isHeader;
        int _columnIndex;
        int _rowIndex;
        Grid _container;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        Grid Container() const
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

        MdTableCell(TextAlignment textAlignment, bool isHeader, int columnIndex, int rowIndex, MarkdownThemes const& themes)
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
                    case TextAlignment::Left:
                        return HorizontalAlignment::Left;
                    case TextAlignment::Center:
                        return HorizontalAlignment::Center;
                    case TextAlignment::Right:
                        return HorizontalAlignment::Right;
                    default:
                        return HorizontalAlignment::Left;
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
                    case
                    TextAlignment::Left:
                        return HorizontalAlignment::Left;
                    case TextAlignment::Center:
                        return HorizontalAlignment::Center;
                    case TextAlignment::Right:
                        return HorizontalAlignment::Right;
                    default:
                        return HorizontalAlignment::Left;
                }
            }());
            _container.Children().Append(_flowDocument.RichTextBlock());
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}
