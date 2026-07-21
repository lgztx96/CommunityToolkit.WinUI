// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "GradientSlider.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::GradientStopCollection GradientSlider::GradientStops() const
    {
        return GetValue(GradientStopsProperty()).try_as<winrt::GradientStopCollection>();
    }

    void GradientSlider::GradientStops(winrt::GradientStopCollection const& value)
    {
        SetValue(GradientStopsProperty(), winrt::box_value(value));
    }

    bool GradientSlider::IsAddStopsEnabled() const
    {
        return winrt::unbox_value<bool>(GetValue(IsAddStopsEnabledProperty()));
    }

    void GradientSlider::IsAddStopsEnabled(bool value)
    {
        SetValue(IsAddStopsEnabledProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<winrt::DependencyProperty> GradientSlider::GradientStopsProperty =
        winrt::DependencyProperty::Register(
            L"GradientStops",
            winrt::xaml_typename<winrt::GradientStopCollection>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &GradientSlider::GradientStopsChangedCallback));

    const wil::single_threaded_property<winrt::DependencyProperty> GradientSlider::IsAddStopsEnabledProperty =
        winrt::DependencyProperty::Register(
            L"IsAddStopsEnabled",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(true)));

    void GradientSlider::GradientStopsChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const&)
    {
        auto control = d.try_as<class_type>();
        if (!control)
            return;

        auto self = winrt::get_self<GradientSlider>(control)->get_strong();

        if (!self->_containerCanvas)
            return;

        // TODO: What happens if the gradient stop collection changes while the user is dragging a stop?

        self->RefreshThumbs();
    }
}
