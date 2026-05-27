// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <memory>
#include <optional>
#endif
#include "AnimationBuilder.h"
#include "../Extensions/CompositorExtensions.h"
#include "../Extensions/DependencyObjectExtensions.h"
#include "../Extensions/EasingTypeExtensions.h"
#include "../Extensions/AnimationExtensions.h"
#include "../Options/RepeatOptionHelper.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    namespace
    {
        template<typename T>
        class AnimationFactory final : public ICompositionAnimationFactory, public IXamlAnimationFactory
        {
        public:
            AnimationFactory(
                winrt::hstring const& property,
                T const& to,
                std::optional<T> from,
                winrt::Windows::Foundation::TimeSpan delay,
                winrt::Windows::Foundation::TimeSpan duration,
                RepeatOption const& repeat,
                EasingType easingType,
                EasingMode easingMode)
                : property(property)
                , to(to)
                , from(from)
                , delay(delay)
                , duration(duration)
                , repeat(repeat)
                , easingType(easingType)
                , easingMode(easingMode)
            {
            }

            CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
            {
                auto easing = CompositorExtensions::TryCreateEasingFunction(targetHint.Compositor(), easingType, easingMode);
                auto [iterationBehavior, iterationCount] = RepeatOptionHelper::ToBehaviorAndCount(repeat);

                target = nullptr;

                if constexpr (std::is_same_v<T, bool>)
                {
                    return CompositorExtensions::CreateBooleanKeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    return CompositorExtensions::CreateScalarKeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, double>)
                {
                    std::optional<float> fromValue = from ? std::optional<float>(static_cast<float>(from.value())) : std::nullopt;

                    return CompositorExtensions::CreateScalarKeyFrameAnimation(
                        targetHint.Compositor(), property, static_cast<float>(to), fromValue, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::Numerics::float2>)
                {
                    return CompositorExtensions::CreateVector2KeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::Numerics::float3>)
                {
                    return CompositorExtensions::CreateVector3KeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::Numerics::float4>)
                {
                    return CompositorExtensions::CreateVector4KeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::UI::Color>)
                {
                    return CompositorExtensions::CreateColorKeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::Numerics::quaternion>)
                {
                    return CompositorExtensions::CreateQuaternionKeyFrameAnimation(
                        targetHint.Compositor(), property, to, from, delay, duration, easing,
                        AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
                }
                else
                {
                    throw winrt::hresult_invalid_argument(L"Invalid animation type");
                }
            }

            Timeline GetAnimation(DependencyObject const& targetHint) override
            {
                auto easing = EasingTypeExtensions::ToEasingFunction(easingType, easingMode);
                auto repeatBehavior = RepeatOptionHelper::ToRepeatBehavior(repeat);

                if constexpr (std::is_same_v<T, float>)
                {
                    std::optional<double> fromValue = from ? std::optional<double>(static_cast<double>(from.value())) : std::nullopt;
                    return DependencyObjectExtensions::CreateDoubleAnimation(targetHint, property, static_cast<double>(to), fromValue, delay, duration, easing, repeatBehavior, FillBehavior::HoldEnd, false, true);
                }
                else if constexpr (std::is_same_v<T, double>)
                {
                    return DependencyObjectExtensions::CreateDoubleAnimation(targetHint, property, to, from, delay, duration, easing, repeatBehavior, FillBehavior::HoldEnd, false, true);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::Point>)
                {
                    return DependencyObjectExtensions::CreatePointAnimation(targetHint, property, to, from, delay, duration, easing, repeatBehavior, FillBehavior::HoldEnd, false, true);
                }
                else if constexpr (std::is_same_v<T, winrt::Windows::UI::Color>)
                {
                    return DependencyObjectExtensions::CreateColorAnimation(targetHint, property, to, from, delay, duration, easing, repeatBehavior);
                }
                else
                {
                    throw winrt::hresult_invalid_argument(L"Invalid animation type");
                }
            }

        private:
            winrt::hstring property;
            T to;
            std::optional<T> from;
            winrt::Windows::Foundation::TimeSpan delay;
            winrt::Windows::Foundation::TimeSpan duration;
            RepeatOption repeat;
            EasingType easingType;
            EasingMode easingMode;
        };
        class CompositionClipScalarAnimation final : public ICompositionAnimationFactory
        {
        public:
            CompositionClipScalarAnimation(
                winrt::hstring const& property,
                float to,
                std::optional<float> from,
                winrt::Windows::Foundation::TimeSpan delay,
                winrt::Windows::Foundation::TimeSpan duration,
                RepeatOption const& repeat,
                EasingType easingType,
                EasingMode easingMode)
                : property(property)
                , to(to)
                , from(from)
                , delay(delay)
                , duration(duration)
                , repeat(repeat)
                , easingType(easingType)
                , easingMode(easingMode)
            {
            }

            CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
            {
                auto visual = targetHint.as<Visual>();
                auto clip = visual.Clip().try_as<InsetClip>();

                if (!clip)
                {
                    clip = visual.Compositor().CreateInsetClip();
                    visual.Clip(clip);
                }

                auto easing = CompositorExtensions::TryCreateEasingFunction(clip.Compositor(), easingType, easingMode);
                auto [iterationBehavior, iterationCount] = RepeatOptionHelper::ToBehaviorAndCount(repeat);

                target = clip;

                return CompositorExtensions::CreateScalarKeyFrameAnimation(
                    clip.Compositor(), property, to, from, delay, duration, easing,
                    AnimationExtensions::DefaultDelayBehavior(), AnimationDirection::Normal, iterationBehavior, iterationCount);
            }

        private:
            winrt::hstring property;
            float to;
            std::optional<float> from;
            winrt::Windows::Foundation::TimeSpan delay;
            winrt::Windows::Foundation::TimeSpan duration;
            RepeatOption repeat;
            EasingType easingType;
            EasingMode easingMode;
        };

        class XamlTransformDoubleAnimationFactory final : public IXamlAnimationFactory
        {
        public:
            XamlTransformDoubleAnimationFactory(
                winrt::hstring const& property,
                double to,
                std::optional<double> from,
                winrt::Windows::Foundation::TimeSpan delay,
                winrt::Windows::Foundation::TimeSpan duration,
                RepeatOption const& repeat,
                EasingType easingType,
                EasingMode easingMode)
                : property(property)
                , to(to)
                , from(from)
                , delay(delay)
                , duration(duration)
                , repeat(repeat)
                , easingType(easingType)
                , easingMode(easingMode)
            {
            }

            Timeline GetAnimation(DependencyObject const& targetHint) override
            {
                auto element = targetHint.as<UIElement>();
                auto transform = element.RenderTransform().try_as<CompositeTransform>();

                if (!transform)
                {
                    transform = CompositeTransform();
                    element.RenderTransform(transform);
                }

                return DependencyObjectExtensions::CreateDoubleAnimation(
                    transform,
                    property,
                    to,
                    from,
                    delay,
                    duration,
                    EasingTypeExtensions::ToEasingFunction(easingType, easingMode),
                    RepeatOptionHelper::ToRepeatBehavior(repeat));
            }

        private:
            winrt::hstring property;
            double to;
            std::optional<double> from;
            winrt::Windows::Foundation::TimeSpan delay;
            winrt::Windows::Foundation::TimeSpan duration;
            RepeatOption repeat;
            EasingType easingType;
            EasingMode easingMode;
        };
    }

    template<typename T>
    AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory(
        winrt::hstring const& property,
        T const& to,
        std::optional<T> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(std::make_unique<AnimationFactory<T>>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode));
    }

    template<typename T>
    AnimationBuilder& AnimationBuilder::AddXamlAnimationFactory(
        winrt::hstring const& property,
        T const& to,
        std::optional<T> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddXamlAnimationFactory(std::make_unique<AnimationFactory<T>>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode));
    }

    AnimationBuilder& AnimationBuilder::AddCompositionClipAnimationFactory(
        winrt::hstring const& property,
        float to,
        std::optional<float> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(std::make_unique<CompositionClipScalarAnimation>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode));
    }

    AnimationBuilder& AnimationBuilder::AddXamlTransformDoubleAnimationFactory(
        winrt::hstring const& property,
        double to,
        std::optional<double> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddXamlAnimationFactory(std::make_unique<XamlTransformDoubleAnimationFactory>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode));
    }

    template AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory<double>(
        winrt::hstring const&, double const&, std::optional<double>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);
    template AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory<winrt::Windows::Foundation::Numerics::float2>(
        winrt::hstring const&, winrt::Windows::Foundation::Numerics::float2 const&, std::optional<winrt::Windows::Foundation::Numerics::float2>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);
    template AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory<winrt::Windows::Foundation::Numerics::float3>(
        winrt::hstring const&, winrt::Windows::Foundation::Numerics::float3 const&, std::optional<winrt::Windows::Foundation::Numerics::float3>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);
    template AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory<winrt::Windows::Foundation::Numerics::quaternion>(
        winrt::hstring const&, winrt::Windows::Foundation::Numerics::quaternion const&, std::optional<winrt::Windows::Foundation::Numerics::quaternion>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);
    template AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory<winrt::Windows::UI::Color>(
        winrt::hstring const&, winrt::Windows::UI::Color const&, std::optional<winrt::Windows::UI::Color>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);

    template AnimationBuilder& AnimationBuilder::AddXamlAnimationFactory<double>(
        winrt::hstring const&, double const&, std::optional<double>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<winrt::Windows::Foundation::TimeSpan>, std::optional<RepeatOption>, EasingType, EasingMode);
}
