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
                if (auto column = e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>()) {
                    auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
                    return columnImpl->CurrentWidth().GridUnitType == GridUnitType::Auto;
                }

                return false;
            });
    }

	std::set<winrt::XamlToolkit::Labs::WinUI::DataRow>& DataTable::Rows() { return _rows; }

    void DataTable::ColumnResized()
    {
        InvalidateArrange();

        for (auto& row : Rows())
        {
            row.InvalidateArrange();
        }
    }

    bool DataTable::ColumnSpacing() const { return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty)); }
    void DataTable::ColumnSpacing(double value) { SetValue(ColumnSpacingProperty, winrt::box_value(value)); }

    const wil::single_threaded_property<DependencyProperty> DataTable::ColumnSpacingProperty =
        DependencyProperty::Register(L"ColumnSpacing", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0)));

    Size DataTable::MeasureOverride(Size availableSize)
    {
        double fixedWidth = 0;
        double proportionalUnits = 0;
        double autoSized = 0;

        double maxHeight = 0;

        auto elements = Children()
            | std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible && e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::views::transform([](auto&& e) { return e.template as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::to<std::vector>();

        // We only need to measure elements that are visible
        for (const auto& column : elements)
        {
			auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
            if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
            {
                proportionalUnits += columnImpl->DesiredWidth().Value;
            }
            else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
            {
                fixedWidth += columnImpl->DesiredWidth().Value;
            }
        }

        // Add in spacing between columns to our fixed size allotment
        fixedWidth += (elements.size() - 1) * ColumnSpacing();

        // TODO: Handle infinite width?
        auto proportionalAmount = (availableSize.Width - fixedWidth) / proportionalUnits;

        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
            if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
            {
                column.Measure(Size(static_cast<float>(proportionalAmount * columnImpl->CurrentWidth().Value), availableSize.Height));
            }
            else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
            {
                column.Measure(Size(static_cast<float>(columnImpl->CurrentWidth().Value), availableSize.Height));
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
                column.Measure(Size(std::max<float>(static_cast<float>(availableSize.Width - fixedWidth - autoSized), 0), availableSize.Height));

                // Keep track of already 'allotted' space, use either the maximum child size (if we know it) or the header content
                autoSized += std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
            }

            maxHeight = std::max<double>(maxHeight, column.DesiredSize().Height);
        }

        return Size(availableSize.Width, static_cast<float>(maxHeight));
    }

    Size DataTable::ArrangeOverride(Size finalSize)
    {
        double fixedWidth = 0;
        double proportionalUnits = 0;
        double autoSized = 0;

        auto elements = Children()
            | std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible && e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
            | std::ranges::views::transform([](auto&& e) { return e.template as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); });

        // We only need to measure elements that are visible
        for (const auto& column : elements)
        {
            auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
            if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
            {
                proportionalUnits += columnImpl->CurrentWidth().Value;
            }
            else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
            {
                fixedWidth += columnImpl->CurrentWidth().Value;
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
            auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
            if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
            {
                width = proportionalAmount * columnImpl->CurrentWidth().Value;
                column.Arrange(Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }
            else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
            {
                width = columnImpl->CurrentWidth().Value;
                column.Arrange(Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }
            else
            {
                // TODO: We use the comparison of sizes a lot, should we cache in the DataColumn itself?
                width = std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
                column.Arrange(Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
            }

            x += width + ColumnSpacing();
        }

        return finalSize;
    }
}
