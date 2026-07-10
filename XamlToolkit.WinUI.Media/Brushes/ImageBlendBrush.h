// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ImageBlendBrush.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Media;
import winrt.Microsoft.UI.Composition;
#endif

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// Brush which blends a <see cref="BitmapImage"/> to the Backdrop in a given mode.
    /// </summary>
    struct ImageBlendBrush : ImageBlendBrushT<ImageBlendBrush>
    {
        ImageBlendBrush() = default;

        winrt::ImageSource Source() const;
        void Source(winrt::ImageSource const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;

        winrt::Stretch Stretch() const;
        void Stretch(winrt::Stretch value);

        static const wil::single_threaded_property<winrt::DependencyProperty> StretchProperty;

        winrt::XamlToolkit::WinUI::Media::ImageBlendMode Mode() const;
        void Mode(winrt::XamlToolkit::WinUI::Media::ImageBlendMode value);

        static const wil::single_threaded_property<winrt::DependencyProperty> ModeProperty;

        void OnConnected();
        void OnDisconnected();

    private:
        winrt::LoadedImageSurface _surface{ nullptr };
        winrt::Microsoft::UI::Composition::CompositionSurfaceBrush _surfaceBrush{ nullptr };

        static void OnSourceChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnStretchChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnModeChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

        static winrt::CompositionStretch CompositionStretchFromStretch(winrt::Stretch value);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct ImageBlendBrush : ImageBlendBrushT<ImageBlendBrush, implementation::ImageBlendBrush>
    {
    };
}