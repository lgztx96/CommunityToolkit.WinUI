#pragma once

#include "../../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An interface representing a generic model containing info for an abstract keyframe.
    /// </summary>
    class IKeyFrameInfo
    {
    public:
        /// <summary>
        /// Gets the easing type to use to reach the new keyframe.
        /// </summary>
        virtual EasingType GetEasingType() const = 0;

        /// <summary>
        /// Gets the easing mode to use to reach the new keyframe.
        /// </summary>
        virtual EasingMode GetEasingMode() const = 0;

        /// <summary>
        /// Gets the value for the new keyframe to add.
        /// </summary>
        /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
        /// <returns>The value for the current keyframe.</returns>
        template<typename T>
        T GetValueAs() const
        {
            return GetValueAsInternal<T>();
        }

        /// <summary>
        /// Tries to insert an expression keyframe into the target animation, if possible.
        /// </summary>
        /// <param name="animation">The target <see cref="KeyFrameAnimation"/> instance.</param>
        /// <param name="duration">The total duration for the full animation.</param>
        /// <returns>Whether or not the current <see cref="IKeyFrameInfo"/> instance contained an expression.</returns>
        virtual bool TryInsertExpressionKeyFrame(KeyFrameAnimation const& animation, winrt::Windows::Foundation::TimeSpan duration) = 0;

        /// <summary>
        /// Gets the normalized progress for the current keyframe.
        /// </summary>
        /// <param name="duration">The total duration for the full animation.</param>
        /// <returns>The normalized progress for the current keyframe.</returns>
        virtual float GetNormalizedProgress(winrt::Windows::Foundation::TimeSpan duration) = 0;

        /// <summary>
        /// Gets the timed progress for the current keyframe.
        /// </summary>
        /// <param name="duration">The total duration for the full animation.</param>
        /// <returns>The timed progress for the current keyframe.</returns>
        virtual winrt::Windows::Foundation::TimeSpan GetTimedProgress(winrt::Windows::Foundation::TimeSpan duration) = 0;

        virtual ~IKeyFrameInfo() = default;

    protected:
        template<typename T>
        T GetValueAsInternal() const
        {
            // Default implementation - should be overridden by derived classes
            throw winrt::hresult_not_implemented(L"GetValueAs not implemented for this type");
        }
    };
}
