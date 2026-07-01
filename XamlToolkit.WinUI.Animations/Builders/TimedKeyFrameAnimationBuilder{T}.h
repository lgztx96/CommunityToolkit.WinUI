#pragma once

#include "../Extensions/AnimationExtensions.h"
#include "../Extensions/CompositorExtensions.h"
#include "../Extensions/EasingTypeExtensions.h"
#include "Interfaces/ITimedKeyFrameAnimationBuilder{T}.h"
#include "Interfaces/IKeyFrameInfo.h"
#include "AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#include <vector>
#include <span>
#include <algorithm>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Media::Animation;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// A generic keyframe animation builder using timed keyframes.
    /// </summary>
    /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
    template<typename T>
    class TimedKeyFrameAnimationBuilder : public ITimedKeyFrameAnimationBuilder<T>
    {
    protected:
        /// <summary>
        /// The abstracted info for a timed animation keyframe.
        /// </summary>
        struct KeyFrameInfo : public IKeyFrameInfo
        {
        private:
            TimeSpan progress;
            T value;
            std::optional<winrt::hstring> expression;
            EasingType easingType;
            EasingMode easingMode;

        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="KeyFrameInfo"/> struct.
            /// </summary>
            KeyFrameInfo(TimeSpan progress, T const& value, EasingType easingType, EasingMode easingMode)
                : progress(progress), value(value), easingType(easingType), easingMode(easingMode)
            {
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="KeyFrameInfo"/> struct with an expression.
            /// </summary>
            KeyFrameInfo(TimeSpan progress, winrt::hstring const& expression, EasingType easingType, EasingMode easingMode)
                : progress(progress), value{}, expression(expression), easingType(easingType), easingMode(easingMode)
            {
            }

            EasingType GetEasingType() const override { return easingType; }
            EasingMode GetEasingMode() const override { return easingMode; }

            T GetValue() const { return value; }

            bool HasExpression() const { return expression.has_value(); }
            winrt::hstring GetExpression() const { return expression.value_or(L""); }

            bool TryInsertExpressionKeyFrame(KeyFrameAnimation const& animation, TimeSpan duration) override
            {
                if (!expression.has_value())
                {
                    return false;
                }

                CompositionEasingFunction easingFunction =
                    CompositorExtensions::TryCreateEasingFunction(animation.Compositor(), easingType, easingMode);

                float normalizedProgress = GetNormalizedProgress(duration);

                if (easingFunction)
                {
                    animation.InsertExpressionKeyFrame(normalizedProgress, expression.value(), easingFunction);
                }
                else
                {
                    animation.InsertExpressionKeyFrame(normalizedProgress, expression.value());
                }

                return true;
            }

            float GetNormalizedProgress(TimeSpan duration) override
            {
                double result = static_cast<double>(progress.count()) / static_cast<double>(duration.count());
                return static_cast<float>(std::clamp(result, 0.0, 1.0));
            }

            TimeSpan GetTimedProgress([[maybe_unused]] TimeSpan duration) override
            {
                return progress;
            }
        };

        /// <summary>
        /// The target property to animate.
        /// </summary>
        winrt::hstring property;

        /// <summary>
        /// The target delay for the animation, if any.
        /// </summary>
        std::optional<TimeSpan> delay;

        /// <summary>
        /// The repeat options for the animation.
        /// </summary>
        RepeatOption repeat;

        /// <summary>
        /// The list builder of keyframes to use.
        /// </summary>
        std::vector<KeyFrameInfo> keyFrames;

        /// <summary>
        /// Initializes a new instance of the <see cref="TimedKeyFrameAnimationBuilder{T}"/> class.
        /// </summary>
        TimedKeyFrameAnimationBuilder(winrt::hstring const& property, std::optional<TimeSpan> delay, RepeatOption repeat)
            : property(property), delay(delay), repeat(repeat)
        {
        }

    public:
        /// <inheritdoc/>
        ITimedKeyFrameAnimationBuilder<T>& KeyFrame(
            TimeSpan progress,
            T const& value,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            keyFrames.push_back(KeyFrameInfo(progress, value, easingType, easingMode));
            return *this;
        }

        /// <summary>
        /// Gets the keyframes span.
        /// </summary>
        std::span<KeyFrameInfo> GetKeyFrames()
        {
            return keyFrames.GetSpan();
        }

        /// <summary>
        /// Gets the property name.
        /// </summary>
        winrt::hstring GetProperty() const { return property; }

        /// <summary>
        /// Gets the delay.
        /// </summary>
        std::optional<TimeSpan> GetDelay() const { return delay; }

        /// <summary>
        /// Gets the repeat option.
        /// </summary>
        RepeatOption GetRepeat() const { return repeat; }
    };
}
