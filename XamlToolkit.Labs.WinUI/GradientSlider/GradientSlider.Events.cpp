// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "GradientSlider.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::event_token GradientSlider::ThumbDragStarted(DragStartedEventHandler const& handler)
    {
        return _thumbDragStarted.add(handler);
    }

    void GradientSlider::ThumbDragStarted(winrt::event_token const& token)
    {
        _thumbDragStarted.remove(token);
    }

    winrt::event_token GradientSlider::ThumbDragCompleted(DragCompletedEventHandler const& handler)
    {
        return _thumbDragCompleted.add(handler);
    }

    void GradientSlider::ThumbDragCompleted(winrt::event_token const& token)
    {
        _thumbDragCompleted.remove(token);
    }

    void GradientSlider::OnThumbDragStarted(DragStartedEventArgs const& e)
    {
        if (_thumbDragStarted) _thumbDragStarted(*this, e);
    }

    void GradientSlider::OnThumbDragCompleted(DragCompletedEventArgs const& e)
    {
        if (_thumbDragCompleted) _thumbDragCompleted(*this, e);
    }

    void GradientSlider::OnValueChanged()
    {
        ValueChanged.invoke(*this, nullptr);
    }
}
