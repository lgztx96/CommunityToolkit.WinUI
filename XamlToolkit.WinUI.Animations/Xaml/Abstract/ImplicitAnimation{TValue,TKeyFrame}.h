#pragma once

#include "Animation{TValue,TKeyFrame}.h"
#include "../Interfaces/IImplicitTimeline.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// C++ counterpart for C# ImplicitAnimation<TValue, TKeyFrame>.
    /// </summary>
    template<AnimationBaseTraits TTraits>
    struct ImplicitAnimationBase : AnimationBase<TTraits>, IImplicitTimeline
    {
        using traits_type = TTraits;
        using parsed_value_type = typename traits_type::parsed_value_type;
        using keyframe_impl_type = typename traits_type::keyframe_impl_type;

        ImplicitAnimationBase()
        {
            this->RegisterPropertyChangedCallback(Animation::DelayProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(Animation::DurationProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(Animation::EasingTypeProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(Animation::EasingModeProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(Animation::RepeatProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(Animation::DelayBehaviorProperty, { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(AnimationBase<TTraits>::ToProperty(), { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(AnimationBase<TTraits>::FromProperty(), { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
            this->RegisterPropertyChangedCallback(AnimationBase<TTraits>::KeyFramesProperty(), { this, &ImplicitAnimationBase::RaiseAnimationPropertyChanged });
        }

        /// <summary>
        /// Gets or sets the optional implicit target for the animation.
        /// This can act as a trigger property for the animation.
        /// </summary>
        wil::single_threaded_rw_property<winrt::hstring> ImplicitTarget = L"";

        /// <inheritdoc/>
        winrt::Microsoft::UI::Composition::CompositionAnimation GetAnimation(
            winrt::Microsoft::UI::Xaml::UIElement const& element,
            winrt::hstring& target) override
        {
            auto explicitTarget = this->ExplicitTarget();

            if (explicitTarget.empty())
            {
                throw winrt::hresult_invalid_argument(L"ExplicitTarget property cannot be null");
            }

            auto builder = Animations::NormalizedKeyFrameAnimationBuilderComposition<parsed_value_type>(
                explicitTarget,
                this->Delay() ? this->Delay().Value() : Animations::AnimationExtensions::DefaultDelay(),
                this->Duration() ? this->Duration().Value() : Animations::AnimationExtensions::DefaultDuration(),
                this->Repeat(),
                this->DelayBehavior());

            auto [to, from] = this->GetParsedValues();

            // If there are no values set for the animation at all (no initial/target values, nor
            // keyframes), we just manually insert a single expression keyframe pointing to the final
            // value for the current animation. This is often the case with implicit animations, as
            // it is used to smoothly transition between two discrete property changes for a visual.
            if (!to.has_value() && !from.has_value() && this->Owner().KeyFrames().Size() == 0)
            {
                builder.ExpressionKeyFrame(1.0, L"this.FinalValue", Animations::AnimationExtensions::DefaultEasingType(), Animations::AnimationExtensions::DefaultEasingMode());
            }
            else
            {
                // Otherwise, we just insert the keyframes for the initial/target values, as well as the
                // other keyframes that might be present into the current animation. The order is not
                // important when inserting keyframes, as each one stores the normalized progress value.
                if (to.has_value())
                {
                    builder.KeyFrame(1.0, to.value(),
                        this->EasingType() ? this->EasingType().Value() : Animations::AnimationExtensions::DefaultEasingType(),
                        this->EasingMode() ? this->EasingMode().Value() : Animations::AnimationExtensions::DefaultEasingMode());
                }

                if (from.has_value())
                {
                    builder.KeyFrame(0.0, from.value(), 
                        Animations::AnimationExtensions::DefaultEasingType(), 
                        Animations::AnimationExtensions::DefaultEasingMode());
                }

                for (auto keyFrame : this->KeyFrames())
                {
					auto keyFrameImpl = winrt::get_self<keyframe_impl_type>(keyFrame);
                    keyFrameImpl->AppendToBuilder(builder);
                }
            }

            target = ImplicitTarget();

            auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
            winrt::Microsoft::UI::Composition::CompositionObject ignored{ nullptr };
            return builder.GetAnimation(visual, ignored);
        }

    protected:
        void RaiseAnimationPropertyChanged(
            [[maybe_unused]] DependencyObject const& sender,
            [[maybe_unused]] DependencyProperty const& property)
        {
            this->AnimationPropertyChanged.invoke(*this, nullptr);
        }
    };
}
