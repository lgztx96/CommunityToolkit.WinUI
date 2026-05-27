#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <ranges>
#include <span>
#endif
#include "WrapPanel2.h"
#if __has_include("WrapPanel2.g.cpp")
#include "WrapPanel2.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    WrapPanel2::WrapPanel2()
    {
        RegisterPropertyChangedCallback(FrameworkElement::HorizontalAlignmentProperty(), &WrapPanel2::OnAlignmentPropertyChanged);
        RegisterPropertyChangedCallback(FrameworkElement::VerticalAlignmentProperty(), &WrapPanel2::OnAlignmentPropertyChanged);
    }

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::LayoutLengthProperty =
        DependencyProperty::RegisterAttached(
            L"LayoutLength",
            winrt::xaml_typename<GridLength>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(GridLengthHelper::Auto()) });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::OrientationProperty =
        DependencyProperty::Register(
            L"Orientation",
            winrt::xaml_typename<Controls::Orientation>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(Controls::Orientation::Horizontal), &WrapPanel2::OnPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::ItemSpacingProperty =
        DependencyProperty::Register(
            L"ItemSpacing",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(0.0), &WrapPanel2::OnPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::LineSpacingProperty =
        DependencyProperty::Register(
            L"LineSpacing",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(0.0), &WrapPanel2::OnPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::ItemsJustificationProperty =
        DependencyProperty::Register(
            L"ItemsJustification",
            winrt::xaml_typename<WrapPanelItemsJustification>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(WrapPanelItemsJustification::Start), &WrapPanel2::OnPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> WrapPanel2::ItemsStretchProperty =
        DependencyProperty::Register(
            L"ItemsStretch",
            winrt::xaml_typename<WrapPanelItemsStretch>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata{ winrt::box_value(WrapPanelItemsStretch::None), &WrapPanel2::OnPropertyChanged });

    void WrapPanel2::OnPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        auto panel = d.as<class_type>();
        panel.InvalidateMeasure();
    }

    void WrapPanel2::OnAlignmentPropertyChanged(DependencyObject const& obj, [[maybe_unused]] DependencyProperty const& prop)
    {
        auto panel = obj.as<class_type>();
        panel.InvalidateMeasure();
    }

    Size WrapPanel2::MeasureOverride(Size availableSize)
    {
        _rowSpecs.clear();
        _longestRowSize = 0;
		auto itemSpacing = ItemSpacing();
		auto itemStretch = ItemsStretch();
		auto realJustification = RealJustification();

        // Define XY/UV coordinate variables
        UVCoord uvAvailableSize(availableSize.Width, availableSize.Height, Orientation());

        RowSpec currentRowSpec;

		auto children = Children();

        auto elements = children
            | std::views::filter([](const auto& e)
            {
                return e.Visibility() == winrt::Microsoft::UI::Xaml::Visibility::Visible;
            })
            | std::views::transform([](const auto& item)
            {
                return item.template as<FrameworkElement>();
            });

        // Do nothing if the panel is empty
        if (elements.empty())
        {
            return Size(0, 0);
        }

        for (const auto& child : elements)
        {
            // Measure the child's desired size and get layout
            child.Measure(availableSize);
            UVCoord uvDesiredSize(child.DesiredSize(), Orientation());
            auto layoutLength = GetLayoutLength(child);

            // Attempt to add the child to the current row/column
             RowSpec spec(layoutLength, uvDesiredSize);
            if (!currentRowSpec.TryAdd(spec, itemSpacing, uvAvailableSize.U(), itemStretch, realJustification))
            {
                // Could not add to current row/column
                // Start a new row/column
                _rowSpecs.push_back(currentRowSpec);
                auto newSize = currentRowSpec.Measure(itemSpacing, itemStretch, realJustification);
                _longestRowSize = std::max<double>(_longestRowSize, newSize);
                currentRowSpec = spec;
            }
        }

        // Add the final row/column
        _rowSpecs.push_back(currentRowSpec);
        _longestRowSize = std::max<double>(_longestRowSize, currentRowSpec.Measure(itemSpacing, itemStretch, realJustification));

        // Calculate final desired size
        UVCoord uvSize(0, 0, Orientation());
        
        uvSize.U(IsMainAxisStretch(uvAvailableSize.U()) ? uvAvailableSize.U() : _longestRowSize);
        double vTotal = 0.0;

        for (auto const& rs : _rowSpecs)
        {
            vTotal += rs.MaxOffAxisSize;
        }

        auto count = static_cast<int>(_rowSpecs.size());
        if (count > 1)
        {
            vTotal += LineSpacing() * (count - 1);
        }

        uvSize.V(vTotal);
        
        // Clamp to available size and return
        uvSize.U(std::min<double>(uvSize.U(), uvAvailableSize.U()));
        uvSize.V(std::min<double>(uvSize.V(), uvAvailableSize.V()));
        return uvSize.ToSize();
    }

    Size WrapPanel2::ArrangeOverride(Size finalSize)
    {
        // Do nothing if there are no rows/columns
        if (_rowSpecs.empty())
            return Size(0, 0);

        // Create XY/UV coordinate variables
		auto orientation = Orientation();
        UVCoord pos(0, 0, orientation);
        UVCoord uvFinalSize(finalSize, orientation);

        // Adjust the starting position based on off-axis alignment
        double contentHeight = 0.0;
        for (const auto& row : _rowSpecs)
        {
            contentHeight += row.MaxOffAxisSize;
        }

        int rowCount = static_cast<int>(_rowSpecs.size());
        if (rowCount > 1)
        {
            contentHeight += LineSpacing() * (rowCount - 1);
        }

        pos.V(GetStartByAlignment(GetOffAlignment(), contentHeight, uvFinalSize.V()));

		auto children = Children();
        auto childQueue = children 
        | std::views::filter([](const auto& e)
        {
            return e.Visibility() == winrt::Microsoft::UI::Xaml::Visibility::Visible;
        })
        | std::views::transform([](const auto& item)
        {
            return item.template as<FrameworkElement>();
        })
        | std::ranges::to<std::vector>();

        size_t cursor = 0;
        for (const auto& row : _rowSpecs)
        {
            // Arrange the row/column
            ArrangeRow(pos, row, uvFinalSize, childQueue, cursor);
        }

        // "Arrange" remaning children by rendering them with zero size
        for (const auto& child : std::span(childQueue).subspan(cursor))
        {
            // Arrange with zero size
            child.Arrange(winrt::Windows::Foundation::Rect{ 0, 0, 0, 0 });
        }

        return finalSize;
    }

    void WrapPanel2::ArrangeRow(UVCoord& pos, RowSpec const& row, UVCoord const& uvFinalSize, std::vector<FrameworkElement> const& childQueue, size_t& cursor)
    {
		auto itemSpacing = ItemSpacing();
		auto realJustification = RealJustification();
		auto justifiedSpacing = JustifiedSpacing();
        auto spacingTotalSize = GetTotalSpacing(row.ItemsCount, itemSpacing, realJustification);
        auto remainingSpace = uvFinalSize.U() - row.ReservedSpace - spacingTotalSize;
        auto portionSize = row.MinPortionSize;

        // Determine if the desired alignment is stretched.
        // Or if fixed row lengths are in use.
        bool stretch = IsMainAxisStretch(uvFinalSize.U()) || justifiedSpacing;

        // Calculate portion size if stretching
        // Same logic applies for matching row lengths, since the size was determined during measure
        if (stretch)
        {
            portionSize = remainingSpace / row.PortionsSum;
        }

        // Reset the starting U position
        pos.U(0);

        // Set a flag for if the row is being forced to stretch
        // Also declare a variable for the effective items spacing. This will be adjusted if needed for justification.
        bool forceStretch = justifiedSpacing && row.PortionsSum == 0 && ItemsStretch() != WrapPanelItemsStretch::None;

        // Setup portionSize for forced stretching
        if (forceStretch)
        {
            switch (ItemsStretch())
            {
                // The first child's size will be overridden to 1*
                // Change portion size to fill remaining space plus its original size
            case WrapPanelItemsStretch::First:
                portionSize = remainingSpace + GetChildSize(childQueue[cursor]);
                break;

                // The last child's size will be overridden to 1*
                // Change portion size to fill remaining space plus its original size
            case WrapPanelItemsStretch::Last:
                portionSize = remainingSpace + GetChildSize(childQueue[cursor + row.ItemsCount - 1]);
                break;

                // All children's sizes will be overridden to 1*
                // Change portion size to evenly distribute remaining space
            case WrapPanelItemsStretch::Equal:
                portionSize = (uvFinalSize.U() - spacingTotalSize) / row.ItemsCount;
                break;

                // All children's sizes will be overridden to star sizes proportional to their original size
                // Change portion size to distribute remaining space proportionally
            case WrapPanelItemsStretch::Proportional:
                portionSize = (uvFinalSize.U() - spacingTotalSize) / row.ReservedSpace;
                break;

                // Default case (should not be hit)
            default:
                portionSize = row.MinPortionSize;
                break;
            }
        }
        else if (justifiedSpacing && row.PortionsSum == 0)
        {
            // If Item Justification is enabled and there's no proportional
            // Adjust the spacing between items according to the justification mode.
            auto divisbleSpace = remainingSpace + spacingTotalSize;
            switch (realJustification)
            {
            case WrapPanelItemsJustification::SpaceBetween:
                itemSpacing = divisbleSpace / (row.ItemsCount - 1);
                break;

            case WrapPanelItemsJustification::SpaceAround:
                itemSpacing = divisbleSpace / row.ItemsCount;
                break;

            case WrapPanelItemsJustification::SpaceEvenly:
                itemSpacing = divisbleSpace / (row.ItemsCount + 1);
                break;

            default:
                itemSpacing = divisbleSpace / row.ItemsCount;
                break;
            }
        }

        // Adjust the starting position
        auto rowSize = row.Measure(itemSpacing, ItemsStretch(), realJustification);
        switch (realJustification)
        {
        case WrapPanelItemsJustification::SpaceAround:
            pos.U(itemSpacing * 0.5);
            break;

        case WrapPanelItemsJustification::SpaceBetween:
            pos.U(0.0);
            break;

        case WrapPanelItemsJustification::SpaceEvenly:
            pos.U(itemSpacing);
            break;

        case WrapPanelItemsJustification::Start:
        case WrapPanelItemsJustification::Center:
        case WrapPanelItemsJustification::End:
        default:
            pos.U(GetStartByAlignment(
                GetJustificationAlignment(),
                rowSize,
                uvFinalSize.U()));
            break;
        }

        // Arrange each child in the row/column
        for (int i = 0; i < row.ItemsCount; i++)
        {
            // Get the next child
            if (cursor >= childQueue.size())
                return;

            auto const& child = childQueue[cursor++];

            // Sanity check
            if (!child)
                return;

            // Determine the child's size
            auto size = GetChildSize(child, i, row, portionSize, forceStretch);

            // NOTE: The arrange method is still in X/Y coordinate system
            child.Arrange(winrt::Windows::Foundation::Rect{
                static_cast<float>(pos.X),
                static_cast<float>(pos.Y),
                static_cast<float>(size.X),
                static_cast<float>(size.Y)
                });

            // Advance the position
            pos.U(pos.U() + size.U() + itemSpacing);
        }

        // Advance to the next row/column
        pos.V(pos.V() + row.MaxOffAxisSize + LineSpacing());
    }

    WrapPanel2::UVCoord WrapPanel2::GetChildSize(FrameworkElement const& child, int indexInRow, RowSpec const& row, double portionSize, bool forceStretch)
    {
        // Get layout and desired size
        auto layoutLength = GetLayoutLength(child);
        UVCoord uvDesiredSize(child.DesiredSize(), Orientation());

        // Override the layout based on the forced stretch method if necessary
        if (forceStretch)
        {
            GridLength oneStar(1, GridUnitType::Star);
            auto stretch = ItemsStretch();

            if (stretch == WrapPanelItemsStretch::First && indexInRow == 0)
            {
                // Override the first item's layout to 1*
                layoutLength = oneStar;
            }
            else if (stretch == WrapPanelItemsStretch::Last && indexInRow == (row.ItemsCount - 1))
            {
                // Override the last item's layout to 1*
                layoutLength = oneStar;
            }
            else if (stretch == WrapPanelItemsStretch::Equal)
            {
                // Override all item's layouts to 1*
                layoutLength = oneStar;
            }
            else if (stretch == WrapPanelItemsStretch::Proportional)
            {
                // Override all item's layouts to star sizes proportional to their original size
                switch (layoutLength.GridUnitType)
                {
                case GridUnitType::Auto:
                    layoutLength = GridLength(uvDesiredSize.U(), GridUnitType::Star);
                    break;

                case GridUnitType::Pixel:
                default:
                    layoutLength = GridLength(layoutLength.Value, GridUnitType::Star);
                    break;
                }
            }
        }

        // Determine the child's U size
        double uSize = 0.0;

        switch (layoutLength.GridUnitType)
        {
        case GridUnitType::Auto:
            uSize = uvDesiredSize.U();
            break;

        case GridUnitType::Pixel:
            uSize = layoutLength.Value;
            break;

        case GridUnitType::Star:
            uSize = layoutLength.Value * portionSize;
            break;

        default:
            uSize = uvDesiredSize.U();
            break;
        }

        // Return the final size
        UVCoord value(0, 0, Orientation());
        value.U(uSize);
        value.V(row.MaxOffAxisSize);
        return value;
    }

    double WrapPanel2::GetStartByAlignment(Alignment alignment, double size, double availableSize)
    {
        switch (alignment)
        {
        case Alignment::Start:
            return 0.0;

        case Alignment::Center:
            return (availableSize * 0.5) - (size * 0.5);

        case Alignment::End:
            return availableSize - size;

        default:
            return 0.0;
        }
    }

    WrapPanel2::Alignment WrapPanel2::GetAlignment()
    {
        switch (Orientation())
        {
        case Orientation::Horizontal:
            switch (HorizontalAlignment())
            {
            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left:
                return Alignment::Start;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center:
                return Alignment::Center;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Right:
                return Alignment::End;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch:
                return Alignment::Stretch;

            default:
                return Alignment::Start;
            }

        case Orientation::Vertical:
            switch (VerticalAlignment())
            {
            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Top:
                return Alignment::Start;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Center:
                return Alignment::Center;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom:
                return Alignment::End;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch:
                return Alignment::Stretch;

            default:
                return Alignment::Start;
            }

        default:
            return Alignment::Start;
        }
    }

    WrapPanel2::Alignment WrapPanel2::GetOffAlignment()
    {
        switch (Orientation())
        {
        case Orientation::Horizontal:
            switch (VerticalAlignment())
            {
            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Top:
                return Alignment::Start;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Center:
                return Alignment::Center;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom:
                return Alignment::End;

            case winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch:
                return Alignment::Stretch;

            default:
                return Alignment::Start;
            }

        case Orientation::Vertical:
            switch (HorizontalAlignment())
            {
            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left:
                return Alignment::Start;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center:
                return Alignment::Center;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Right:
                return Alignment::End;

            case winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch:
                return Alignment::Stretch;

            default:
                return Alignment::Start;
            }

        default:
            return Alignment::Start;
        }
    }

    WrapPanel2::Alignment WrapPanel2::GetJustificationAlignment()
    {
        switch (RealJustification())
        {
        case WrapPanelItemsJustification::Start:
            return Alignment::Start;

        case WrapPanelItemsJustification::Center:
            return Alignment::Center;

        case WrapPanelItemsJustification::End:
            return Alignment::End;

        default:
            return Alignment::Stretch;
        }
    }

    bool WrapPanel2::IsMainAxisStretch(double availableSize)
    {
        return GetAlignment() == Alignment::Stretch && !std::isinf(availableSize);
    }

    bool WrapPanel2::JustifiedSpacing()
    {
        return IsSpacingJustified(RealJustification());
    }

    WrapPanelItemsJustification WrapPanel2::RealJustification()
    {
        if (ItemsJustification() == WrapPanelItemsJustification::Automatic)
        {
            switch (GetAlignment())
            {
            case Alignment::Start:
                return WrapPanelItemsJustification::Start;

            case Alignment::Center:
                return WrapPanelItemsJustification::Center;

            case Alignment::End:
                return WrapPanelItemsJustification::End;

            default:
                return WrapPanelItemsJustification::SpaceBetween;
            }
        }

        return ItemsJustification();
    }

    bool WrapPanel2::IsSpacingJustified(WrapPanelItemsJustification justification)
    {
        switch (justification)
        {
        case WrapPanelItemsJustification::Start:
        case WrapPanelItemsJustification::Center:
        case WrapPanelItemsJustification::End:
            return false;

        case WrapPanelItemsJustification::SpaceAround:
        case WrapPanelItemsJustification::SpaceBetween:
        case WrapPanelItemsJustification::SpaceEvenly:
            return true;

        default:
            return false;
        }
    }

    double WrapPanel2::GetTotalSpacing(int itemCount, double itemsSpacing, WrapPanelItemsJustification justification)
    {
        switch (justification)
        {
        case WrapPanelItemsJustification::SpaceAround:
            return itemCount * itemsSpacing;

        case WrapPanelItemsJustification::SpaceEvenly:
            return (itemCount + 1) * itemsSpacing;

        case WrapPanelItemsJustification::Start:
        case WrapPanelItemsJustification::Center:
        case WrapPanelItemsJustification::End:
        case WrapPanelItemsJustification::SpaceBetween:
        default:
            return (itemCount - 1) * itemsSpacing;
        }
    }

    double WrapPanel2::GetChildSize(winrt::Microsoft::UI::Xaml::FrameworkElement const& child)
    {
        auto childLayout = GetLayoutLength(child);

        switch (childLayout.GridUnitType)
        {
        case GridUnitType::Auto:
        {
            UVCoord uv(child.DesiredSize(), Orientation());
            return uv.U();
        }

        case GridUnitType::Pixel:
            return childLayout.Value;

        default:
            return 0.0;
        }
    }
}
