// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropGammaTransferBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Microsoft.UI.Xaml;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A brush which alters the colors of whatever is behind it in the application by applying a per-channel gamma transfer function.
    /// </summary>
    struct BackdropGammaTransferBrush : BackdropGammaTransferBrushT<BackdropGammaTransferBrush, XamlCompositionEffectBrushBase>
    {
        BackdropGammaTransferBrush() = default;

        // Alpha channel
        double AlphaAmplitude() const;
        void AlphaAmplitude(double value);
        bool AlphaDisable() const;
        void AlphaDisable(bool value);
        double AlphaExponent() const;
        void AlphaExponent(double value);
        double AlphaOffset() const;
        void AlphaOffset(double value);

        // Red channel
        double RedAmplitude() const;
        void RedAmplitude(double value);
        bool RedDisable() const;
        void RedDisable(bool value);
        double RedExponent() const;
        void RedExponent(double value);
        double RedOffset() const;
        void RedOffset(double value);

        // Green channel
        double GreenAmplitude() const;
        void GreenAmplitude(double value);
        bool GreenDisable() const;
        void GreenDisable(bool value);
        double GreenExponent() const;
        void GreenExponent(double value);
        double GreenOffset() const;
        void GreenOffset(double value);

        // Blue channel
        double BlueAmplitude() const;
        void BlueAmplitude(double value);
        bool BlueDisable() const;
        void BlueDisable(bool value);
        double BlueExponent() const;
        void BlueExponent(double value);
        double BlueOffset() const;
        void BlueOffset(double value);

        // Dependency properties
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlphaAmplitudeProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlphaDisableProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlphaExponentProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlphaOffsetProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RedAmplitudeProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RedDisableProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RedExponentProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RedOffsetProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> GreenAmplitudeProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> GreenDisableProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> GreenExponentProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> GreenOffsetProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BlueAmplitudeProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BlueDisableProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BlueExponentProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BlueOffsetProperty;

    private:
        static winrt::Microsoft::UI::Xaml::PropertyChangedCallback OnScalarPropertyChangedHelper(std::wstring_view propertyName);

        static void OnBooleanPropertyChanged(winrt::Microsoft::UI::Xaml::DependencyObject const& d, winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        winrt::fire_and_forget OnConnected() override;
        void OnDisconnected() override;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropGammaTransferBrush : BackdropGammaTransferBrushT<BackdropGammaTransferBrush, implementation::BackdropGammaTransferBrush>
    {
    };
}