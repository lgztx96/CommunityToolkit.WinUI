// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "AcrylicBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import std;
import winrt.Windows.Foundation;
import winrt.Windows.UI;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A <see cref="XamlCompositionBrush"/> that implements an acrylic effect with customizable parameters
    /// </summary>
    struct AcrylicBrush : AcrylicBrushT<AcrylicBrush, XamlCompositionEffectBrushBase>
    {
        AcrylicBrush() = default;

        double BlurAmount() const;
        void BlurAmount(double value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BlurAmountProperty;

        winrt::Windows::UI::Color TintColor() const;
        void TintColor(winrt::Windows::UI::Color value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TintColorProperty;

        double TintOpacity() const;
        void TintOpacity(double value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TintOpacityProperty;

        winrt::Windows::Foundation::Uri TextureUri() const;
        void TextureUri(winrt::Windows::Foundation::Uri const& value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TextureUriProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, float const&)> _blurAmountSetter;
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, winrt::Windows::UI::Color const&)> _tintColorSetter;
        std::function<void(winrt::Microsoft::UI::Composition::CompositionBrush const&, float const&)> _tintOpacitySetter;

        static void OnBlurAmountPropertyChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void OnTintColorPropertyChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void OnTintOpacityPropertyChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void OnTextureUriPropertyChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct AcrylicBrush : AcrylicBrushT<AcrylicBrush, implementation::AcrylicBrush>
    {
    };
}