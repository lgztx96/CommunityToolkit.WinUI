#pragma once

#include "BlurRadiusDropShadowAnimation.g.h"
#include "../Abstract/ShadowAnimation{TValue,TKeyFrame}.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../KeyFrames/ScalarKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct BlurRadiusDropShadowAnimation;

    struct BlurRadiusDropShadowAnimationTraits
    {
        using owner_type = BlurRadiusDropShadowAnimation;
        using class_type = Animations::BlurRadiusDropShadowAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<double> const& value)
        {
            return value;
        }
    };

    struct BlurRadiusDropShadowAnimation : BlurRadiusDropShadowAnimationT<BlurRadiusDropShadowAnimation, ShadowAnimationBase<BlurRadiusDropShadowAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"BlurRadius";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct BlurRadiusDropShadowAnimation : BlurRadiusDropShadowAnimationT<BlurRadiusDropShadowAnimation, implementation::BlurRadiusDropShadowAnimation> {};
}
