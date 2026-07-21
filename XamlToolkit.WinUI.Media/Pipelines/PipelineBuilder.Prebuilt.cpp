// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    PipelineBuilderImpl PipelineBuilderImpl::FromBackdropAcrylic(
        winrt::Windows::UI::Color tintColor,
        float tintOpacity,
        float blurAmount,
        winrt::Uri const& noiseUri,
        Media::CacheMode cacheMode)
    {
        auto pipeline = FromBackdrop().Shade(tintColor, tintOpacity).Blur(blurAmount);

        if (noiseUri)
        {
            return pipeline.Blend(FromTiles(
                noiseUri, 
                Media::DpiMode::DisplayDpiWith96AsLowerBound, 
                cacheMode), 
                BlendEffectMode::Overlay);
        }

        return pipeline;
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBackdropAcrylic(
        winrt::Windows::UI::Color tintColor,
        EffectSetter<winrt::Windows::UI::Color>& tintColorSetter,
        float tintOpacity,
        EffectSetter<float>& tintOpacitySetter,
        float blurAmount,
        EffectSetter<float>& blurAmountSetter,
        winrt::Uri const& noiseUri,
        Media::CacheMode cacheMode)
    {
        auto pipeline = FromBackdrop()
            .Shade(tintColor, tintColorSetter, tintOpacity, tintOpacitySetter)
            .Blur(blurAmount, blurAmountSetter);

        if (noiseUri)
        {
            return pipeline.Blend(FromTiles(
                noiseUri, 
                Media::DpiMode::DisplayDpiWith96AsLowerBound, 
                cacheMode), 
                BlendEffectMode::Overlay);
        }

        return pipeline;
    }

    PipelineBuilderImpl PipelineBuilderImpl::FromBackdropAcrylic(
        winrt::Windows::UI::Color tintColor,
        EffectAnimation<winrt::Windows::UI::Color>& tintAnimation,
        float tintOpacity,
        EffectAnimation<float>& tintOpacityAnimation,
        float blurAmount,
        EffectAnimation<float>& blurAmountAnimation,
        winrt::Uri const& noiseUri,
        Media::CacheMode cacheMode)
    {
        auto pipeline = FromBackdrop()
            .Shade(tintColor, tintAnimation, tintOpacity, tintOpacityAnimation)
            .Blur(blurAmount, blurAmountAnimation);

        if (noiseUri)
        {
            return pipeline.Blend(FromTiles(
                noiseUri, 
                Media::DpiMode::DisplayDpiWith96AsLowerBound, 
                cacheMode), 
                BlendEffectMode::Overlay);
        }

        return pipeline;
    }
}