// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "GradientSliderThumb.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::event_token GradientSliderThumb::DragStarted(DragStartedEventHandler const& handler)
    {
        return _dragStarted.add(handler);
    }

    void GradientSliderThumb::DragStarted(winrt::event_token const& token)
    {
        _dragStarted.remove(token);
    }
    winrt::event_token GradientSliderThumb::DragDelta(DragDeltaEventHandler const& handler)
    {
        return _dragDelta.add(handler);
    }

    void GradientSliderThumb::DragDelta(winrt::event_token const& token)
    {
        _dragDelta.remove(token);
    }

    winrt::event_token GradientSliderThumb::DragCompleted(DragCompletedEventHandler const& handler)
    {
        return _dragCompleted.add(handler);
    }

    void GradientSliderThumb::DragCompleted(winrt::event_token const& token)
    {
        _dragCompleted.remove(token);
    }

    void GradientSliderThumb::GradientSliderThumb_PointerEntered(IInspectable const&, PointerRoutedEventArgs const&)
    {
        _pointerOver = true;

        if (!_pressed)
        {
            VisualStateManager::GoToState(*this, PointerOverStateName, true);
        }
    }

    void GradientSliderThumb::GradientSliderThumb_PointerExited(IInspectable const&, PointerRoutedEventArgs const&)
    {
        _pointerOver = false;

        if (!_pressed)
        {
            VisualStateManager::GoToState(*this, NormalStateName, true);
        }
    }

    void GradientSliderThumb::GradientSliderThumb_PointerPressed(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        _pressed = true;
        _isDragging = true;

        CapturePointer(e.Pointer());

        _dragStartPosition = e.GetCurrentPoint(nullptr).Position();
        _lastPosition = _dragStartPosition;

        DragStartedEventArgs dragStartedArgs(_dragStartPosition.X, _dragStartPosition.Y);
        if (_dragStarted) _dragStarted(*this, dragStartedArgs);

        VisualStateManager::GoToState(*this, PressedStateName, true);
    }

    void GradientSliderThumb::GradientSliderThumb_PointerMoved(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (!_isDragging)
            return;

        auto position = e.GetCurrentPoint(nullptr).Position();

        double deltaX = position.X - _lastPosition.X;
        double deltaY = position.Y - _lastPosition.Y;

        _lastPosition = position;

        DragDeltaEventArgs dragDeltaArgs(deltaX, deltaY);
        if (_dragDelta) _dragDelta(*this, dragDeltaArgs);
    }

    void GradientSliderThumb::GradientSliderThumb_PointerReleased(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (_isDragging)
        {
            auto end = e.GetCurrentPoint(nullptr).Position();

            double totalX = end.X - _dragStartPosition.X;
            double totalY = end.Y - _dragStartPosition.Y;

            DragCompletedEventArgs dragCompletedArgs(totalX, totalY, false);
            if (_dragCompleted) _dragCompleted(*this, dragCompletedArgs);
        }

        _isDragging = false;
        _pressed = false;

        ReleasePointerCapture(e.Pointer());

        VisualStateManager::GoToState(*this, _pointerOver ? PointerOverStateName : NormalStateName, true);
    }

    void GradientSliderThumb::GradientSliderThumb_PointerCanceled(IInspectable const&, PointerRoutedEventArgs const& e)
    {
        if (_isDragging)
        {
            DragCompletedEventArgs dragCompletedArgs(0, 0, true);
            if (_dragCompleted) _dragCompleted(*this, dragCompletedArgs);
        }

        _isDragging = false;
        _pressed = false;

        ReleasePointerCapture(e.Pointer());
        VisualStateManager::GoToState(*this, NormalStateName, true);
    }

    void GradientSliderThumb::GradientSliderThumb_IsEnabledChanged(IInspectable const&, DependencyPropertyChangedEventArgs const&)
    {
        VisualStateManager::GoToState(*this, IsEnabled() ? NormalStateName : DisabledStateName, true);
    }
}
