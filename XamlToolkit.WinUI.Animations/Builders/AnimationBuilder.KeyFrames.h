#pragma once

#include "AnimationBuilder.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.Xaml.h"
#include "TimedKeyFrameAnimationBuilder{T}.h"
#include "TimedKeyFrameAnimationBuilder{T}.Composition.h"
#include "TimedKeyFrameAnimationBuilder{T}.Xaml.h"
#include "Interfaces/INormalizedKeyFrameAnimationBuilder{T}.h"
#include "Interfaces/ITimedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <functional>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Media::Animation;

    /// <summary>
    /// Extension methods for AnimationBuilder to add keyframe animations.
    /// </summary>
    class AnimationBuilderKeyFrames
    {
    private:
        template<typename T, typename TCompositionCallback, typename TXamlCallback>
        static AnimationBuilder& NormalizedKeyFramesCore(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay,
            std::optional<TimeSpan> duration,
            std::optional<RepeatOption> repeatOption,
            std::optional<AnimationDelayBehavior> delayBehavior,
            FrameworkLayer layer,
            TCompositionCallback&& addCompositionAnimation,
            TXamlCallback&& addXamlAnimation)
        {
            if (layer == FrameworkLayer::Composition)
            {
                auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    duration.value_or(AnimationExtensions::DefaultDuration()),
                    repeatOption.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);
                addCompositionAnimation(keyFrameBuilder);
            }
            else
            {
                auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderXaml<T>(
                    property,
                    delay,
                    duration.value_or(AnimationExtensions::DefaultDuration()),
                    repeatOption.value_or(RepeatOptionHelper::Once()));

                build(keyFrameBuilder);
                addXamlAnimation(keyFrameBuilder);
            }

            return builder;
        }

        template<typename T, typename TCompositionCallback, typename TXamlCallback>
        static AnimationBuilder& TimedKeyFramesCore(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay,
            std::optional<RepeatOption> repeat,
            std::optional<AnimationDelayBehavior> delayBehavior,
            FrameworkLayer layer,
            TCompositionCallback&& addCompositionAnimation,
            TXamlCallback&& addXamlAnimation)
        {
            if (layer == FrameworkLayer::Composition)
            {
                auto keyFrameBuilder = TimedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    repeat.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);
                addCompositionAnimation(keyFrameBuilder);
            }
            else
            {
                auto keyFrameBuilder = TimedKeyFrameAnimationBuilderXaml<T>(
                    property,
                    delay,
                    repeat.value_or(RepeatOptionHelper::Once()));

                build(keyFrameBuilder);
                addXamlAnimation(keyFrameBuilder);
            }

            return builder;
        }

    public:
        /// <summary>
        /// Adds a custom animation based on normalized keyframes to the current schedule.
        /// </summary>
        /// <typeparam name="T">The type of values to animate.</typeparam>
        /// <param name="builder">The AnimationBuilder instance.</param>
        /// <param name="property">The target property to animate.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="duration">The animation duration.</param>
        /// <param name="repeatOption">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename T>
        static AnimationBuilder& NormalizedKeyFrames(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return NormalizedKeyFramesCore<T>(
                builder,
                property,
                std::move(build),
                delay,
                duration,
                repeatOption,
                delayBehavior,
                layer,
                [&builder](auto const& keyFrameBuilder)
                {
                    builder.AddCompositionAnimationFactory(keyFrameBuilder);
                },
                [&builder](auto const& keyFrameBuilder)
                {
                    builder.AddXamlAnimationFactory(keyFrameBuilder);
                });
        }

        /// <summary>
        /// Adds a custom animation based on normalized keyframes to the current schedule.
        /// </summary>
        /// <typeparam name="T">The type of values to animate.</typeparam>
        /// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
        /// <param name="builder">The AnimationBuilder instance.</param>
        /// <param name="property">The target property to animate.</param>
        /// <param name="state">The state to pass to the builder.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="duration">The animation duration.</param>
        /// <param name="repeatOption">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename T, typename TState>
        static AnimationBuilder& NormalizedKeyFrames(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            TState state,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            auto callback = [state, build](INormalizedKeyFrameAnimationBuilder<T>& keyframes)
            {
                build(keyframes, state);
            };

            return NormalizedKeyFrames<T>(builder, property, callback, delay, duration, repeatOption, delayBehavior, layer);
        }

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        /// <typeparam name="T">The type of values to animate.</typeparam>
        /// <param name="builder">The AnimationBuilder instance.</param>
        /// <param name="property">The target property to animate.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename T>
        static AnimationBuilder& TimedKeyFrames(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return TimedKeyFramesCore<T>(
                builder,
                property,
                std::move(build),
                delay,
                repeat,
                delayBehavior,
                layer,
                [&builder](auto const& keyFrameBuilder)
                {
                    builder.AddCompositionAnimationFactory(keyFrameBuilder);
                },
                [&builder](auto const& keyFrameBuilder)
                {
                    builder.AddXamlAnimationFactory(keyFrameBuilder);
                });
        }

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        /// <typeparam name="T">The type of values to animate.</typeparam>
        /// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
        /// <param name="builder">The AnimationBuilder instance.</param>
        /// <param name="property">The target property to animate.</param>
        /// <param name="state">The state to pass to the builder.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename T, typename TState>
        static AnimationBuilder& TimedKeyFrames(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            TState state,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            auto callback = [state, build](ITimedKeyFrameAnimationBuilder<T>& keyframes)
            {
                build(keyframes, state);
            };

            return TimedKeyFrames<T>(builder, property, callback, delay, repeat, delayBehavior, layer);
        }
    };
}
