// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "TilesBrush.h"
#if __has_include("TilesBrush.g.cpp")
#include "TilesBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TilesBrush::TextureUriProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"TextureUri",
            winrt::xaml_typename<winrt::Windows::Foundation::Uri>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &TilesBrush::OnDependencyPropertyChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TilesBrush::DpiModeProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"DpiMode",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Media::DpiMode>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DpiMode::DisplayDpiWith96AsLowerBound), &TilesBrush::OnDependencyPropertyChanged });

    winrt::Windows::Foundation::Uri TilesBrush::TextureUri() const
    {
        return GetValue(TextureUriProperty()).try_as<winrt::Windows::Foundation::Uri>();
    }

    void TilesBrush::TextureUri(winrt::Windows::Foundation::Uri const& value)
    {
        SetValue(TextureUriProperty(), value);
    }

    DpiMode TilesBrush::DpiMode() const
    {
        return winrt::unbox_value<winrt::XamlToolkit::WinUI::Media::DpiMode>(GetValue(DpiModeProperty()));
    }

    void TilesBrush::DpiMode(winrt::XamlToolkit::WinUI::Media::DpiMode value)
    {
        SetValue(DpiModeProperty(), winrt::box_value(value));
    }

    void TilesBrush::OnDependencyPropertyChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::TilesBrush>();
		auto self = winrt::get_self<TilesBrush>(brush);
        if (self->CompositionBrush())
        {
            self->XamlCompositionEffectBrushBase::OnDisconnected();
            self->XamlCompositionEffectBrushBase::OnConnected();
        }
    }

    Pipelines::PipelineBuilder TilesBrush::OnPipelineRequested()
    {
        if (auto uri = TextureUri())
        {
            return Pipelines::PipelineBuilder::FromTiles(uri, DpiMode(), Media::CacheMode::Default);
        }
        return Pipelines::PipelineBuilder::FromColor(winrt::Windows::UI::Color{});
    }
}