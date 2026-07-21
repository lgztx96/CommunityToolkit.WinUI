#pragma once

#include "TimedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Gets a <see cref="Timeline"/> instance representing the animation to start.
    /// </summary>
    template<typename T, typename TKeyFrame>
    winrt::Timeline BuildXamlKeyFrameAnimation(
        winrt::DependencyObject target,
        winrt::hstring const& property,
        std::optional<winrt::TimeSpan> delay,
        winrt::TimeSpan duration,
        RepeatOption repeat,
        std::span<TKeyFrame> keyFrames)
    {
        auto finalize = [&](winrt::Timeline const& animation)
        {
            animation.BeginTime(delay);
            animation.RepeatBehavior(RepeatOptionHelper::ToRepeatBehavior(repeat));

            winrt::Storyboard::SetTarget(animation, target);
            winrt::Storyboard::SetTargetProperty(animation, property);

            return animation;
        };

        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            winrt::DoubleAnimationUsingKeyFrames doubleAnimation;
            doubleAnimation.EnableDependentAnimation(true);

            for (auto& keyFrame : keyFrames)
            {
                winrt::EasingDoubleKeyFrame easingKeyFrame;
                easingKeyFrame.KeyTime(winrt::KeyTimeHelper::FromTimeSpan(keyFrame.GetTimedProgress(duration)));
                easingKeyFrame.Value(keyFrame.GetValue());
                easingKeyFrame.EasingFunction(EasingTypeExtensions::ToEasingFunction(keyFrame.GetEasingType(), keyFrame.GetEasingMode()));

                doubleAnimation.KeyFrames().Append(easingKeyFrame);
            }

            return finalize(doubleAnimation);
        }
        else if constexpr (std::is_same_v<T, winrt::Point>)
        {
            winrt::PointAnimationUsingKeyFrames pointAnimation;
            pointAnimation.EnableDependentAnimation(true);

            for (auto& keyFrame : keyFrames)
            {
                EasingPointKeyFrame easingKeyFrame;
                easingKeyFrame.KeyTime(KeyTimeHelper::FromTimeSpan(keyFrame.GetTimedProgress(duration)));
                easingKeyFrame.Value(keyFrame.GetValue());
                easingKeyFrame.EasingFunction(EasingTypeExtensions::ToEasingFunction(keyFrame.GetEasingType(), keyFrame.GetEasingMode()));

                pointAnimation.KeyFrames().Append(easingKeyFrame);
            }

            return finalize(pointAnimation);
        }
        else if constexpr (std::is_same_v<T, winrt::Windows::UI::Color>)
        {
            winrt::ColorAnimationUsingKeyFrames colorAnimation;
            colorAnimation.EnableDependentAnimation(true);

            for (auto& keyFrame : keyFrames)
            {
                winrt::EasingColorKeyFrame easingKeyFrame;
                easingKeyFrame.KeyTime(winrt::KeyTimeHelper::FromTimeSpan(keyFrame.GetTimedProgress(duration)));
                easingKeyFrame.Value(keyFrame.GetValue());
                easingKeyFrame.EasingFunction(EasingTypeExtensions::ToEasingFunction(keyFrame.GetEasingType(), keyFrame.GetEasingMode()));

                colorAnimation.KeyFrames().Append(easingKeyFrame);
            }

            return finalize(colorAnimation);
        }
        else if constexpr (std::is_same_v<T, winrt::IInspectable>)
        {
            winrt::ObjectAnimationUsingKeyFrames objectAnimation;
            objectAnimation.EnableDependentAnimation(true);

            for (auto& keyFrame : keyFrames)
            {
                winrt::DiscreteObjectKeyFrame easingKeyFrame;
                easingKeyFrame.KeyTime(winrt::KeyTimeHelper::FromTimeSpan(keyFrame.GetTimedProgress(duration)));
                easingKeyFrame.Value(keyFrame.GetValue());
                objectAnimation.KeyFrames().Append(easingKeyFrame);
            }

            return finalize(objectAnimation);
        }
        else
        {
            throw winrt::hresult_invalid_argument(L"Invalid animation type");
        }
    }

    /// <summary>
    /// A custom <see cref="TimedKeyFrameAnimationBuilder{T}"/> class targeting the XAML layer.
    /// </summary>
    template<typename T>
    class TimedKeyFrameAnimationBuilderXaml : public TimedKeyFrameAnimationBuilder<T>, public IXamlAnimationFactory
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="TimedKeyFrameAnimationBuilderXaml"/> class.
        /// </summary>
        TimedKeyFrameAnimationBuilderXaml(
            winrt::hstring const& property,
            std::optional<winrt::TimeSpan> delay,
            RepeatOption repeat)
            : TimedKeyFrameAnimationBuilder<T>(property, delay, repeat)
        {
        }

        /// <inheritdoc/>
        ITimedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            [[maybe_unused]] winrt::TimeSpan progress,
            [[maybe_unused]] winrt::hstring const& expression,
            [[maybe_unused]] EasingType easingType = AnimationExtensions::DefaultEasingType(),
            [[maybe_unused]] EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            throw winrt::hresult_error(E_FAIL, L"Expression keyframes can only be used on the composition layer");
        }

        /// <inheritdoc/>
        winrt::Timeline GetAnimation(winrt::DependencyObject const& targetHint) override
        {
            return BuildXamlKeyFrameAnimation<T, typename TimedKeyFrameAnimationBuilder<T>::KeyFrameInfo>(
                targetHint,
                this->property,
                this->delay,
                winrt::TimeSpan{}, // default duration
                this->repeat,
                this->keyFrames);
        }
    };
}
