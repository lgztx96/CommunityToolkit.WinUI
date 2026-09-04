// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"
#include "../Extensions/Windows.UI.Composition/CompositionObjectExtensions.h"
#include "PipelineBuilder.h"
#if __has_include("Pipelines.PipelineBuilder.g.cpp")
#include "Pipelines.PipelineBuilder.g.cpp"
#endif

namespace winrt 
{
    using namespace winrt::Windows::Graphics::Effects;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    namespace
    {
        winrt::hstring GuidToUppercaseAscii(winrt::guid const& guid)
        {
            return winrt::format(
                L"_{:08X}{:04X}{:04X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                guid.Data1,
                guid.Data2,
                guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
            );
        }
    }

    winrt::hstring PipelineBuilderImpl::GenerateId()
    {
        return GuidToUppercaseAscii(winrt::GuidHelper::CreateNewGuid());
    }

    PipelineBuilderImpl::PipelineBuilderImpl(std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()> factory)
    {
        _state = std::make_shared<State>();
        winrt::hstring id = GenerateId();
        _state->_sourceProducer = [id]() -> winrt::IAsyncOperation<winrt::IGraphicsEffectSource>
        {
            co_return winrt::CompositionEffectSourceParameter(id);
        };
		_state->_lazyParameters.emplace(id, std::move(factory));
    }

    PipelineBuilderImpl::PipelineBuilderImpl(
        std::function<winrt::IAsyncOperation<winrt::IGraphicsEffectSource>()> factory,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()>> lazy)
    {
        _state = std::make_shared<State>();
        _state->_sourceProducer = std::move(factory);
        _state->_animationProperties = std::move(animations);
        _state->_lazyParameters = std::move(lazy);
    }

    PipelineBuilderImpl::PipelineBuilderImpl(
        PipelineBuilderImpl const& source,
        std::function<winrt::IAsyncOperation<winrt::IGraphicsEffectSource>()> factory,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()>> lazy)
    {
        _state = std::make_shared<State>();
        _state->_sourceProducer = std::move(factory);
        _state->_animationProperties = animations.empty()
            ? source._state->_animationProperties
            : Merge(std::move(animations), source._state->_animationProperties);
        _state->_lazyParameters = lazy.empty()
            ? source._state->_lazyParameters
            : Merge(std::move(lazy), source._state->_lazyParameters);
    }

    PipelineBuilderImpl::PipelineBuilderImpl(
        std::function<winrt::IAsyncOperation<winrt::IGraphicsEffectSource>()> factory,
        PipelineBuilderImpl const& a,
        PipelineBuilderImpl const& b,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<winrt::IAsyncOperation<winrt::CompositionBrush>()>> lazy)
    {
        _state = std::make_shared<State>();
        _state->_sourceProducer = std::move(factory);

        auto mergedAnimations = Merge(a._state->_animationProperties, b._state->_animationProperties);
        _state->_animationProperties = animations.empty()
            ? std::move(mergedAnimations)
            : Merge(std::move(animations), std::move(mergedAnimations));

        auto mergedLazy = Merge(a._state->_lazyParameters, b._state->_lazyParameters);
        _state->_lazyParameters = lazy.empty()
            ? std::move(mergedLazy)
            : Merge(std::move(lazy), std::move(mergedLazy));
    }

    winrt::IAsyncOperation<winrt::CompositionBrush> PipelineBuilderImpl::BuildAsync()
    {
        auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

        const auto& effectSource = co_await _state->_sourceProducer();
        auto effect = effectSource.try_as<winrt::IGraphicsEffect>();

        // Validate the pipeline
        if (!effect)
        {
            throw winrt::hresult_error(E_FAIL, L"The pipeline doesn't contain a valid effects sequence");
        }

        winrt::CompositionEffectFactory factory{ nullptr };
        if (!_state->_animationProperties.empty())
        {
            factory = compositor.CreateEffectFactory(effect, _state->_animationProperties);
        }
        else
        {
            factory = compositor.CreateEffectFactory(effect);
        }

        // Create the effect brush and apply parameters
        auto effectBrush = factory.CreateBrush();
        for (const auto& [key, valueProducer] : _state->_lazyParameters)
        {
            const auto& brush = co_await valueProducer();
            effectBrush.SetSourceParameter(key, brush);
        }

        co_return effectBrush;
    }

    winrt::IAsyncOperation<winrt::SpriteVisual> PipelineBuilderImpl::AttachAsync(winrt::UIElement target, winrt::UIElement reference)
    {
        auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

        winrt::SpriteVisual visual = compositor.CreateSpriteVisual();

        visual.Brush(co_await BuildAsync());

        winrt::ElementCompositionPreview::SetElementChildVisual(target, visual);

        if (reference)
        {
            if (reference == target)
            {
                visual.RelativeSizeAdjustment(winrt::float2{ 1.0f, 1.0f });
            }
            else
            {
                winrt::XamlToolkit::WinUI::Media::Extensions::CompositionObjectExtensions::BindSize(visual, reference);
            }
        }

        co_return visual;
    }

    winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush PipelineBuilderImpl::AsBrush() const
    {
		return winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush(winrt::make<implementation::PipelineBuilder>(*this));
    }
}