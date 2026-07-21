// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    PipelineBuilderImpl PipelineBuilderImpl::Blur(float blur, winrt::hstring& target, EffectBorderMode mode, EffectOptimization optimization) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".BlurAmount";
        auto factory = [state = _state, blur, mode, optimization, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasGaussianBlurEffect effect;
            effect.BlurAmount(blur);
            effect.BorderMode(mode);
            effect.Optimization(optimization);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::CrossFade(PipelineBuilderImpl const& pipeline, float factor, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".CrossFade";
        auto otherState = pipeline._state;
        auto factory = [state = _state, otherState, factor, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasCrossFadeEffect effect;
            effect.CrossFade(factor);
            effect.Name(name);

            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await otherState->_sourceProducer();

            effect.Source1(source1);
            effect.Source2(source2);
            co_return effect;
        };

        return PipelineBuilderImpl(std::move(factory), *this, pipeline, { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Exposure(float amount, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Exposure";
        auto factory = [state = _state, amount, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasExposureEffect effect;
            effect.Exposure(amount);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::HueRotation(float angle, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Angle";
        auto factory = [state = _state, angle, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasHueRotationEffect effect;
            effect.Angle(angle);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Opacity(float opacity, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Opacity";
        auto factory = [state = _state, opacity, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasOpacityEffect effect;
            effect.Opacity(opacity);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Saturation(float saturation, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Saturation";
        auto factory = [state = _state, saturation, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasSaturationEffect effect;
            effect.Saturation(saturation);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Sepia(float intensity, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Intensity";
        auto factory = [state = _state, intensity, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasSepiaEffect effect;
            effect.Intensity(intensity);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Tint(winrt::Windows::UI::Color color, winrt::hstring& target) const
    {
        winrt::hstring name = GenerateId();
        target = name + L".Color";
        auto factory = [state = _state, color, name]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasTintEffect effect;
            effect.Color(color);
            effect.Name(name);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory), { target });
    }
}