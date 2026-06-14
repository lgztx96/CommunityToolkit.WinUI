// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropSepiaBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import std;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// Brush which applies a SepiaEffect to the Backdrop.
    /// </summary>
    struct BackdropSepiaBrush : BackdropSepiaBrushT<BackdropSepiaBrush, XamlCompositionEffectBrushBase>
    {
        BackdropSepiaBrush() = default;

        double Intensity() const;
        void Intensity(double value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IntensityProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, float const&)> _setter;

        static void OnIntensityChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropSepiaBrush : BackdropSepiaBrushT<BackdropSepiaBrush, implementation::BackdropSepiaBrush>
    {
    };
}