#pragma once

#include "../../Options/RepeatOptionHelper.h"
#include "../AnimationBuilder.h"
#include "INormalizedKeyFrameAnimationBuilder{T}.h"
#include "ITimedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <functional>
#include <optional>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An animation for an animation builder using keyframes, targeting a specific property.
    /// </summary>
    /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
    template<typename T>
    class IPropertyAnimationBuilder
    {
    public:
        /// <summary>
        /// Adds a custom animation based on normalized keyframes ot the current schedule.
        /// </summary>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="duration">The animation duration.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if the animation is not being executed on the composition layer).</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        virtual AnimationBuilder& NormalizedKeyFrames(
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) = 0;

        /// <summary>
        /// Adds a custom animation based on normalized keyframes ot the current schedule.
        /// </summary>
        /// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
        /// <param name="state">The state to pass to the builder.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="duration">The animation duration.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if the animation is not being executed on the composition layer).</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename TState>
        AnimationBuilder& NormalizedKeyFrames(
            TState state,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt)
        {
            auto callback = [state, build](INormalizedKeyFrameAnimationBuilder<T>& builder)
            {
                build(builder, state);
            };

            return NormalizedKeyFrames(callback, delay, duration, repeat, delayBehavior);
        }

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if the animation is not being executed on the composition layer).</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        virtual AnimationBuilder& TimedKeyFrames(
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) = 0;

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        /// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
        /// <param name="state">The state to pass to the builder.</param>
        /// <param name="build">The callback to use to construct the custom animation.</param>
        /// <param name="delay">The optional initial delay for the animation.</param>
        /// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
        /// <param name="delayBehavior">The delay behavior to use (ignored if the animation is not being executed on the composition layer).</param>
        /// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
        template<typename TState>
        AnimationBuilder& TimedKeyFrames(
            TState state,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt)
        {
            auto callback = [state, build](ITimedKeyFrameAnimationBuilder<T>& builder)
            {
                build(builder, state);
            };

            return TimedKeyFrames(callback, delay, repeat, delayBehavior);
        }

        virtual ~IPropertyAnimationBuilder() = default;
    };
}
