#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <ranges>
#include <vector>
#endif
#include "DataTable.h"
#include "DataColumn.h"
#if __has_include("DataTable.g.cpp")
#include "DataTable.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    bool DataTable::IsAnyColumnAuto()
    {
        auto children = Children();
        return std::any_of(children.begin(), children.end(), [](auto&& e)
        {
            if (auto column = e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>()) 
            {
                auto columnImpl = winrt::get_self<implementation::DataColumn>(column);
                return columnImpl->CurrentWidth().GridUnitType == winrt::GridUnitType::Auto;
            }

            return false;
        });
    }

	std::set<winrt::XamlToolkit::Labs::WinUI::DataRow>& DataTable::Rows() { return _rows; }

    void DataTable::ColumnResized()
    {
        InvalidateArrange();

        for (const auto& row : Rows())
        {
            row.InvalidateArrange();
        }
    }

    bool DataTable::ColumnSpacing() const { return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty())); }
    void DataTable::ColumnSpacing(double value) { SetValue(ColumnSpacingProperty(), winrt::box_value(value)); }

    const wil::single_threaded_property<winrt::DependencyProperty> DataTable::ColumnSpacingProperty =
        winrt::DependencyProperty::Register(
            L"ColumnSpacing", 
            winrt::xaml_typename<double>(), 
            winrt::xaml_typename<class_type>(), 
            winrt::PropertyMetadata(winrt::box_value(0.0)));

    winrt::Size DataTable::MeasureOverride(winrt::Size availableSize)
    {
        double fixedWidth = 0;
        double proportionalUnits = 0;
        double autoSized = 0;
        double desiredWidth = 0;

        double maxHeight = 0;

        auto elements = Children()
            | std::ranges::views::filter([](auto&& e) { return e.Visibility() == winrt::Visibility::Visible && e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::views::transform([](auto&& e) { return e.template as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::to<std::vector>();

        // We only need to measure elements that are visible
        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<implementation::DataColumn>(column);
            auto currentWidth = columnImpl->CurrentWidth();

            if (winrt::GridLengthHelper::GetIsStar(currentWidth))
            {
                proportionalUnits += currentWidth.Value;
            }
            else if (winrt::GridLengthHelper::GetIsAbsolute(currentWidth))
            {
                fixedWidth += currentWidth.Value;
            }
        }

        // Add in spacing between columns to our fixed size allotment
        fixedWidth += (elements.size() - 1) * ColumnSpacing();

        // TODO: Handle infinite width?
        auto proportionalAmount = proportionalUnits > 0 ? (availableSize.Width - fixedWidth) / proportionalUnits : 0;

        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<implementation::DataColumn>(column);
            auto currentWidth = columnImpl->CurrentWidth();

            if (winrt::GridLengthHelper::GetIsStar(currentWidth))
            {
                auto width = proportionalAmount * currentWidth.Value;

                column.Measure(winrt::Size(static_cast<float>(width), availableSize.Height));

                desiredWidth += width;
            }
            else if (winrt::GridLengthHelper::GetIsAbsolute(currentWidth))
            {
                column.Measure(winrt::Size(static_cast<float>(currentWidth.Value), availableSize.Height));

                desiredWidth += currentWidth.Value;
            }
            else
            {
                // TODO: Technically this is using 'Auto' on the Header content
                // What the developer probably intends is it to be adjusted based on the contents of the rows...
                // To enable this scenario, we'll need to actually measure the contents of the rows for that column
                // in DataRow and figure out the maximum size to report back and adjust here in some sort of hand-shake
                // for the layout process... (i.e. get the data in the measure step, use it in the arrange step here,
                // then invalidate the child arranges [don't re-measure and cause loop]...)

                // For now, we'll just use the header content as a guideline to see if things work.

                // Avoid negative values when columns don't fit `availableSize`. Otherwise the `Size` constructor will throw.
                column.Measure(winrt::Size(std::max<float>(static_cast<float>(availableSize.Width - fixedWidth - autoSized), 0), availableSize.Height));

                // Keep track of already 'allotted' space, use either the maximum child size (if we know it) or the header content
                auto width = std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);

                autoSized += width;
                desiredWidth += width;
            }

            maxHeight = std::max<double>(maxHeight, column.DesiredSize().Height);
        }

        if (proportionalUnits > 0)
        {
            // Star columns consume the available width.
            desiredWidth = availableSize.Width;
        }
        else
        {
            desiredWidth += (elements.size() - 1) * ColumnSpacing();
        }

        return winrt::Size(static_cast<float>(desiredWidth), static_cast<float>(maxHeight));
    }

    winrt::Size DataTable::ArrangeOverride(winrt::Size finalSize)
    {
        double fixedWidth = 0;
        double proportionalUnits = 0;
        double autoSized = 0;

        auto elements = Children()
            | std::ranges::views::filter([](auto&& e) { return e.Visibility() == winrt::Visibility::Visible && e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::views::transform([](auto&& e) { return e.template as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); });

        // We only need to measure elements that are visible
        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<implementation::DataColumn>(column);
			auto currentWidth = columnImpl->CurrentWidth();
            if (winrt::GridLengthHelper::GetIsStar(currentWidth))
            {
                proportionalUnits += currentWidth.Value;
            }
            else if (winrt::GridLengthHelper::GetIsAbsolute(currentWidth))
            {
                fixedWidth += currentWidth.Value;
            }
            else
            {
                autoSized += std::max<double>(columnImpl->DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
            }
        }

        // TODO: Handle infinite width?
        // TODO: This can go out of bounds or something around here when pushing a resized column to the right...
        auto proportionalAmount = (finalSize.Width - fixedWidth - autoSized) / proportionalUnits;

        double width = 0;
        double x = 0;

        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<implementation::DataColumn>(column);
			auto currentWidth = columnImpl->CurrentWidth();
            if (winrt::GridLengthHelper::GetIsStar(currentWidth))
            {
                width = proportionalAmount * currentWidth.Value;
                column.Arrange(winrt::Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }
            else if (winrt::GridLengthHelper::GetIsAbsolute(currentWidth))
            {
                width = currentWidth.Value;
                column.Arrange(winrt::Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }
            else
            {
                // TODO: We use the comparison of sizes a lot, should we cache in the DataColumn itself?
                width = std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
                column.Arrange(winrt::Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }

            x += width + ColumnSpacing();
        }

        return finalSize;
    }
}
