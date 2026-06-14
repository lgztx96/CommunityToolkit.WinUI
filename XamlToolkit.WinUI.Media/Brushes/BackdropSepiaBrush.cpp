// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "BackdropSepiaBrush.h"
#if __has_include("BackdropSepiaBrush.g.cpp")
#include "BackdropSepiaBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BackdropSepiaBrush::IntensityProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Intensity",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.5), &BackdropSepiaBrush::OnIntensityChanged });

    double BackdropSepiaBrush::Intensity() const
    {
        return winrt::unbox_value<double>(GetValue(IntensityProperty()));
    }

    void BackdropSepiaBrush::Intensity(double value)
    {
        SetValue(IntensityProperty(), winrt::box_value(value));
    }

    void BackdropSepiaBrush::OnIntensityChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::BackdropSepiaBrush>();
		auto self = winrt::get_self<BackdropSepiaBrush>(brush);
        // Clamp Value as per docs http://microsoft.github.io/Win2D/html/T_Microsoft_Graphics_Canvas_Effects_SepiaEffect.htm
        auto value = winrt::unbox_value<double>(e.NewValue());
        if (value > 1.0)
        {
            self->Intensity(1.0);
        }
        else if (value < 0.0)
        {
            self->Intensity(0.0);
        }

        if (auto target = self->CompositionBrush())
        {
            if (self->_setter)
            {
                self->_setter(target, static_cast<float>(self->Intensity()));
            }
        }
    }

    Pipelines::PipelineBuilder BackdropSepiaBrush::OnPipelineRequested()
    {
        auto builder = Pipelines::PipelineBuilderImpl::FromBackdrop();
        return winrt::make<Pipelines::implementation::PipelineBuilder>(builder.Sepia(static_cast<float>(Intensity()), _setter));
    }
}