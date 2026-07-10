// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropBlurBrush.g.h"
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
    /// The <see cref="BackdropBlurBrush"/> is a <see cref="Brush"/> that blurs whatever is behind it in the application.
    /// </summary>
    struct BackdropBlurBrush : BackdropBlurBrushT<BackdropBlurBrush, XamlCompositionEffectBrushBase>
    {
        BackdropBlurBrush() = default;

        double Amount() const;
        void Amount(double value);

        static const wil::single_threaded_property<winrt::DependencyProperty> AmountProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::CompositionBrush const&, float const&)> _amountSetter;

        static void OnAmountChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropBlurBrush : BackdropBlurBrushT<BackdropBlurBrush, implementation::BackdropBlurBrush>
    {
    };
}