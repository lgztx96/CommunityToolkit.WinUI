// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "AcrylicBrush.h"
#if __has_include("AcrylicBrush.g.cpp")
#include "AcrylicBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AcrylicBrush::BlurAmountProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"BlurAmount",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0), &AcrylicBrush::OnBlurAmountPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AcrylicBrush::TintColorProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"TintColor",
            winrt::xaml_typename<winrt::Windows::UI::Color>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(winrt::Windows::UI::Color{}), &AcrylicBrush::OnTintColorPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AcrylicBrush::TintOpacityProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"TintOpacity",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.5), &AcrylicBrush::OnTintOpacityPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AcrylicBrush::TextureUriProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"TextureUri",
            winrt::xaml_typename<winrt::Windows::Foundation::Uri>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &AcrylicBrush::OnTextureUriPropertyChanged });

    double AcrylicBrush::BlurAmount() const 
    {
        return winrt::unbox_value<double>(GetValue(BlurAmountProperty())); 
    }

    void AcrylicBrush::BlurAmount(double value)
    {
        SetValue(BlurAmountProperty(), winrt::box_value(std::max<double>(value, 0.0)));
    }

    winrt::Windows::UI::Color AcrylicBrush::TintColor() const 
    { 
        return winrt::unbox_value<winrt::Windows::UI::Color>(GetValue(TintColorProperty()));
    }

    void AcrylicBrush::TintColor(winrt::Windows::UI::Color value)
    {
        SetValue(TintColorProperty(), winrt::box_value(value));
    }

    double AcrylicBrush::TintOpacity() const
    { 
        return winrt::unbox_value<double>(GetValue(TintOpacityProperty())); 
    }

    void AcrylicBrush::TintOpacity(double value)
    {
        SetValue(TintOpacityProperty(), winrt::box_value(std::clamp(value, 0.0, 1.0)));
    }

    winrt::Windows::Foundation::Uri AcrylicBrush::TextureUri() const
    { 
        return GetValue(TextureUriProperty()).try_as<winrt::Windows::Foundation::Uri>(); 
    }

    void AcrylicBrush::TextureUri(winrt::Windows::Foundation::Uri const& value)
    {
        SetValue(TextureUriProperty(), value);
    }

    void AcrylicBrush::OnBlurAmountPropertyChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::AcrylicBrush>();
        auto self = winrt::get_self<implementation::AcrylicBrush>(brush);
        if (auto target = self->CompositionBrush())
        {
            if (self->_blurAmountSetter) 
            {
                self->_blurAmountSetter(target, static_cast<float>(winrt::unbox_value<double>(e.NewValue())));
            }
        }
    }

    void AcrylicBrush::OnTintColorPropertyChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::AcrylicBrush>();
        auto self = winrt::get_self<implementation::AcrylicBrush>(brush);
        if (auto target = self->CompositionBrush())
        {
            if (self->_tintColorSetter) 
            {
                self->_tintColorSetter(target, winrt::unbox_value<winrt::Windows::UI::Color>(e.NewValue()));
            }
        }
    }

    void AcrylicBrush::OnTintOpacityPropertyChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::AcrylicBrush>();
		auto self = winrt::get_self<implementation::AcrylicBrush>(brush);
        if (auto target = self->CompositionBrush())
        {
            if (self->_tintOpacitySetter)
            {
                self->_tintOpacitySetter(target, static_cast<float>(winrt::unbox_value<double>(e.NewValue())));
            }
        }
    }

    void AcrylicBrush::OnTextureUriPropertyChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::AcrylicBrush>();
		auto self = winrt::get_self<implementation::AcrylicBrush>(brush);
        if (self->CompositionBrush())
        {
            self->XamlCompositionEffectBrushBase::OnDisconnected();
            self->XamlCompositionEffectBrushBase::OnConnected();
        }
    }

    Pipelines::PipelineBuilder AcrylicBrush::OnPipelineRequested()
    {
        return winrt::make<Pipelines::implementation::PipelineBuilder>(Pipelines::PipelineBuilderImpl::FromBackdropAcrylic(
            TintColor(),
            _tintColorSetter,
            static_cast<float>(TintOpacity()),
            _tintOpacitySetter,
            static_cast<float>(BlurAmount()),
            _blurAmountSetter,
            TextureUri()));
    }
}