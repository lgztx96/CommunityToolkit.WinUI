// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "GradientSlider.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void GradientSlider::Thumb_DragStarted(winrt::IInspectable const& sender, DragStartedEventArgs const& e)
    {
        if (auto thumb = sender.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>())
        {
            _draggingThumb = thumb;
            auto xStart = Canvas::GetLeft(thumb);
            auto yStart = e.VerticalOffset();
            _dragPosition = Point(static_cast<float>(xStart), static_cast<float>(yStart));

            OnThumbDragStarted(e);
        }
    }

    void GradientSlider::Thumb_DragDelta(winrt::IInspectable const& sender, DragDeltaEventArgs const& e)
    {
        if (!_containerCanvas)
            return;

        if (auto thumb = sender.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>()) 
        {
            _dragPosition.X += static_cast<float>(e.HorizontalChange());
            _dragPosition.Y += static_cast<float>(e.VerticalChange());

            HandleThumbDragging(thumb, _dragPosition);
        }
    }

    void GradientSlider::Thumb_DragCompleted(winrt::IInspectable const&, DragCompletedEventArgs const& e)
    {
        _draggingThumb = nullptr;

        OnThumbDragCompleted(e);
        OnValueChanged();
    }

    void GradientSlider::Thumb_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        auto thumb = sender.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>();
        if (!thumb)
            return;

        double change = 0.0;

        if (e.Key() == VirtualKey::Left)
        {
            if (FlowDirection() == FlowDirection::RightToLeft)
                change = 0.05;
            else
                change = -0.01;
        }
        else if (e.Key() == VirtualKey::Right)
        {
            if (FlowDirection() == FlowDirection::RightToLeft)
                change = -0.05;
            else
                change = 0.01;
        }

        if (change != 0.0)
        {
            auto stop = thumb.GradientStop();
            stop.Offset(std::clamp(change + stop.Offset(), 0.0, 1.0));
            UpdateThumbPosition(thumb);
            e.Handled(true);
        }
    }

    void GradientSlider::Thumb_RightTapped(IInspectable const& sender, RightTappedRoutedEventArgs const&)
    {
        auto thumb = sender.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>();
        if (!thumb)
            return;

        // Prevent removing the last stop
        if (GradientStops().Size() <= 1)
            return;

        auto stop = thumb.GradientStop();

        uint32_t index = 0;
        for (auto const& s : GradientStops())
        {
            if (winrt::get_abi(s) == winrt::get_abi(stop))
            {
                GradientStops().RemoveAt(index);
                break;
            }
            index++;
        }

        RemoveStopThumb(stop);
    }

    void GradientSlider::ContainerCanvas_PointerEntered(IInspectable const&, PointerRoutedEventArgs const&)
    {
        if (!_placeholderThumb)
            return;

        if (IsAddStopsEnabled())
        {
            _placeholderThumb.Visibility(Visibility::Visible);
        }

        VisualStateManager::GoToState(*this, PointerOverStateName, false);
    }

    void GradientSlider::ContainerCanvas_PointerMoved(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (!_containerCanvas || !_placeholderThumb)
            return;

        auto position = e.GetCurrentPoint(_containerCanvas).Position();
        auto posX = position.X;

        if (!_draggingThumb)
        {
            // NOTE: This check could be made O(log(n)) by tracking the thumbs positions in a sorted list and running a binary search
            _placeholderThumb.IsEnabled(!IsPointerOverThumb(posX));

            auto thumbPosition = posX - _placeholderThumb.ActualWidth() / 2;
            thumbPosition = std::clamp(thumbPosition, 0.0, _containerCanvas.ActualWidth() - _placeholderThumb.ActualWidth());
            Canvas::SetLeft(_placeholderThumb, thumbPosition);
        }
        else
        {
            auto captures = _draggingThumb.PointerCaptures();
            if (!captures || captures.Size() == 0)
            {
                HandleThumbDragging(_draggingThumb, position);
            }
        }
    }

    void GradientSlider::ContainerCanvas_PointerExited(IInspectable const&, PointerRoutedEventArgs const&)
    {
        if (!_placeholderThumb)
            return;

        _placeholderThumb.Visibility(Visibility::Collapsed);
        _placeholderThumb.IsEnabled(false);

        VisualStateManager::GoToState(*this, NormalStateName, false);
    }

    void GradientSlider::ContainerCanvas_PointerPressed(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (!_containerCanvas || !_placeholderThumb)
            return;

        if (!IsAddStopsEnabled())
            return;

        auto positionX = e.GetCurrentPoint(_containerCanvas).Position().X;
        if (IsPointerOverThumb(positionX))
            return;

        _containerCanvas.CapturePointer(e.Pointer());

        _placeholderThumb.IsEnabled(false);

        GradientStop stop;
        stop.Offset(positionX / _containerCanvas.ActualWidth());
        stop.Color(Colors::Black());

        GradientStops().Append(stop);
        _draggingThumb = AddStopThumb(stop);
    }

    void GradientSlider::ContainerCanvas_PointerReleased(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (!_containerCanvas)
            return;

        _draggingThumb = nullptr;
        _containerCanvas.ReleasePointerCapture(e.Pointer());

        OnValueChanged();
    }

    bool GradientSlider::IsPointerOverThumb(double position)
    {
        if (!_containerCanvas)
            return false;

        for (auto const& child : _containerCanvas.Children())
        {
            auto thumb = child.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>();
            if (!thumb)
                continue;

            auto thumbPos = Canvas::GetLeft(thumb);
            if (position > thumbPos - thumb.ActualWidth() && position < thumbPos + (thumb.ActualWidth() * 2))
                return true;
        }

        return false;
    }

    void GradientSlider::HandleThumbDragging(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb, Point position)
    {
        if (!_containerCanvas)
            return;

        auto newPos = position.X / (_containerCanvas.ActualWidth() - thumb.ActualWidth());
        thumb.GradientStop().Offset(std::clamp(newPos, 0.0, 1.0));
        UpdateThumbPosition(thumb);
    }
}
