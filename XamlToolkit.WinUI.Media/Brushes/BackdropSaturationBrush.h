// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropSaturationBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Microsoft.UI.Xaml;
#endif

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// Brush which applies a SaturationEffect to the Backdrop.
    /// </summary>
    struct BackdropSaturationBrush : BackdropSaturationBrushT<BackdropSaturationBrush, XamlCompositionEffectBrushBase>
    {
        BackdropSaturationBrush() = default;

        double Saturation() const;
        void Saturation(double value);

        static const wil::single_threaded_property<winrt::DependencyProperty> SaturationProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, float const&)> _setter;

        static void OnSaturationChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropSaturationBrush : BackdropSaturationBrushT<BackdropSaturationBrush, implementation::BackdropSaturationBrush>
    {
    };
}