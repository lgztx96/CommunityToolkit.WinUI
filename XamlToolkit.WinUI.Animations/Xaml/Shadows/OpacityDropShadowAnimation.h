#pragma once

#include "OpacityDropShadowAnimation.g.h"
#include "../Abstract/ShadowAnimation{TValue,TKeyFrame}.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../KeyFrames/ScalarKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct OpacityDropShadowAnimation;

    struct OpacityDropShadowAnimationTraits
    {
        using owner_type = OpacityDropShadowAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::OpacityDropShadowAnimation;
        using public_value_type = winrt::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::IReference<double> const& value)
        {
            return value;
        }
    };

    struct OpacityDropShadowAnimation : OpacityDropShadowAnimationT<OpacityDropShadowAnimation, ShadowAnimationBase<OpacityDropShadowAnimationTraits>, IAttachedTimeline>
    {
        winrt::hstring ExplicitTarget() const noexcept;

        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            winrt::UIElement const& parent,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct OpacityDropShadowAnimation : OpacityDropShadowAnimationT<OpacityDropShadowAnimation, implementation::OpacityDropShadowAnimation> {};
}
