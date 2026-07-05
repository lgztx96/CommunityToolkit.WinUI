#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "WrapPanel.h"
#if __has_include("WrapPanel.g.cpp")
#include "WrapPanel.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> WrapPanel::HorizontalSpacingProperty =
        winrt::DependencyProperty::Register(
            L"HorizontalSpacing",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(0.0), &WrapPanel::LayoutPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> WrapPanel::VerticalSpacingProperty =
        winrt::DependencyProperty::Register(
            L"VerticalSpacing",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(0.0), &WrapPanel::LayoutPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> WrapPanel::OrientationProperty =
        winrt::DependencyProperty::Register(
            L"Orientation",
            winrt::xaml_typename<winrt::Orientation>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::Orientation::Horizontal), &WrapPanel::LayoutPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> WrapPanel::PaddingProperty =
        winrt::DependencyProperty::Register(
            L"Padding",
            winrt::xaml_typename<winrt::Thickness>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::Thickness{ 0,0,0,0 }), &WrapPanel::LayoutPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> WrapPanel::StretchChildProperty =
        winrt::DependencyProperty::Register(
            L"StretchChild",
            winrt::xaml_typename<enum StretchChild>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(StretchChild::None), &WrapPanel::LayoutPropertyChanged));

    void WrapPanel::LayoutPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        if (const auto wp = d.try_as<class_type>())
        {
            wp.InvalidateMeasure();
            wp.InvalidateArrange();
        }
    }

    winrt::Size WrapPanel::MeasureOverride(winrt::Size availableSize)
    {
        auto padding = Padding();
        auto childAvailableSize = winrt::Size(
            availableSize.Width - static_cast<float>(padding.Left) - static_cast<float>(padding.Right),
            availableSize.Height - static_cast<float>(padding.Top) - static_cast<float>(padding.Bottom));

        for (const auto& child : Children())
        {
            child.Measure(childAvailableSize);
        }

        auto requiredSize = UpdateRows(availableSize);
        return requiredSize;
    }

    winrt::Size WrapPanel::ArrangeOverride(winrt::Size finalSize)
    {
        auto orientation = Orientation();
		auto desiredSize = DesiredSize();
        if ((orientation == winrt::Orientation::Horizontal && finalSize.Width < desiredSize.Width) ||
            (orientation == winrt::Orientation::Vertical && finalSize.Height < desiredSize.Height))
        {
            // We haven't received our desired size. We need to refresh the rows.
            UpdateRows(finalSize);
        }

        if (_rows.size() > 0)
        {
            // Now that we have all the data, we do the actual arrange pass
            auto childIndex = 0;
            auto children = Children();
            for (const auto& row : _rows)
            {
                for (const auto& rect : row.ChildrenRects)
                {
                    auto child = children.GetAt(childIndex++);
                    while (child.Visibility() == winrt::Visibility::Collapsed)
                    {
                        // Collapsed children are not added into the rows,
                        // we skip them.
                        child = children.GetAt(childIndex++);
                    }

                    auto arrangeRect = UvRect
                    {
                        .Position = rect.Position,
                        .Size = UvMeasure { rect.Size.U, row.Size.V },
                    };

                    auto finalRect = arrangeRect.ToRect(orientation);
                    child.Arrange(finalRect);
                }
            }
        }

        return finalSize;
    }

    winrt::Size WrapPanel::UpdateRows(winrt::Size availableSize)
    {
        _rows.clear();

        auto padding = Padding();
        auto orientation = Orientation();
        auto children = Children();
        auto paddingStart = UvMeasure(orientation, padding.Left, padding.Top);
        auto paddingEnd = UvMeasure(orientation, padding.Right, padding.Bottom);

        if (children.Size() == 0)
        {
            auto emptySize = paddingStart.Add(paddingEnd).ToSize(orientation);
            return emptySize;
        }

        auto parentMeasure = UvMeasure(orientation, availableSize.Width, availableSize.Height);
        auto spacingMeasure = UvMeasure(orientation, HorizontalSpacing(), VerticalSpacing());
        auto position = UvMeasure(orientation, padding.Left, padding.Top);

        Row currentRow;
        auto finalMeasure = UvMeasure(orientation, 0.0, 0.0);
        auto Arrange = [&](winrt::UIElement const& child, bool isLast = false)
        {
            if (child.Visibility() == winrt::Visibility::Collapsed)
            {
                return; // if an item is collapsed, avoid adding the spacing
            }

            auto desiredMeasure = UvMeasure(orientation, child.DesiredSize());
            if ((desiredMeasure.U + position.U + paddingEnd.U) > parentMeasure.U || position.U >= parentMeasure.U)
            {
                // next row!
                position.U = paddingStart.U;
                position.V += currentRow.Size.V + spacingMeasure.V;

                _rows.emplace_back(currentRow);
                currentRow = Row();
            }

            // Stretch the last item to fill the available space
            // if the parent measure is not infinite
            if (isLast && !std::isinf(parentMeasure.U))
            {
                desiredMeasure.U = parentMeasure.U - position.U;
            }

            currentRow.Add(position, desiredMeasure);

            // adjust the location for the next items
            position.U += desiredMeasure.U + spacingMeasure.U;
            finalMeasure.U = std::max<double>(finalMeasure.U, position.U);
        };

        auto lastIndex = static_cast<int>(children.Size()) - 1;
        for (auto i = 0; i < lastIndex; i++)
        {
            Arrange(children.GetAt(i));
        }

        Arrange(children.GetAt(lastIndex), StretchChild() == StretchChild::Last);
        if (currentRow.ChildrenRects.size() > 0)
        {
            _rows.emplace_back(currentRow);
        }

        if (_rows.empty())
        {
            auto emptySize = paddingStart.Add(paddingEnd).ToSize(orientation);
            return emptySize;
        }

        // Get max V here before computing final rect
        auto lastRowRect = _rows.back().Rect();
        finalMeasure.V = lastRowRect.Position.V + lastRowRect.Size.V;
        auto finalRect = finalMeasure.Add(paddingEnd).ToSize(orientation);
        return finalRect;
    }
}
