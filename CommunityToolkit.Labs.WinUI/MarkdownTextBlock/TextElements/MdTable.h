// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdTableCell.h"
#include "MdTableUIElement.h"

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace winrt::Microsoft::UI;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    class MdTable final : public IAddChild
    {
        Paragraph _paragraph;
        MdTableUIElement _tableElement{nullptr};

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdTable(int columnCount, int headRowCount, int bodyRowCount, MarkdownThemes const& themes)
        {
            if (!themes.TableBorderBrush()) 
            {
                themes.TableBorderBrush(themes.BorderBrush());
            }

            _tableElement = winrt::make<winrt::CommunityToolkit::Labs::WinUI::implementation::MdTableUIElement>(
                columnCount,
                headRowCount + bodyRowCount,
                themes.TableBorderThickness(), 
                themes.TableBorderBrush(),
                themes.TableHeadingBackground(),
                themes.CornerRadius(),
                themes.TableMargin()
            );

            InlineUIContainer inlineUIContainer;
            inlineUIContainer.Child(_tableElement);
            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto cellChild = dynamic_cast<const MdTableCell*>(child))
            {
                auto cell = cellChild->Container();

                Grid::SetColumn(cell, cellChild->ColumnIndex());
                Grid::SetRow(cell, cellChild->RowIndex());
                Grid::SetColumnSpan(cell, cellChild->ColumnSpan());
                Grid::SetRowSpan(cell, cellChild->RowSpan());

                _tableElement.Children().Append(cell);
            }
        }
    };
}

