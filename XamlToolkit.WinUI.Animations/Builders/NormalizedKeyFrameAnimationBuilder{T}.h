#pragma once

#include "../Options/RepeatOptionHelper.h"
#include "../Extensions/AnimationExtensions.h"
#include "../Extensions/CompositorExtensions.h"
#include "../Extensions/EasingTypeExtensions.h"
#include "Interfaces/INormalizedKeyFrameAnimationBuilder{T}.h"
#include "Interfaces/IKeyFrameInfo.h"
#include "AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#include <vector>
#include <algorithm>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace winrt::Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Media::Animation;
    using namespace XamlToolkit::WinUI::Animations;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// A generic keyframe animation builder.
    /// </summary>
    /// <typeparam name="T">The type of values being set by the animation being constructed.</typeparam>
    template<typename T>
    class NormalizedKeyFrameAnimationBuilder : public INormalizedKeyFrameAnimationBuilder<T>
    {
    protected:
        /// <summary>
        /// The abstracted info for a normalized animation keyframe.
        /// </summary>
        struct KeyFrameInfo : public IKeyFrameInfo
        {
        private:
            double progress;
            T value;
            std::optional<winrt::hstring> expression;
            EasingType easingType;
            EasingMode easingMode;

        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="KeyFrameInfo"/> struct.
            /// </summary>
            KeyFrameInfo(double progress, T const& value, EasingType easingType, EasingMode easingMode)
                : progress(progress), value(value), easingType(easingType), easingMode(easingMode)
            {
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="KeyFrameInfo"/> struct with an expression.
            /// </summary>
            KeyFrameInfo(double progress, winrt::hstring const& expression, EasingType easingType, EasingMode easingMode)
                : progress(progress), value{}, expression(expression), easingType(easingType), easingMode(easingMode)
            {
            }

            EasingType GetEasingType() const override { return easingType; }
            EasingMode GetEasingMode() const override { return easingMode; }

            T GetValue() const { return value; }

            bool HasExpression() const { return expression.has_value(); }
            winrt::hstring GetExpression() const { return expression.value_or(L""); }

            bool TryInsertExpressionKeyFrame(KeyFrameAnimation const& animation, [[maybe_unused]] TimeSpan duration) override
            {
                if (!expression.has_value())
                {
                    return false;
                }

                auto easingFunction = CompositorExtensions::TryCreateEasingFunction(animation.Compositor(), easingType, easingMode);

                if (easingFunction)
                {
                    animation.InsertExpressionKeyFrame((float)progress, expression.value(), easingFunction);
                }
                else
                {
                    animation.InsertExpressionKeyFrame((float)progress, expression.value());
                }

                return true;
            }

            float GetNormalizedProgress([[maybe_unused]] TimeSpan duration) override
            {
                return static_cast<float>(progress);
            }

            TimeSpan GetTimedProgress(TimeSpan duration) override
            {
                return TimeSpan{ static_cast<int64_t>(duration.count() * progress) };
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
        /// The target duration for the animation.
        /// </summary>
        TimeSpan duration;

        /// <summary>
        /// The repeat options for the animation.
        /// </summary>
        RepeatOption repeat;

        /// <summary>
        /// The list builder of keyframes to use.
        /// </summary>
        std::vector<KeyFrameInfo> keyFrames;

        /// <summary>
        /// Initializes a new instance of the <see cref="NormalizedKeyFrameAnimationBuilder{T}"/> class.
        /// </summary>
        NormalizedKeyFrameAnimationBuilder(winrt::hstring const& property, std::optional<TimeSpan> delay, TimeSpan duration, RepeatOption repeat)
            : property(property), delay(delay), duration(duration), repeat(repeat)
        {
        }

    public:
        /// <inheritdoc/>
        INormalizedKeyFrameAnimationBuilder<T>& KeyFrame(
            double progress,
            T const& value,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            keyFrames.push_back(KeyFrameInfo(progress, value, easingType, easingMode));
            return *this;
        }
    };
}
