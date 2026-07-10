#pragma once

#include "../Builders/Interfaces/ITimedKeyFrameAnimationBuilder{T}.h"
#include "../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An extension class for the <see cref="ITimedKeyFrameAnimationBuilder{T}"/> type.
    /// </summary>
    class ITimedKeyFrameAnimationBuilderExtensions
    {
    public:
        /// <summary>
        /// Adds a new timed keyframe to the builder in use.
        /// </summary>
        /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
        /// <param name="builder">The target <see cref="ITimedKeyFrameAnimationBuilder{T}"/> instance in use.</param>
        /// <param name="progress">The timed progress for the keyframe (in milliseconds), relative to the start of the animation.</param>
        /// <param name="value">The value for the new keyframe to add.</param>
        /// <param name="easingType">The easing type to use to reach the new keyframe.</param>
        /// <param name="easingMode">The easing mode to use to reach the new keyframe.</param>
        /// <returns>The same <see cref="ITimedKeyFrameAnimationBuilder{T}"/> instance that the method was invoked upon.</returns>
        template<typename T>
        static ITimedKeyFrameAnimationBuilder<T>& KeyFrame(
            ITimedKeyFrameAnimationBuilder<T>& builder,
            int progress,
            T const& value,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return builder.KeyFrame(winrt::TimeSpan{ static_cast<int64_t>(progress) * 10000 }, value, easingType, easingMode);
        }
    };
}
