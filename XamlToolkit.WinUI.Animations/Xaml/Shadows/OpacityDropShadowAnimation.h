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
        using class_type = Animations::OpacityDropShadowAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<double> const& value)
        {
            return value;
        }
    };

    struct OpacityDropShadowAnimation : OpacityDropShadowAnimationT<OpacityDropShadowAnimation, ShadowAnimationBase<OpacityDropShadowAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Opacity";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct OpacityDropShadowAnimation : OpacityDropShadowAnimationT<OpacityDropShadowAnimation, implementation::OpacityDropShadowAnimation> {};
}
