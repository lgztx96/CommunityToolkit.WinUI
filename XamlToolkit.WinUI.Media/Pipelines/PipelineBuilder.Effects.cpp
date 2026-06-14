// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBuilderImpl.h"
#include "../Extensions/Windows.UI.Composition/CompositionObjectExtensions.h"

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    PipelineBuilderImpl PipelineBuilderImpl::Blur(float blur, EffectBorderMode mode, EffectOptimization optimization) const
    {
        auto factory = [state = _state, blur, mode, optimization]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasGaussianBlurEffect effect;
            effect.BlurAmount(blur);
            effect.BorderMode(mode);
            effect.Optimization(optimization);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Blur(float blur, EffectSetter<float>& setter, EffectBorderMode mode, EffectOptimization optimization) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, blur, mode, optimization, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasGaussianBlurEffect effect;
            effect.BlurAmount(blur);
            effect.BorderMode(mode);
            effect.Optimization(optimization);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".BlurAmount";

        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Blur(float blur, EffectAnimation<float>& animation, EffectBorderMode mode, EffectOptimization optimization) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, blur, mode, optimization, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasGaussianBlurEffect effect;
            effect.BlurAmount(blur);
            effect.BorderMode(mode);
            effect.Optimization(optimization);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".BlurAmount";

        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Saturation(float const saturation) const
    {
        auto factory = [state = _state, saturation]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSaturationEffect effect;
            effect.Saturation(saturation);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Saturation(float saturation, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, saturation, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSaturationEffect effect;
            effect.Saturation(saturation);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Saturation";
        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Saturation(float saturation, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, saturation, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSaturationEffect effect;
            effect.Saturation(saturation);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Saturation";
        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Sepia(float intensity) const
    {
        auto factory = [state = _state, intensity]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSepiaEffect effect;
            effect.Intensity(intensity);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Sepia(float intensity, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, intensity, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSepiaEffect effect;
            effect.Intensity(intensity);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Intensity";
        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Sepia(float intensity, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, intensity, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasSepiaEffect effect;
            effect.Intensity(intensity);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Intensity";
        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Opacity(float opacity) const
    {
        auto factory = [state = _state, opacity]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasOpacityEffect effect;
            effect.Opacity(opacity);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Opacity(float opacity, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, opacity, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasOpacityEffect effect;
            effect.Opacity(opacity);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Opacity";
        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Opacity(float opacity, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, opacity, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasOpacityEffect effect;
            effect.Opacity(opacity);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".Opacity";
        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Exposure(float amount) const
    {
        auto factory = [state = _state, amount]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasExposureEffect effect;
            effect.Exposure(amount);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Exposure(float amount, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, amount, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasExposureEffect effect;
            effect.Exposure(amount);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Exposure";
        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Exposure(float amount, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, amount, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasExposureEffect effect;
            effect.Exposure(amount);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".Exposure";
        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::HueRotation(float angle) const
    {
        auto factory = [state = _state, angle]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasHueRotationEffect effect;
            effect.Angle(angle);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::HueRotation(float angle, EffectSetter<float>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, angle, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasHueRotationEffect effect;
            effect.Angle(angle);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Angle";
        setter = [propertyName](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::HueRotation(float angle, EffectAnimation<float>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, angle, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasHueRotationEffect effect;
            effect.Angle(angle);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".Angle";
        animation = [propertyName](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Tint(winrt::Windows::UI::Color color) const
    {
        auto factory = [state = _state, color]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTintEffect effect;
            effect.Color(color);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Tint(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& setter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, color, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTintEffect effect;
            effect.Color(color);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring propertyName = id + L".Color";
        setter = [propertyName](CompositionBrush const& brush, winrt::Windows::UI::Color const& value)
        {
            brush.Properties().InsertColor(propertyName, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Tint(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& animation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, color, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTintEffect effect;
            effect.Color(color);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        winrt::hstring propertyName = id + L".Color";
        animation = [propertyName](CompositionBrush const& brush, winrt::Windows::UI::Color const& value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, propertyName, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { propertyName });
    }

    PipelineBuilderImpl PipelineBuilderImpl::TemperatureAndTint(float temperature, float tint)  const
    {
        auto factory = [state = _state, temperature, tint]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTemperatureAndTintEffect effect;
            effect.Temperature(temperature);
            effect.Tint(tint);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::TemperatureAndTint(float temperature, EffectSetter<float>& temperatureSetter, float tint, EffectSetter<float>& tintSetter) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, temperature, tint, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTemperatureAndTintEffect effect;
            effect.Temperature(temperature);
            effect.Tint(tint);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

		winrt::hstring temperatureProperty = id + L".Temperature";
		winrt::hstring tintProperty = id + L".Tint";
        temperatureSetter = [temperatureProperty](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(temperatureProperty, value);
        };

        tintSetter = [tintProperty](CompositionBrush const& brush, float value)
        {
            brush.Properties().InsertScalar(tintProperty, value);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { temperatureProperty, tintProperty });
    }

    PipelineBuilderImpl PipelineBuilderImpl::TemperatureAndTint(float temperature, EffectAnimation<float>& temperatureAnimation, float tint, EffectAnimation<float>& tintAnimation) const
    {
        winrt::hstring id = GenerateId();
        auto factory = [state = _state, temperature, tint, id]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasTemperatureAndTintEffect effect;
            effect.Temperature(temperature);
            effect.Tint(tint);
            effect.Name(id);
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        winrt::hstring temperatureProperty = id + L".Temperature";
        winrt::hstring tintProperty = id + L".Tint";
        temperatureAnimation = [temperatureProperty](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, temperatureProperty, value, duration);
        };

        tintAnimation = [tintProperty](CompositionBrush const& brush, float value, TimeSpan const& duration) -> IAsyncAction
        {
            co_return co_await Extensions::CompositionObjectExtensions::StartAnimationAsync(brush, tintProperty, value, duration);
        };

        return PipelineBuilderImpl(*this, std::move(factory), { temperatureProperty, tintProperty });
    }

    PipelineBuilderImpl PipelineBuilderImpl::Shade(winrt::Windows::UI::Color color, float mix) const
    {
        return FromColor(color).CrossFade(*this, mix);
    }

    PipelineBuilderImpl PipelineBuilderImpl::Shade(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& colorSetter, float mix, EffectSetter<float>& mixSetter) const
    {
        EffectSetter<winrt::Windows::UI::Color> colorSetterLocal;
        auto pipeline = FromColor(color, colorSetterLocal);
        colorSetter = colorSetterLocal;
        return pipeline.CrossFade(*this, mix, mixSetter);
    }

    PipelineBuilderImpl PipelineBuilderImpl::Shade(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& colorAnimation, float mix, EffectAnimation<float>& mixAnimation) const
    {
        EffectAnimation<winrt::Windows::UI::Color> colorAnimationLocal;
        auto pipeline = FromColor(color, colorAnimationLocal);
        colorAnimation = colorAnimationLocal;
        return pipeline.CrossFade(*this, mix, mixAnimation);
    }

    PipelineBuilderImpl PipelineBuilderImpl::LuminanceToAlpha() const
    {
        auto factory = [state = _state]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasLuminanceToAlphaEffect effect;
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Invert() const
    {
        auto factory = [state = _state]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasInvertEffect effect;
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Grayscale() const
    {
        auto factory = [state = _state]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            CanvasGrayscaleEffect effect;
            const auto& source = co_await state->_sourceProducer();
            effect.Source(source);
            co_return effect;
        };

        return PipelineBuilderImpl(*this, std::move(factory));
    }

    PipelineBuilderImpl PipelineBuilderImpl::Effect(
        std::function<IGraphicsEffectSource(IGraphicsEffectSource const&)> factory,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers) const
    {
        auto effectFactory = [state = _state, factory]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            const auto& source = co_await state->_sourceProducer();
            co_return factory(source);
        };

        return PipelineBuilderImpl(*this, std::move(effectFactory), animations, initializers);
    }

    PipelineBuilderImpl PipelineBuilderImpl::Effect(
        std::function<IAsyncOperation<IGraphicsEffectSource>(IGraphicsEffectSource const&)> factory,
        std::vector<winrt::hstring> animations,
        std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers) const
    {
        auto effectFactory = [state = _state, factory]() -> IAsyncOperation<IGraphicsEffectSource>
        {
            const auto& source = co_await state->_sourceProducer();
            co_return co_await factory(source);
        };

        return PipelineBuilderImpl(*this, std::move(effectFactory), animations, initializers);
    }
}