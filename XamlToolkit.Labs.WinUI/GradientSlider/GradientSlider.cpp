// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "GradientSlider.h"
#if __has_include("GradientSlider.g.cpp")
#include "GradientSlider.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    GradientSlider::GradientSlider()
        : _containerCanvas(nullptr)
        , _placeholderThumb(nullptr)
        , _backgroundRectangle(nullptr)
        , _dragPosition(0, 0)
        , _draggingThumb(nullptr)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

        GradientStopCollection stops;

        auto blackStop = GradientStop();
        blackStop.Color(Colors::Black());
        blackStop.Offset(0.0);

        auto whiteStop = GradientStop();
        whiteStop.Color(Colors::White());
        whiteStop.Offset(1.0);

        stops.Append(blackStop);
        stops.Append(whiteStop);

        GradientStops(stops);
    }

    void GradientSlider::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        if (_containerCanvas)
        {
            _containerCanvasSizeChangedRevoker.revoke();
        }

        _containerCanvas = GetTemplateChild(ContainerCanvasPartName).try_as<Canvas>();
        _placeholderThumb = GetTemplateChild(PlaceholderThumbPartName).try_as<Thumb>();
        _backgroundRectangle = GetTemplateChild(BackgroundRectanglePartName).try_as<Rectangle>();

        if (_containerCanvas)
        {
            _containerCanvasSizeChangedRevoker = _containerCanvas.SizeChanged(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_SizeChanged });
        }

        if (_placeholderThumb)
        {
            _containerCanvasPointerEnteredRevoker = _containerCanvas.PointerEntered(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_PointerEntered });
            _containerCanvasPointerMovedRevoker = _containerCanvas.PointerMoved(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_PointerMoved });
            _containerCanvasPointerExitedRevoker = _containerCanvas.PointerExited(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_PointerExited });
            _containerCanvasPointerPressedRevoker = _containerCanvas.PointerPressed(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_PointerPressed });
            _containerCanvasPointerReleasedRevoker = _containerCanvas.PointerReleased(winrt::auto_revoke, { this, &GradientSlider::ContainerCanvas_PointerReleased });

            _placeholderThumb.Visibility(Visibility::Collapsed);
        }

        RefreshThumbs();
    }

    void GradientSlider::ContainerCanvas_SizeChanged(IInspectable const&, SizeChangedEventArgs const&)
    {
        SyncThumbs();
    }

    winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb GradientSlider::AddStopThumb(GradientStop const& stop)
    {
        if (!_containerCanvas)
        {
            throw winrt::hresult_access_denied(L"ContainerCanvas is not available.");
        }

        // Prepare a thumb for the gradient stop
        auto thumb = winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb();
        thumb.GradientStop(stop);

        // Subcribe to events and callbacks
        thumb.DragStarted({ this, &GradientSlider::Thumb_DragStarted });
        thumb.DragDelta({ this, &GradientSlider::Thumb_DragDelta });
        thumb.DragCompleted({ this, &GradientSlider::Thumb_DragCompleted });
        thumb.KeyDown({ this, &GradientSlider::Thumb_KeyDown });
        thumb.Loaded({ this, &GradientSlider::Thumb_Loaded });
        thumb.RightTapped({ this, &GradientSlider::Thumb_RightTapped });

        auto token = stop.RegisterPropertyChangedCallback(
            GradientStop::OffsetProperty(),
            DependencyPropertyChangedCallback{ this, &GradientSlider::OnGradientStopOffsetChanged });

        _stopCallbacks.insert_or_assign(stop, token);

        // Track the thumb and add to the canvas
        _stopThumbs.insert_or_assign(stop, thumb);
        _containerCanvas.Children().Append(thumb);

        return thumb;
    }

    void GradientSlider::RemoveStopThumb(GradientStop const& stop)
    {
        if (!_containerCanvas)
            return;

        // Should this be an exception?
        auto thumbIt = _stopThumbs.find(stop);
        if (thumbIt == _stopThumbs.end())
            return;

        auto& thumb = thumbIt->second;

        if (auto callbackIt = _stopCallbacks.find(stop); callbackIt != _stopCallbacks.end())
        {
            stop.UnregisterPropertyChangedCallback(GradientStop::OffsetProperty(), callbackIt->second);
            _stopCallbacks.erase(callbackIt);
        }

        uint32_t index;
        auto children = _containerCanvas.Children();
        if (children.IndexOf(thumb, index))
        {
            children.RemoveAt(index);
        }
        
        _stopThumbs.erase(thumbIt);
    }

    void GradientSlider::RefreshThumbs()
    {
        ClearThumbs();
        for (auto const& stop : GradientStops())
        {
            AddStopThumb(stop);
        }

        SyncBackground();
    }

    void GradientSlider::ClearThumbs()
    {
        std::vector<GradientStop> stopsToRemove;
        for (auto const& [key, thumb] : _stopThumbs)
        {
            stopsToRemove.push_back(key);
        }

        for (auto const& stop : stopsToRemove)
        {
            RemoveStopThumb(stop);
        }
    }

    void GradientSlider::SyncThumbs()
    {
        for (auto const& [_, thumb] : _stopThumbs)
        {
            UpdateThumbPosition(thumb);
        }
    }

    void GradientSlider::SyncBackground()
    {
        if (!_containerCanvas || !_backgroundRectangle)
            return;

        auto brush = LinearGradientBrush();
        brush.GradientStops(GradientStops());
        brush.StartPoint(Point(0.0f, 0.5f));
        brush.EndPoint(Point(1.0f, 0.5f));

        _backgroundRectangle.Fill(brush);
    }

    void GradientSlider::Thumb_Loaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto thumb = sender.try_as<winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb>();
        if (!thumb)
            return;

        // Thumb position cannot be determined until it has loaded.
        // Defer until the loading event
        UpdateThumbPosition(thumb);
    }

    void GradientSlider::OnGradientStopOffsetChanged(DependencyObject const& d, DependencyProperty const&)
    {
        auto stop = d.try_as<GradientStop>();
        if (!stop)
            return;

        auto it = _stopThumbs.find(stop);
        if (it == _stopThumbs.end())
            return;

        UpdateThumbPosition(it->second);
    }

    void GradientSlider::UpdateThumbPosition(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb)
    {
        if (!_containerCanvas)
            return;

        auto dragWidth = _containerCanvas.ActualWidth() - thumb.ActualWidth();
        Canvas::SetLeft(thumb, thumb.GradientStop().Offset() * dragWidth);
    }
}
