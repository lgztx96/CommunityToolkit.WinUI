// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "TilesBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A <see cref="XamlCompositionBrush"/> that displays a tiled image
    /// </summary>
    struct TilesBrush : TilesBrushT<TilesBrush, XamlCompositionEffectBrushBase>
    {
        TilesBrush() = default;

        winrt::Windows::Foundation::Uri TextureUri() const;
        void TextureUri(winrt::Windows::Foundation::Uri const& value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TextureUriProperty;

        DpiMode DpiMode() const;
        void DpiMode(winrt::XamlToolkit::WinUI::Media::DpiMode value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> DpiModeProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        static void OnDependencyPropertyChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct TilesBrush : TilesBrushT<TilesBrush, implementation::TilesBrush>
    {
    };
}