// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "BackdropSaturationBrush.h"
#if __has_include("BackdropSaturationBrush.g.cpp")
#include "BackdropSaturationBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BackdropSaturationBrush::SaturationProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Saturation",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.5), &BackdropSaturationBrush::OnSaturationChanged });

    double BackdropSaturationBrush::Saturation() const
    {
        return winrt::unbox_value<double>(GetValue(SaturationProperty()));
    }

    void BackdropSaturationBrush::Saturation(double value)
    {
        SetValue(SaturationProperty(), winrt::box_value(value));
    }

    void BackdropSaturationBrush::OnSaturationChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::BackdropSaturationBrush>();
		auto self = winrt::get_self<BackdropSaturationBrush>(brush);

        // Clamp Value as per docs http://microsoft.github.io/Win2D/html/T_Microsoft_Graphics_Canvas_Effects_SaturationEffect.htm
        auto value = winrt::unbox_value<double>(e.NewValue());
        if (value > 1.0)
        {
            self->Saturation(1.0);
        }
        else if (value < 0.0)
        {
            self->Saturation(0.0);
        }

        if (auto target = self->CompositionBrush())
        {
            if (self->_setter)
            {
                self->_setter(target, static_cast<float>(self->Saturation()));
            }
        }
    }

    Pipelines::PipelineBuilder BackdropSaturationBrush::OnPipelineRequested()
    {
        auto builder = Pipelines::PipelineBuilderImpl::FromBackdrop();
        return winrt::make<Pipelines::implementation::PipelineBuilder>(builder.Saturation(static_cast<float>(Saturation()), _setter));
    }
}