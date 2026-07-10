#pragma once

#include "NormalizedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Gets a <see cref="CompositionAnimation"/> instance representing the animation to start.
    /// </summary>
#pragma warning(push)
#pragma warning(disable:4702)
    template<typename T, typename TKeyFrame>
    winrt::CompositionAnimation BuildCompositionKeyFrameAnimation(
        winrt::CompositionObject target,
        winrt::hstring const& property,
        std::optional<winrt::TimeSpan> delay,
        winrt::TimeSpan duration,
        RepeatOption repeat,
        winrt::AnimationDelayBehavior delayBehavior,
        std::span<TKeyFrame> keyFrames)
    {
        winrt::KeyFrameAnimation animation{ nullptr };

        if constexpr (std::is_same_v<T, bool>)
        {
            winrt::BooleanKeyFrameAnimation boolAnimation = target.Compositor().CreateBooleanKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(boolAnimation, duration))
                {
                    continue;
                }

                boolAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
            }

            animation = boolAnimation;
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            winrt::ScalarKeyFrameAnimation scalarAnimation = target.Compositor().CreateScalarKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(scalarAnimation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    scalarAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    scalarAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = scalarAnimation;
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            winrt::ScalarKeyFrameAnimation scalarAnimation = target.Compositor().CreateScalarKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(scalarAnimation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                float value = static_cast<float>(keyFrame.GetValue());
                if (easingFunction)
                {
                    scalarAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), value, easingFunction);
                }
                else
                {
                    scalarAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), value);
                }
            }

            animation = scalarAnimation;
        }
        else if constexpr (std::is_same_v<T, winrt::float2>)
        {
            winrt::Vector2KeyFrameAnimation vector2Animation = target.Compositor().CreateVector2KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector2Animation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    vector2Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    vector2Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = vector2Animation;
        }
        else if constexpr (std::is_same_v<T, winrt::float3>)
        {
            winrt::Vector3KeyFrameAnimation vector3Animation = target.Compositor().CreateVector3KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector3Animation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    vector3Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    vector3Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = vector3Animation;
        }
        else if constexpr (std::is_same_v<T, winrt::float4>)
        {
            winrt::Vector4KeyFrameAnimation vector4Animation = target.Compositor().CreateVector4KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector4Animation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    vector4Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    vector4Animation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = vector4Animation;
        }
        else if constexpr (std::is_same_v<T, winrt::Color>)
        {
            winrt::ColorKeyFrameAnimation colorAnimation = target.Compositor().CreateColorKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(colorAnimation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    colorAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    colorAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = colorAnimation;
        }
        else if constexpr (std::is_same_v<T, winrt::quaternion>)
        {
            winrt::QuaternionKeyFrameAnimation quaternionAnimation = target.Compositor().CreateQuaternionKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(quaternionAnimation, duration))
                {
                    continue;
                }

                winrt::CompositionEasingFunction easingFunction = winrt::CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

                if (easingFunction)
                {
                    quaternionAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue(), easingFunction);
                }
                else
                {
                    quaternionAnimation.InsertKeyFrame(keyFrame.GetNormalizedProgress(duration), keyFrame.GetValue());
                }
            }

            animation = quaternionAnimation;
        }
        else
        {
            throw winrt::hresult_invalid_argument(L"Invalid animation type");
        }

        animation.Duration(duration);

        if (delay.has_value())
        {
            animation.DelayBehavior(delayBehavior);
            animation.DelayTime(delay.value());
        }

        animation.Target(property);

        winrt::AnimationIterationBehavior iterationBehavior;
        int iterationCount;
        RepeatOptionHelper::ToBehaviorAndCount(repeat, iterationBehavior, iterationCount);
        animation.IterationBehavior(iterationBehavior);
        animation.IterationCount(iterationCount);

        return animation;
    }
#pragma warning(pop)

    /// <summary>
    /// A custom <see cref="NormalizedKeyFrameAnimationBuilder{T}"/> class targeting the composition layer.
    /// </summary>
    template<typename T>
    class NormalizedKeyFrameAnimationBuilderComposition : public NormalizedKeyFrameAnimationBuilder<T>, public ICompositionAnimationFactory
    {
    private:
        winrt::AnimationDelayBehavior delayBehavior;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="NormalizedKeyFrameAnimationBuilderComposition"/> class.
        /// </summary>
        NormalizedKeyFrameAnimationBuilderComposition(
            winrt::hstring const& property,
            std::optional<winrt::TimeSpan> delay,
            winrt::TimeSpan duration,
            RepeatOption repeat,
            winrt::AnimationDelayBehavior delayBehavior)
            : NormalizedKeyFrameAnimationBuilder<T>(property, delay, duration, repeat), delayBehavior(delayBehavior)
        {
        }

        /// <inheritdoc/>
        INormalizedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            double progress,
            winrt::hstring const& expression,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            winrt::EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            this->keyFrames.push_back(typename NormalizedKeyFrameAnimationBuilder<T>::KeyFrameInfo(progress, expression, easingType, easingMode));
            return *this;
        }

        /// <inheritdoc/>
        winrt::CompositionAnimation GetAnimation(winrt::CompositionObject const& targetHint, winrt::CompositionObject& target) override
        {
            target = nullptr;

            return BuildCompositionKeyFrameAnimation<T, typename NormalizedKeyFrameAnimationBuilder<T>::KeyFrameInfo>(
                targetHint,
                this->property,
                this->delay,
                this->duration,
                this->repeat,
                delayBehavior,
                this->keyFrames);
        }
    };
}
