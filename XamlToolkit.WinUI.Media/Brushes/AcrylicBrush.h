// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "AcrylicBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import std;
import winrt.Windows.Foundation;
import winrt.Windows.UI;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;
#endif

namespace winrt
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
}

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

        static const wil::single_threaded_property<winrt::DependencyProperty> BlurAmountProperty;

        winrt::Color TintColor() const;
        void TintColor(winrt::Color value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TintColorProperty;

        double TintOpacity() const;
        void TintOpacity(double value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TintOpacityProperty;

        winrt::Uri TextureUri() const;
        void TextureUri(winrt::Uri const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TextureUriProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        std::function<void(winrt::CompositionBrush const&, float const&)> _blurAmountSetter;
        std::function<void(winrt::CompositionBrush const&, winrt::Color const&)> _tintColorSetter;
        std::function<void(winrt::CompositionBrush const&, float const&)> _tintOpacitySetter;

        static void OnBlurAmountPropertyChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnTintColorPropertyChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnTintOpacityPropertyChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnTextureUriPropertyChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct AcrylicBrush : AcrylicBrushT<AcrylicBrush, implementation::AcrylicBrush>
    {
    };
}