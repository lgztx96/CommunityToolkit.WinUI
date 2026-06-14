// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropBlurBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;

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

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AmountProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, float const&)> _amountSetter;

        static void OnAmountChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropBlurBrush : BackdropBlurBrushT<BackdropBlurBrush, implementation::BackdropBlurBrush>
    {
    };
}