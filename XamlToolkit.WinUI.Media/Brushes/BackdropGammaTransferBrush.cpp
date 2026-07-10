// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "BackdropGammaTransferBrush.h"
#if __has_include("BackdropGammaTransferBrush.g.cpp")
#include "BackdropGammaTransferBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    // Alpha channel properties
    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::AlphaAmplitudeProperty =
        winrt::DependencyProperty::Register(
            L"AlphaAmplitude",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"AlphaAmplitude") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::AlphaDisableProperty =
        winrt::DependencyProperty::Register(
            L"AlphaDisable",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(false), &BackdropGammaTransferBrush::OnBooleanPropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::AlphaExponentProperty =
        winrt::DependencyProperty::Register(
            L"AlphaExponent",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"AlphaExponent") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::AlphaOffsetProperty =
        winrt::DependencyProperty::Register(
            L"AlphaOffset",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0), OnScalarPropertyChangedHelper(L"AlphaOffset") });


    // Red channel properties
    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::RedAmplitudeProperty =
        winrt::DependencyProperty::Register(
            L"RedAmplitude",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"RedAmplitude") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::RedDisableProperty =
        winrt::DependencyProperty::Register(
            L"RedDisable",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(false), &BackdropGammaTransferBrush::OnBooleanPropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::RedExponentProperty =
        winrt::DependencyProperty::Register(
            L"RedExponent",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"RedExponent") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::RedOffsetProperty =
        winrt::DependencyProperty::Register(
            L"RedOffset",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0), OnScalarPropertyChangedHelper(L"RedOffset") });


    // Green channel properties
    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::GreenAmplitudeProperty =
        winrt::DependencyProperty::Register(
            L"GreenAmplitude",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"GreenAmplitude") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::GreenDisableProperty =
        winrt::DependencyProperty::Register(
            L"GreenDisable",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(false), &BackdropGammaTransferBrush::OnBooleanPropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::GreenExponentProperty =
        winrt::DependencyProperty::Register(
            L"GreenExponent",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"GreenExponent") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::GreenOffsetProperty =
        winrt::DependencyProperty::Register(
            L"GreenOffset",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0), OnScalarPropertyChangedHelper(L"GreenOffset") });


    // Blue channel properties
    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::BlueAmplitudeProperty =
        winrt::DependencyProperty::Register(
            L"BlueAmplitude",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"BlueAmplitude") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::BlueDisableProperty =
        winrt::DependencyProperty::Register(
            L"BlueDisable",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(false), &BackdropGammaTransferBrush::OnBooleanPropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::BlueExponentProperty =
        winrt::DependencyProperty::Register(
            L"BlueExponent",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1.0), OnScalarPropertyChangedHelper(L"BlueExponent") });

    const wil::single_threaded_property<winrt::DependencyProperty> BackdropGammaTransferBrush::BlueOffsetProperty =
        winrt::DependencyProperty::Register(
            L"BlueOffset",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0), OnScalarPropertyChangedHelper(L"BlueOffset") });

    // Alpha channel setters
	double BackdropGammaTransferBrush::AlphaAmplitude() const { return winrt::unbox_value<double>(GetValue(AlphaAmplitudeProperty())); }
    void BackdropGammaTransferBrush::AlphaAmplitude(double value) { SetValue(AlphaAmplitudeProperty(), winrt::box_value(value)); }

	bool BackdropGammaTransferBrush::AlphaDisable() const{ return winrt::unbox_value<bool>(GetValue(AlphaDisableProperty())); }
    void BackdropGammaTransferBrush::AlphaDisable(bool value) { SetValue(AlphaDisableProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::AlphaExponent() const { return winrt::unbox_value<double>(GetValue(AlphaExponentProperty())); }
    void BackdropGammaTransferBrush::AlphaExponent(double value) { SetValue(AlphaExponentProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::AlphaOffset() const { return winrt::unbox_value<double>(GetValue(AlphaOffsetProperty())); }
    void BackdropGammaTransferBrush::AlphaOffset(double value) { SetValue(AlphaOffsetProperty(), winrt::box_value(value)); }

    // Red channel setters
	double BackdropGammaTransferBrush::RedAmplitude() const { return winrt::unbox_value<double>(GetValue(RedAmplitudeProperty())); }
    void BackdropGammaTransferBrush::RedAmplitude(double value) { SetValue(RedAmplitudeProperty(), winrt::box_value(value)); }

	bool BackdropGammaTransferBrush::RedDisable() const { return winrt::unbox_value<bool>(GetValue(RedDisableProperty())); }
    void BackdropGammaTransferBrush::RedDisable(bool value) { SetValue(RedDisableProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::RedExponent() const { return winrt::unbox_value<double>(GetValue(RedExponentProperty())); }
    void BackdropGammaTransferBrush::RedExponent(double value) { SetValue(RedExponentProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::RedOffset() const { return winrt::unbox_value<double>(GetValue(RedOffsetProperty())); }
    void BackdropGammaTransferBrush::RedOffset(double value) { SetValue(RedOffsetProperty(), winrt::box_value(value)); }

    // Green channel setters
	double BackdropGammaTransferBrush::GreenAmplitude() const { return winrt::unbox_value<double>(GetValue(GreenAmplitudeProperty())); }
    void BackdropGammaTransferBrush::GreenAmplitude(double value) { SetValue(GreenAmplitudeProperty(), winrt::box_value(value)); }

	bool BackdropGammaTransferBrush::GreenDisable() const { return winrt::unbox_value<bool>(GetValue(GreenDisableProperty())); }
    void BackdropGammaTransferBrush::GreenDisable(bool value) { SetValue(GreenDisableProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::GreenExponent() const { return winrt::unbox_value<double>(GetValue(GreenExponentProperty())); }
    void BackdropGammaTransferBrush::GreenExponent(double value) { SetValue(GreenExponentProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::GreenOffset() const { return winrt::unbox_value<double>(GetValue(GreenOffsetProperty())); }
    void BackdropGammaTransferBrush::GreenOffset(double value) { SetValue(GreenOffsetProperty(), winrt::box_value(value)); }

    // Blue channel setters
	double BackdropGammaTransferBrush::BlueAmplitude() const { return winrt::unbox_value<double>(GetValue(BlueAmplitudeProperty())); }
    void BackdropGammaTransferBrush::BlueAmplitude(double value) { SetValue(BlueAmplitudeProperty(), winrt::box_value(value)); }

	bool BackdropGammaTransferBrush::BlueDisable() const { return winrt::unbox_value<bool>(GetValue(BlueDisableProperty())); }
    void BackdropGammaTransferBrush::BlueDisable(bool value) { SetValue(BlueDisableProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::BlueExponent() const { return winrt::unbox_value<double>(GetValue(BlueExponentProperty())); }
    void BackdropGammaTransferBrush::BlueExponent(double value) { SetValue(BlueExponentProperty(), winrt::box_value(value)); }

	double BackdropGammaTransferBrush::BlueOffset() const { return winrt::unbox_value<double>(GetValue(BlueOffsetProperty())); }
    void BackdropGammaTransferBrush::BlueOffset(double value) { SetValue(BlueOffsetProperty(), winrt::box_value(value)); }

    winrt::PropertyChangedCallback BackdropGammaTransferBrush::OnScalarPropertyChangedHelper(std::wstring_view propertyName)
    {
        return [&](auto&& d, auto&& e)
        {
            auto brush = d.template as<winrt::XamlToolkit::WinUI::Media::BackdropGammaTransferBrush>();
            auto self = winrt::get_self<BackdropGammaTransferBrush>(brush);

            if (auto compositionBrush = self->CompositionBrush())
            {
                compositionBrush.Properties().InsertScalar(winrt::format(L"GammaTransfer.{}", propertyName), static_cast<float>(winrt::unbox_value<double>(e.NewValue())));
            }
        };
    }

    void BackdropGammaTransferBrush::OnBooleanPropertyChanged(
        winrt::DependencyObject const& d,
        [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::BackdropGammaTransferBrush>();
		auto self = winrt::get_self<BackdropGammaTransferBrush>(brush);
        self->OnDisconnected();
        self->OnConnected();
    }

    winrt::fire_and_forget BackdropGammaTransferBrush::OnConnected()
    {
        // Delay creating composition resources until they're required.
        if (CompositionBrush() == nullptr)
        {

            winrt::Microsoft::UI::Composition::CompositionCapabilities compositionCapabilities;

            // Abort if effects aren't supported.
            if (!compositionCapabilities.AreEffectsSupported())
            {
                co_return;
            }

            auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

            auto backdrop = compositor.CreateBackdropBrush();

            // Use a Win2D blur affect applied to a CompositionBackdropBrush.
            auto graphicsEffect = winrt::Microsoft::Graphics::Canvas::Effects::GammaTransferEffect();
            graphicsEffect.Name(L"GammaTransfer");
            graphicsEffect.AlphaAmplitude(static_cast<float>(AlphaAmplitude()));
            graphicsEffect.AlphaDisable(AlphaDisable());
            graphicsEffect.AlphaExponent(static_cast<float>(AlphaExponent()));
            graphicsEffect.AlphaOffset(static_cast<float>(AlphaOffset()));
            graphicsEffect.RedAmplitude(static_cast<float>(RedAmplitude()));
            graphicsEffect.RedDisable(RedDisable());
            graphicsEffect.RedExponent(static_cast<float>(RedExponent()));
            graphicsEffect.RedOffset(static_cast<float>(RedOffset()));
            graphicsEffect.GreenAmplitude(static_cast<float>(GreenAmplitude()));
            graphicsEffect.GreenDisable(GreenDisable());
            graphicsEffect.GreenExponent(static_cast<float>(GreenExponent()));
            graphicsEffect.GreenOffset(static_cast<float>(GreenOffset()));
            graphicsEffect.BlueAmplitude(static_cast<float>(BlueAmplitude()));
            graphicsEffect.BlueDisable(BlueDisable());
            graphicsEffect.BlueExponent(static_cast<float>(BlueExponent()));
            graphicsEffect.BlueOffset(static_cast<float>(BlueOffset()));

            // Source
            graphicsEffect.Source(winrt::Microsoft::UI::Composition::CompositionEffectSourceParameter(L"backdrop"));

            std::vector<winrt::hstring> animatableProperties =
            {
                L"GammaTransfer.AlphaAmplitude",
                L"GammaTransfer.AlphaExponent",
                L"GammaTransfer.AlphaOffset",
                L"GammaTransfer.RedAmplitude",
                L"GammaTransfer.RedExponent",
                L"GammaTransfer.RedOffset",
                L"GammaTransfer.GreenAmplitude",
                L"GammaTransfer.GreenExponent",
                L"GammaTransfer.GreenOffset",
                L"GammaTransfer.BlueAmplitude",
                L"GammaTransfer.BlueExponent",
                L"GammaTransfer.BlueOffset",
            };
            auto effectFactory = compositor.CreateEffectFactory(graphicsEffect, animatableProperties);
            auto effectBrush = effectFactory.CreateBrush();

            effectBrush.SetSourceParameter(L"backdrop", backdrop);

            CompositionBrush(effectBrush);
        }
    }

    void BackdropGammaTransferBrush::OnDisconnected()
    {
        // Dispose of composition resources when no longer in use.
        if (auto brush = CompositionBrush())
        {
            brush.Close();
            CompositionBrush(nullptr);
        }
    }
}