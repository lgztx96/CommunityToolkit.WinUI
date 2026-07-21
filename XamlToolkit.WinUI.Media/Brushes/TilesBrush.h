// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "TilesBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
#endif

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A <see cref="XamlCompositionBrush"/> that displays a tiled image
    /// </summary>
    struct TilesBrush : TilesBrushT<TilesBrush, XamlCompositionEffectBrushBase>
    {
        TilesBrush() = default;

        winrt::Uri TextureUri() const;
        void TextureUri(winrt::Uri const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TextureUriProperty;

        DpiMode DpiMode() const;
        void DpiMode(Media::DpiMode value);

        static const wil::single_threaded_property<winrt::DependencyProperty> DpiModeProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

    private:
        static void OnDependencyPropertyChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct TilesBrush : TilesBrushT<TilesBrush, implementation::TilesBrush>
    {
    };
}