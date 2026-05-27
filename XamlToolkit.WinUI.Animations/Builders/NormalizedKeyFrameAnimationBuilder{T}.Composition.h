#pragma once

#include "NormalizedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Animation;
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Gets a <see cref="CompositionAnimation"/> instance representing the animation to start.
    /// </summary>
#pragma warning(push)
#pragma warning(disable:4702)
    template<typename T, typename TKeyFrame>
    CompositionAnimation BuildCompositionKeyFrameAnimation(
        CompositionObject target,
        winrt::hstring const& property,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        winrt::Windows::Foundation::TimeSpan duration,
        RepeatOption repeat,
        AnimationDelayBehavior delayBehavior,
        std::span<TKeyFrame> keyFrames)
    {
        KeyFrameAnimation animation{ nullptr };

        if constexpr (std::is_same_v<T, bool>)
        {
            BooleanKeyFrameAnimation boolAnimation = target.Compositor().CreateBooleanKeyFrameAnimation();

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
            ScalarKeyFrameAnimation scalarAnimation = target.Compositor().CreateScalarKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(scalarAnimation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
            ScalarKeyFrameAnimation scalarAnimation = target.Compositor().CreateScalarKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(scalarAnimation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        else if constexpr (std::is_same_v<T, float2>)
        {
            Vector2KeyFrameAnimation vector2Animation = target.Compositor().CreateVector2KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector2Animation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        else if constexpr (std::is_same_v<T, float3>)
        {
            Vector3KeyFrameAnimation vector3Animation = target.Compositor().CreateVector3KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector3Animation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        else if constexpr (std::is_same_v<T, float4>)
        {
            Vector4KeyFrameAnimation vector4Animation = target.Compositor().CreateVector4KeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(vector4Animation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        else if constexpr (std::is_same_v<T, winrt::Windows::UI::Color>)
        {
            ColorKeyFrameAnimation colorAnimation = target.Compositor().CreateColorKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(colorAnimation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        else if constexpr (std::is_same_v<T, quaternion>)
        {
            QuaternionKeyFrameAnimation quaternionAnimation = target.Compositor().CreateQuaternionKeyFrameAnimation();

            for (auto& keyFrame : keyFrames)
            {
                if (keyFrame.TryInsertExpressionKeyFrame(quaternionAnimation, duration))
                {
                    continue;
                }

                CompositionEasingFunction easingFunction = CompositorExtensions::TryCreateEasingFunction(target.Compositor(), keyFrame.GetEasingType(), keyFrame.GetEasingMode());

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
        auto [iterationBehavior, iterationCount] = RepeatOptionHelper::ToBehaviorAndCount(repeat);
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
        AnimationDelayBehavior delayBehavior;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="NormalizedKeyFrameAnimationBuilderComposition"/> class.
        /// </summary>
        NormalizedKeyFrameAnimationBuilderComposition(
            winrt::hstring const& property,
            std::optional<TimeSpan> delay,
            TimeSpan duration,
            RepeatOption repeat,
            AnimationDelayBehavior delayBehavior)
            : NormalizedKeyFrameAnimationBuilder<T>(property, delay, duration, repeat), delayBehavior(delayBehavior)
        {
        }

        /// <inheritdoc/>
        INormalizedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            double progress,
            winrt::hstring const& expression,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            this->keyFrames.push_back(typename NormalizedKeyFrameAnimationBuilder<T>::KeyFrameInfo(progress, expression, easingType, easingMode));
            return *this;
        }

        /// <inheritdoc/>
        CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
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
