// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "GradientSlider.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    GradientStopCollection GradientSlider::GradientStops() const
    {
        return winrt::unbox_value<GradientStopCollection>(GetValue(GradientStopsProperty()));
    }

    void GradientSlider::GradientStops(GradientStopCollection const& value)
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

    const wil::single_threaded_property<DependencyProperty> GradientSlider::GradientStopsProperty =
        DependencyProperty::Register(
            L"GradientStops",
            winrt::xaml_typename<GradientStopCollection>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &GradientSlider::GradientStopsChangedCallback));

    const wil::single_threaded_property<DependencyProperty> GradientSlider::IsAddStopsEnabledProperty =
        DependencyProperty::Register(
            L"IsAddStopsEnabled",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(true)));

    void GradientSlider::GradientStopsChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const&)
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
