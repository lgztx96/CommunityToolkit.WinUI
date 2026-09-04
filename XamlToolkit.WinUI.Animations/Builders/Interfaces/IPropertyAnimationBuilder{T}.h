#pragma once

#include "../AnimationBuilder.h"
#include "INormalizedKeyFrameAnimationBuilder{T}.h"
#include "ITimedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#include <functional>
#include <optional>
#else
import winrt.XamlToolkit.WinUI.Animations;
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Composition;
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
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) = 0;

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
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) = 0;

        virtual ~IPropertyAnimationBuilder() = default;
    };
}
