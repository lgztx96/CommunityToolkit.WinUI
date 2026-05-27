#pragma once

#include "RotationInDegreesAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/ScalarKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct RotationInDegreesAnimation;

    struct RotationInDegreesAnimationTraits
    {
        using owner_type = RotationInDegreesAnimation;
        using class_type = Animations::RotationInDegreesAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<double> const& value)
        {
            return value;
        }
    };

    struct RotationInDegreesAnimation : RotationInDegreesAnimationT<RotationInDegreesAnimation, ImplicitAnimationBase<RotationInDegreesAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"RotationAngleInDegrees";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct RotationInDegreesAnimation : RotationInDegreesAnimationT<RotationInDegreesAnimation, implementation::RotationInDegreesAnimation> {};
}
