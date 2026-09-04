#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <limits>
#include <numeric>
#endif
#include "MdTableUIElement.h"
#if __has_include("MdTableUIElement.g.cpp")
#include "MdTableUIElement.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    MdTableUIElement::MdTableUIElement(int columnCount, int rowCount, float borderThickness, winrt::Brush const& borderBrush, winrt::Brush const& headingBrush, winrt::CornerRadius const& cornerRadius, winrt::Thickness const& tableMargin)
    {
        _columnCount = columnCount;
        _rowCount = rowCount;
        _borderThickness = borderThickness;
        Margin(tableMargin);
        auto children = Children();
        {
            winrt::Border border;
            border.Background(headingBrush);
            border.CornerRadius({ cornerRadius.TopLeft, cornerRadius.TopRight, 0, 0 });
            children.Append(border);
        }

        {
            winrt::Border border;
            border.BorderThickness(winrt::Thickness(_borderThickness, _borderThickness, _borderThickness, _borderThickness));
            border.CornerRadius(cornerRadius);
            border.BorderBrush(borderBrush);
            children.Append(border);
        }

        for (int col = 1; col < columnCount; col++)
        {
            winrt::Rectangle rect;
            rect.Fill(borderBrush);
            children.Append(rect);
        }

        for (int row = 1; row < rowCount; row++)
        {
            winrt::Rectangle rect;
            rect.Fill(borderBrush);
            children.Append(rect);
        }
    }

    // Helper method to enumerate FrameworkElements instead of UIElements.
    std::generator<winrt::FrameworkElement> MdTableUIElement::ContentChildren() const
    {
        const auto children = Children();
        for (uint32_t i = _columnCount + _rowCount; i < children.Size(); i++)
        {
            co_yield children.GetAt(i).try_as<winrt::FrameworkElement>();
        }
    }

    // Helper method to get table vertical edges.
    std::generator<winrt::Rectangle> MdTableUIElement::VerticalLines() const
    {
        const auto children = Children();
        for (int i = 2; i < _columnCount + 1; i++)
        {
            co_yield children.GetAt(i).try_as<winrt::Rectangle>();
        }
    }

    // Helper method to get table horizontal edges.
    std::generator<winrt::Rectangle> MdTableUIElement::HorizontalLines() const
    {
        const auto children = Children();
        for (int i = _columnCount + 1; i < _columnCount + _rowCount; i++)
        {
            co_yield children.GetAt(i).try_as<winrt::Rectangle>();
        }
    }

    winrt::Size MdTableUIElement::MeasureOverride(winrt::Size availableSize)
    {
        // Measure the width of each column, with no horizontal width restrictions.
        std::vector<float> naturalColumnWidths;
        naturalColumnWidths.resize(_columnCount);
        for (const auto& child : ContentChildren())
        {
            auto columnIndex = winrt::Grid::GetColumn(child);
            child.Measure(winrt::Size(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
            naturalColumnWidths[columnIndex] = std::max<float>(naturalColumnWidths[columnIndex], child.DesiredSize().Width);
        }

        // Now figure out the actual column widths.
        auto remainingContentWidth = availableSize.Width - ((_columnCount + 1) * _borderThickness);
        _columnWidths.resize(_columnCount);
        int remainingColumnCount = _columnCount;
        while (remainingColumnCount > 0)
        {
            // Calculate the fair width of all columns.
            float fairWidth = std::max<float>(0, remainingContentWidth / remainingColumnCount);

            // Are there any columns less than that?  If so, they get what they are asking for.
            bool recalculationNeeded = false;
            for (int i = 0; i < _columnCount; i++)
            {
                if (_columnWidths[i] == 0 && naturalColumnWidths[i] < fairWidth)
                {
                    _columnWidths[i] = naturalColumnWidths[i];
                    remainingColumnCount--;
                    remainingContentWidth -= _columnWidths[i];
                    recalculationNeeded = true;
                }
            }

            // If there are no columns less than the fair width, every remaining column gets that width.
            if (recalculationNeeded == false)
            {
                for (int i = 0; i < _columnCount; i++)
                {
                    if (_columnWidths[i] == 0)
                    {
                        _columnWidths[i] = fairWidth;
                    }
                }

                break;
            }
        }

        // TODO: we can skip this step if none of the column widths changed, and just re-use
        // the row heights we obtained earlier.

        // Now measure row heights.
        _rowHeights.resize(_rowCount);
        for (const auto& child : ContentChildren())
        {
            auto columnIndex = winrt::Grid::GetColumn(child);
            auto rowIndex = winrt::Grid::GetRow(child);
            child.Measure(winrt::Size(_columnWidths[columnIndex], std::numeric_limits<float>::infinity()));
            _rowHeights[rowIndex] = std::max<float>(_rowHeights[rowIndex], child.DesiredSize().Height);
        }

        return winrt::Size(
            std::reduce(_columnWidths.begin(), _columnWidths.end()) + (_borderThickness * (_columnCount + 1)),
            std::reduce(_rowHeights.begin(), _rowHeights.end()) + ((_rowCount + 1) * _borderThickness));
    }

    winrt::Size MdTableUIElement::ArrangeOverride(winrt::Size finalSize)
    {
        if (_columnWidths.empty() || _rowHeights.empty())
        {
            throw winrt::hresult_illegal_method_call(L"Expected Measure to be called first.");
        }

        auto children = Children();

        // Arrange content.
        for (const auto& child : ContentChildren())
        {
            auto columnIndex = winrt::Grid::GetColumn(child);
            auto rowIndex = winrt::Grid::GetRow(child);

            winrt::Rect rect(_borderThickness, 0, 0, 0);

            for (int col = 0; col < columnIndex; col++)
            {
                rect.X += _borderThickness + _columnWidths[col];
            }

            rect.Y = _borderThickness;
            for (int row = 0; row < rowIndex; row++)
            {
                rect.Y += _borderThickness + _rowHeights[row];
            }

            rect.Width = _columnWidths[columnIndex];
            rect.Height = _rowHeights[rowIndex];
            child.Arrange(rect);
        }

        // Arrange vertical border elements.
        {
            int colIndex = 0;
            float x = 0;
            for (const auto& borderLine : VerticalLines())
            {
                x += _borderThickness + _columnWidths[colIndex];
                borderLine.Arrange(winrt::Rect(x, 0, _borderThickness, finalSize.Height));
                if (colIndex >= static_cast<int>(_columnWidths.size()))
                {
                    break;
                }

                colIndex++;
            }
        }

        // Arrange horizontal border elements.
        {
            children.GetAt(0).Arrange(winrt::Rect(0, 0, finalSize.Width, _rowHeights[0] + (_borderThickness * 2)));
            children.GetAt(1).Arrange(winrt::Rect(0, 0, finalSize.Width, finalSize.Height));
            int rowIndex = 0;
            float y = 0;
            for (const auto& borderLine : HorizontalLines())
            {
                y += _borderThickness + _rowHeights[rowIndex];
                borderLine.Arrange(winrt::Rect(0, y, finalSize.Width, _borderThickness));
                if (rowIndex >= static_cast<int>(_rowHeights.size()))
                {
                    break;
                }

                rowIndex++;
            }
        }

        return finalSize;
    }
}
