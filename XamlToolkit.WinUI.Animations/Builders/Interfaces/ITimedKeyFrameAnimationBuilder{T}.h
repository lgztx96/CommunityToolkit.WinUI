#pragma once

#include "../../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An interface for an animation builder using timed keyframes.
    /// </summary>
    /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
    template<typename T>
    class ITimedKeyFrameAnimationBuilder
    {
    public:
        /// <summary>
        /// Adds a new timed keyframe to the builder in use.
        /// </summary>
        /// <param name="progress">The timed progress for the keyframe, relative to the start of the animation.</param>
        /// <param name="value">The value for the new keyframe to add.</param>
        /// <param name="easingType">The easing type to use to reach the new keyframe.</param>
        /// <param name="easingMode">The easing mode to use to reach the new keyframe.</param>
        /// <returns>The same <see cref="ITimedKeyFrameAnimationBuilder{T}"/> instance that the method was invoked upon.</returns>
        virtual ITimedKeyFrameAnimationBuilder<T>& KeyFrame(
            winrt::TimeSpan progress,
            T const& value,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) = 0;

        /// <summary>
        /// Adds a new timed expressionkeyframe to the builder in use.
        /// This method can only be used when the animation being built targets the composition layer.
        /// </summary>
        /// <param name="progress">The timed progress for the keyframe, relative to the start of the animation.</param>
        /// <param name="expression">The expression for the new keyframe to add.</param>
        /// <param name="easingType">The easing type to use to reach the new keyframe.</param>
        /// <param name="easingMode">The easing mode to use to reach the new keyframe.</param>
        /// <returns>The same <see cref="ITimedKeyFrameAnimationBuilder{T}"/> instance that the method was invoked upon.</returns>
        /// <exception cref="InvalidOperationException">Thrown when the animation being built targets the XAML layer.</exception>
        virtual ITimedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            winrt::TimeSpan progress,
            winrt::hstring const& expression,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) = 0;

        virtual ~ITimedKeyFrameAnimationBuilder() = default;
    };
}
