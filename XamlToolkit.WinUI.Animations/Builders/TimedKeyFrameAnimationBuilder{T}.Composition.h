#pragma once

#include "TimedKeyFrameAnimationBuilder{T}.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.Composition.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#endif

namespace winrt 
{
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// A custom <see cref="TimedKeyFrameAnimationBuilder{T}"/> class targeting the composition layer.
    /// </summary>
    template<typename T>
    class TimedKeyFrameAnimationBuilderComposition : public TimedKeyFrameAnimationBuilder<T>, public ICompositionAnimationFactory
    {
    private:
        AnimationDelayBehavior delayBehavior;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="TimedKeyFrameAnimationBuilderComposition"/> class.
        /// </summary>
        TimedKeyFrameAnimationBuilderComposition(
            winrt::hstring const& property,
            std::optional<TimeSpan> delay,
            RepeatOption repeat,
            AnimationDelayBehavior delayBehavior)
            : TimedKeyFrameAnimationBuilder<T>(property, delay, repeat), delayBehavior(delayBehavior)
        {
        }

        /// <inheritdoc/>
        ITimedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            TimeSpan progress,
            winrt::hstring const& expression,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            this->keyFrames.push_back(typename TimedKeyFrameAnimationBuilder<T>::KeyFrameInfo(progress, expression, easingType, easingMode));
            return *this;
        }

        /// <inheritdoc/>
        CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
        {
            target = nullptr;

            // We can retrieve the total duration from the last timed keyframe, and then set
            // this as the target duration and use it to normalize the keyframe progresses.
            if (this->keyFrames.empty())
            {
                throw winrt::hresult_invalid_argument(L"No keyframes have been added");
            }

            TimeSpan duration = this->keyFrames.back().GetTimedProgress(TimeSpan{});

            return BuildCompositionKeyFrameAnimation<T, typename TimedKeyFrameAnimationBuilder<T>::KeyFrameInfo>(
                targetHint,
                this->property,
                this->delay,
                duration,
                this->repeat,
                delayBehavior,
                this->keyFrames);
        }
    };
}
