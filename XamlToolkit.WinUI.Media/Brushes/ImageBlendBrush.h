// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ImageBlendBrush.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Media;
import winrt.Microsoft.UI.Composition;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// Brush which blends a <see cref="BitmapImage"/> to the Backdrop in a given mode.
    /// </summary>
    struct ImageBlendBrush : ImageBlendBrushT<ImageBlendBrush>
    {
        ImageBlendBrush() = default;

        winrt::Microsoft::UI::Xaml::Media::ImageSource Source() const;
        void Source(winrt::Microsoft::UI::Xaml::Media::ImageSource const& value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SourceProperty;

        winrt::Microsoft::UI::Xaml::Media::Stretch Stretch() const;
        void Stretch(winrt::Microsoft::UI::Xaml::Media::Stretch value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> StretchProperty;

        winrt::XamlToolkit::WinUI::Media::ImageBlendMode Mode() const;
        void Mode(winrt::XamlToolkit::WinUI::Media::ImageBlendMode value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ModeProperty;

        void OnConnected();
        void OnDisconnected();

    private:
        winrt::Microsoft::UI::Xaml::Media::LoadedImageSurface _surface{ nullptr };
        winrt::Microsoft::UI::Composition::CompositionSurfaceBrush _surfaceBrush{ nullptr };

        static void OnSourceChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void OnStretchChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void OnModeChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static winrt::Microsoft::UI::Composition::CompositionStretch CompositionStretchFromStretch(
            winrt::Microsoft::UI::Xaml::Media::Stretch value);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct ImageBlendBrush : ImageBlendBrushT<ImageBlendBrush, implementation::ImageBlendBrush>
    {
    };
}