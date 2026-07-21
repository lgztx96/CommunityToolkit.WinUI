#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "ResizeThumb.h"
#if __has_include("ResizeThumb.g.cpp")
#include "ResizeThumb.g.cpp"
#endif
#include "../XamlToolkit.WinUI/common.h"
#include "TargetControlResizedEventArgs.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    ResizeThumb::ResizeThumb()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        Loaded({ this, &ResizeThumb::ResizeThumb_Loaded });
    }

    void ResizeThumb::ResizeThumb_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (TargetControl() == nullptr)
        {
            TargetControl(winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::FrameworkElement>(*this));
        }
    }

    void ResizeThumb::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        // Ensure we have the proper cursor value setup
        OnDirectionPropertyChanged(*this, nullptr);
        OnCursorPropertyChanged(*this, nullptr);
    }

    void ResizeThumb::OnManipulationStarting([[maybe_unused]] winrt::ManipulationStartingRoutedEventArgs const& e)
    {
        // Snap the original size and position when we start dragging.
        auto target = TargetControl();
        _originalSize = winrt::Size(
            target ? static_cast<float>(target.ActualWidth()) : 0, 
            target ? static_cast<float>(target.ActualHeight()) : 0);

        if (PositionMode() == ResizePositionMode::MarginTopLeft)
        {
            _originalMargin = target.Margin();
        }
        else
        {
            auto left = winrt::Canvas::GetLeft(target ? target : *this);
            auto top = winrt::Canvas::GetTop(target ? target : *this);
            _originalPosition = winrt::Point(static_cast<float>(left), static_cast<float>(top));
        }
    }

    void ResizeThumb::OnManipulationDelta(winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        base_type::OnManipulationDelta(e);

        // We use Truncate here to provide 'snapping' points with the DragIncrement property
        // It works for both our negative and positive values, as otherwise we'd need to use
        // Ceiling when negative and Floor when positive to maintain the correct behavior.
        auto dragIncrement = DragIncrement();
        auto translation = e.Cumulative().Translation;
        auto horizontalChange =
            std::trunc(translation.X / dragIncrement) * dragIncrement;
        auto verticalChange =
            std::trunc(translation.Y / dragIncrement) * dragIncrement;

        // Important: adjust for RTL language flow settings and invert horizontal axis
        if (FlowDirection() == winrt::FlowDirection::RightToLeft)
        {
            horizontalChange *= -1;
        }

        auto direction = Direction();
        // If we're adjusting the opposite boundary then we need to invert the change values.
        if (direction == ResizeDirection::Right || direction == ResizeDirection::TopRight || direction == ResizeDirection::BottomRight)
        {
            horizontalChange *= -1;
        }

        if (direction == ResizeDirection::Bottom || direction == ResizeDirection::BottomLeft || direction == ResizeDirection::BottomRight)
        {
            verticalChange *= -1;
        }

        // Apply the changes to the target control
        if (auto target = TargetControl())
        {
            // Keep track if we became constrained in a direction and don't adjust position if we didn't update the size.
            bool adjustWidth = false;
            bool adjustHeight = false;

            // Calculate the new size (Note: This is the opposite direction to expand the opposing boundary of the thumb)
            auto newWidth = (_originalSize ? _originalSize->Width : 0) - horizontalChange;
            auto newHeight = (_originalSize ? _originalSize->Height : 0) - verticalChange;

            // TODO: There may be other constraints on elements (aspect ratio of constrained box, image set to uniform, etc...)
            // that may need to be considered here as well in which case we should restrict our reactions to those as well to stay synced
            // and not move the element unexpectedly.
            if (direction != ResizeDirection::Top && direction != ResizeDirection::Bottom)
            {
                if (IsValidWidth(target, newWidth, ActualWidth()))
                {
                    target.Width(newWidth);
                    adjustWidth = true;
                }
            }

            if (direction != ResizeDirection::Left && direction != ResizeDirection::Right)
            {
                if (IsValidHeight(target, newHeight, ActualHeight()))
                {
                    target.Height(newHeight);
                    adjustHeight = true;
                }
            }

            // Adjust the position based on position mode first
            if (PositionMode() == ResizePositionMode::MarginTopLeft)
            {
                auto newMargin = _originalMargin ? *_originalMargin : Thickness();

                if ((direction == ResizeDirection::Left || direction == ResizeDirection::TopLeft || direction == ResizeDirection::BottomLeft)
                    && adjustWidth)
                    newMargin.Left += horizontalChange;

                if ((direction == ResizeDirection::Top || direction == ResizeDirection::TopLeft || direction == ResizeDirection::TopRight)
                    && adjustHeight)
                    newMargin.Top += verticalChange;

                target.Margin(newMargin);

                auto args = winrt::make_self<TargetControlResizedEventArgs>(
                    newMargin.Left,
                    newMargin.Top,
                    target.Width(),
                    target.Height());

                TargetControlResized.invoke(*this, *args);
            }
            else
            {
                auto newX = (_originalPosition ? _originalPosition->X : 0) + horizontalChange;
                auto newY = (_originalPosition ? _originalPosition->Y : 0) + verticalChange;

                if ((direction == ResizeDirection::Left || direction == ResizeDirection::TopLeft || direction == ResizeDirection::BottomLeft)
                    && adjustWidth) 
                {
                    winrt::Canvas::SetLeft(target, newX);
                }

                if ((direction == ResizeDirection::Top || direction == ResizeDirection::TopLeft || direction == ResizeDirection::TopRight)
                    && adjustHeight)
                {
                    winrt::Canvas::SetTop(target, newY);
                }

                auto args = winrt::make_self<TargetControlResizedEventArgs>(
                    winrt::Canvas::GetLeft(target),
                    winrt::Canvas::GetTop(target),
                    target.Width(),
                    target.Height());
                TargetControlResized.invoke(*this, *args);
            }
        }
    }
}
