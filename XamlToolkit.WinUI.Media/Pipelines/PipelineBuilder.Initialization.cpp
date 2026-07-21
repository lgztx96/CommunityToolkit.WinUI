// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"
#include "../Helpers/SurfaceLoader.h"
#include "../Helpers/CompositionObjectCache.h"
#include "../Extensions/Windows.UI.Composition/CompositionObjectExtensions.h"
#include "Extensions/System/UriExtensions.h"

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::XamlToolkit::WinUI::Media::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    namespace
    {
        // Cache managers for backdrop brushes
        static CompositionObjectCache<CompositionBrush> BackdropBrushCache;
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBackdrop()
    {
        auto factory = []() -> winrt::IAsyncOperation<winrt::CompositionBrush>
        {
            auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();
			auto brush = compositor.CreateBackdropBrush();
            // auto brush = BackdropBrushCache.GetValue(compositor, [](Compositor const& c) { return c.CreateBackdropBrush(); });
            co_return brush;
        };

        return PipelineBuilderImpl(std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromColor(winrt::Windows::UI::Color color)
    {
        return PipelineBuilderImpl([color]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.Color(color);
            co_return effect;
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromColor(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& setter)
    {
        winrt::hstring id = GenerateId();

        auto factory = [color, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.Color(color);
            effect.Name(id);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".Color";
        setter = [propertyName](winrt::CompositionBrush const& brush, winrt::Windows::UI::Color const& value)
        {
            brush.Properties().InsertColor(propertyName, value);
        };

        return PipelineBuilderImpl(std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromColor(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& animation)
    {
        winrt::hstring id = GenerateId();

        auto factory = [color, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.Color(color);
            effect.Name(id);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Color";
        animation = [propertyName](winrt::CompositionBrush const& brush, winrt::Windows::UI::Color const& value, winrt::TimeSpan const& duration) -> winrt::IAsyncAction
        {
            co_return co_await Media::Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromHdrColor(winrt::float4 color)
    {
        return PipelineBuilderImpl([color]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.ColorHdr(color);
            co_return effect;
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromHdrColor(winrt::float4 color, EffectSetter<winrt::float4>& setter)
    {
        winrt::hstring id = GenerateId();

        auto factory = [color, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.ColorHdr(color);
            effect.Name(id);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".ColorHdr";
        setter = [propertyName](winrt::CompositionBrush const& brush, winrt::float4 const& value)
        {
            brush.Properties().InsertVector4(propertyName, value);
        };

        return PipelineBuilderImpl(std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromHdrColor(winrt::float4 color, EffectAnimation<winrt::float4>& animation)
    {
        winrt::hstring id = GenerateId();

        auto factory = [color, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasColorSourceEffect effect;
            effect.ColorHdr(color);
            effect.Name(id);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".ColorHdr";
        animation = [propertyName](winrt::CompositionBrush const& brush, winrt::float4 const& value, winrt::TimeSpan const& duration) -> winrt::IAsyncAction
        {
            co_return co_await Media::Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBrush(CompositionBrush const& brush)
    {
        return PipelineBuilderImpl([brush]() -> winrt::IAsyncOperation<winrt::CompositionBrush>
        {
            co_return brush;
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBrush(std::function<CompositionBrush()> factory)
    {
        return PipelineBuilderImpl([factory]() -> winrt::IAsyncOperation<winrt::CompositionBrush>
        {
            co_return factory();
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBrush(std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()> factory)
    {
        return PipelineBuilderImpl(factory);
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromEffect(IGraphicsEffectSource const& effect)
    {
        return PipelineBuilderImpl([effect]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            co_return effect;
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromEffect(std::function<IGraphicsEffectSource()> factory)
    {
        return PipelineBuilderImpl([factory]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            co_return factory();
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromEffect(std::function<winrt::IAsyncOperation<winrt::IGraphicsEffectSource>()> factory)
    {
        return PipelineBuilderImpl(factory);
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromImage(winrt::hstring const& relativePath, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
    {
        return FromImage(UriExtensions::ToAppxUri(relativePath), dpiMode, cacheMode);
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromImage(winrt::Uri const& uri, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
    {
        return PipelineBuilderImpl([uri, dpiMode, cacheMode]() -> winrt::IAsyncOperation<winrt::CompositionBrush>
        {
            co_return co_await SurfaceLoader::LoadImageAsync(uri, dpiMode, cacheMode);
        });
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromTiles(winrt::hstring const& relativePath, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
    {
        return FromTiles(UriExtensions::ToAppxUri(relativePath), dpiMode, cacheMode);
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromTiles(winrt::Uri const& uri, Media::DpiMode dpiMode, Media::CacheMode cacheMode)
    {
        auto imagePipeline = FromImage(uri, dpiMode, cacheMode);
        auto sharedState = imagePipeline._state;

        auto factory = [sharedState]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasBorderEffect effect;
            effect.ExtendX(winrt::CanvasEdgeBehavior::Wrap);
            effect.ExtendY(winrt::CanvasEdgeBehavior::Wrap);
            const auto& source = co_await sharedState->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(imagePipeline, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromUIElement(winrt::UIElement const& element)
    {
        return PipelineBuilderImpl([element]() -> winrt::IAsyncOperation<winrt::CompositionBrush>
        {
            auto compositor = winrt::ElementCompositionPreview::GetElementVisual(element).Compositor();
            co_return compositor.CreateBackdropBrush();
        });
    }
}