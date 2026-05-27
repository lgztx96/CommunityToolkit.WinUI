#pragma once

#include "RotationAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/ScalarKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct RotationAnimation;

    struct RotationAnimationTraits
    {
        using owner_type = RotationAnimation;
        using class_type = Animations::RotationAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<double> const& value)
        {
            return value;
        }
    };

    struct RotationAnimation : RotationAnimationT<RotationAnimation, ImplicitAnimationBase<RotationAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"RotationAngle";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct RotationAnimation : RotationAnimationT<RotationAnimation, implementation::RotationAnimation> {};
}
