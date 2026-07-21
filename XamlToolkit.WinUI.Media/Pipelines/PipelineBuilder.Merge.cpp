// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"
#include "../Extensions/Windows.UI.Composition/CompositionObjectExtensions.h"

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    PipelineBuilderImpl PipelineBuilderImpl::Blend(
        PipelineBuilderImpl const& pipeline, 
        BlendEffectMode mode, 
        winrt::XamlToolkit::WinUI::Media::Placement placement) const
    {
        std::shared_ptr<State> fgState;
        std::shared_ptr<State> bgState;

        if (placement == winrt::XamlToolkit::WinUI::Media::Placement::Foreground)
        {
            fgState = pipeline._state;
            bgState = _state;
        }
        else
        {
            fgState = _state;
            bgState = pipeline._state;
        }

        auto factory = [fgState, bgState, mode]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasBlendEffect effect;
            effect.Mode(mode);

            auto fgSource = co_await fgState->_sourceProducer();
            auto bgSource = co_await bgState->_sourceProducer();

            effect.Foreground(fgSource);
            effect.Background(bgSource);
            co_return effect;
        };

        return PipelineBuilderImpl(std::move(factory), fgState, bgState);
    }

    PipelineBuilderImpl PipelineBuilderImpl::CrossFade(PipelineBuilderImpl const& pipeline, float factor) const
    {
        auto otherState = pipeline._state;
        auto factory = [state = _state, otherState, factor]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasCrossFadeEffect effect;
            effect.CrossFade(factor);

            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await otherState->_sourceProducer();

            effect.Source1(source1);
            effect.Source2(source2);
            co_return effect;
        };

        return PipelineBuilderImpl(std::move(factory), *this, pipeline);
    }

    PipelineBuilderImpl PipelineBuilderImpl::CrossFade(PipelineBuilderImpl const& pipeline, float factor, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto otherState = pipeline._state;
        auto factory = [state = _state, otherState, factor, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasCrossFadeEffect effect;
            effect.CrossFade(factor);
            effect.Name(id);

            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await otherState->_sourceProducer();

            effect.Source1(source1);
            effect.Source2(source2);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".CrossFade";
        setter = [propertyName](winrt::CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(std::move(factory), *this, pipeline, { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::CrossFade(PipelineBuilderImpl const& pipeline, float factor, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto otherState = pipeline._state;
        auto factory = [state = _state, otherState, factor, id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            CanvasCrossFadeEffect effect;
            effect.CrossFade(factor);
            effect.Name(id);

            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await otherState->_sourceProducer();

            effect.Source1(source1);
            effect.Source2(source2);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".CrossFade";
        animation = [propertyName](winrt::CompositionBrush const& brush, float value, winrt::TimeSpan const& duration) -> winrt::IAsyncAction
        {
            co_return co_await Media::Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(std::move(factory), *this, pipeline, { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Merge(
        std::function<winrt::IGraphicsEffectSource(winrt::IGraphicsEffectSource const&, winrt::IGraphicsEffectSource const&)> factory,
        PipelineBuilderImpl const& background,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()>> initializers) const
    {
        auto bgState = background._state;
        auto effectFactory = [state = _state, bgState, factory]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await bgState->_sourceProducer();
            co_return factory(source1, source2);
        };

        return PipelineBuilderImpl(std::move(effectFactory), *this, background, animations, initializers);
    }

    PipelineBuilderImpl PipelineBuilderImpl::Merge(
        std::function<winrt::IAsyncOperation<winrt::IGraphicsEffectSource>(winrt::IGraphicsEffectSource const&, winrt::IGraphicsEffectSource const&)> factory,
        PipelineBuilderImpl const& background,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()>> initializers) const
    {
        auto bgState = background._state;
        auto effectFactory = [state = _state, bgState, factory]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            auto source1 = co_await state->_sourceProducer();
            auto source2 = co_await bgState->_sourceProducer();
            co_return co_await factory(source1, source2);
        };

        return PipelineBuilderImpl(std::move(effectFactory), *this, background, animations, initializers);
    }
}