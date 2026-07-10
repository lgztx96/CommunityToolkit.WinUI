// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "Pipelines.PipelineBuilder.g.h"
#include "PipelineBuilderImpl.h"

#include "../XamlToolkit.WinUI/common.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#else
import winrt.Windows.UI;
import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Media::Pipelines::implementation
{
    /// <summary>
    /// A WinRT wrapper for the PipelineBuilderImpl pure C++ class
    /// </summary>
    struct PipelineBuilder : PipelineBuilderT<PipelineBuilder>
    {
		PipelineBuilder() = default;

        PipelineBuilder(PipelineBuilderImpl const& impl) : _impl(impl) { }

        static winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder FromBackdrop()
        {
            return winrt::make<PipelineBuilder>(PipelineBuilderImpl::FromBackdrop());
        }

        static winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder FromColor(winrt::Windows::UI::Color color)
        {
            return winrt::make<PipelineBuilder>(PipelineBuilderImpl::FromColor(color));
        }

        static winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder FromImage(winrt::Uri const& uri, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
        {
            return winrt::make<PipelineBuilder>(PipelineBuilderImpl::FromImage(uri, dpiMode, cacheMode));
        }

        static winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder FromTiles(winrt::Uri const& uri, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
        {
            return winrt::make<PipelineBuilder>(PipelineBuilderImpl::FromTiles(uri, dpiMode, cacheMode));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Blur(float blur)
        {
            return winrt::make<PipelineBuilder>(_impl.Blur(blur));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Blur(float blur, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Blur(blur, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Saturation(float saturation)
        {
            return winrt::make<PipelineBuilder>(_impl.Saturation(saturation));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Saturation(float saturation, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Saturation(saturation, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Sepia(float intensity)
        {
            return winrt::make<PipelineBuilder>(_impl.Sepia(intensity));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Sepia(float intensity, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Sepia(intensity, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Opacity(float opacity)
        {
            return winrt::make<PipelineBuilder>(_impl.Opacity(opacity));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Opacity(float opacity, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Opacity(opacity, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Exposure(float amount)
        {
            return winrt::make<PipelineBuilder>(_impl.Exposure(amount));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Exposure(float amount, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Exposure(amount, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder HueRotation(float angle)
        {
            return winrt::make<PipelineBuilder>(_impl.HueRotation(angle));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder HueRotation(float angle, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.HueRotation(angle, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Tint(winrt::Windows::UI::Color color)
        {
            return winrt::make<PipelineBuilder>(_impl.Tint(color));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Tint(winrt::Windows::UI::Color color, winrt::hstring& id)
        {
            return winrt::make<PipelineBuilder>(_impl.Tint(color, id));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder TemperatureAndTint(float temperature, float tint)
        {
            return winrt::make<PipelineBuilder>(_impl.TemperatureAndTint(temperature, tint));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Shade(winrt::Windows::UI::Color color, float mix)
        {
            return winrt::make<PipelineBuilder>(_impl.Shade(color, mix));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder LuminanceToAlpha()
        {
            return winrt::make<PipelineBuilder>(_impl.LuminanceToAlpha());
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Invert()
        {
            return winrt::make<PipelineBuilder>(_impl.Invert());
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Grayscale()
        {
            return winrt::make<PipelineBuilder>(_impl.Grayscale());
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder Blend(winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder const& pipeline, winrt::Microsoft::Graphics::Canvas::Effects::BlendEffectMode mode, Media::Placement placement)
        {
            auto pipelineImpl = winrt::get_self<PipelineBuilder>(pipeline)->get_strong();
            return winrt::make<PipelineBuilder>(_impl.Blend(pipelineImpl->_impl, mode, placement));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder CrossFade(winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder const& pipeline, float factor)
        {
            auto pipelineImpl = winrt::get_self<PipelineBuilder>(pipeline)->get_strong();
            return winrt::make<PipelineBuilder>(_impl.CrossFade(pipelineImpl->_impl, factor));
        }

        winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder CrossFade(winrt::XamlToolkit::WinUI::Media::Pipelines::PipelineBuilder const& pipeline, float factor, winrt::hstring& id)
        {
            auto pipelineImpl = winrt::get_self<PipelineBuilder>(pipeline)->get_strong();
            return winrt::make<PipelineBuilder>(_impl.CrossFade(pipelineImpl->_impl, factor, id));
        }

        winrt::IAsyncOperation<winrt::CompositionBrush> BuildAsync()
        {
            return _impl.BuildAsync();
        }

        winrt::IAsyncOperation<winrt::SpriteVisual> AttachAsync(winrt::UIElement const& target, winrt::UIElement const& reference)
        {
			return _impl.AttachAsync(target, reference);
        }

		winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush AsBrush() const
		{
			return _impl.AsBrush();
		}

    private:
        PipelineBuilderImpl _impl;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::Pipelines::factory_implementation
{
    struct PipelineBuilder : PipelineBuilderT<PipelineBuilder, implementation::PipelineBuilder>
    {
    };
}