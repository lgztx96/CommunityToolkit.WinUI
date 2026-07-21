#pragma once

#include "OrientationAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/QuaternionKeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct OrientationAnimation;

    struct OrientationAnimationTraits
    {
        using owner_type = OrientationAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::OrientationAnimation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::quaternion;
        using keyframe_impl_type = implementation::QuaternionKeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::QuaternionKeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return AnimationExtensions::ToQuaternion(value);
        }
    };

    struct OrientationAnimation : OrientationAnimationT<OrientationAnimation, ImplicitAnimationBase<OrientationAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Orientation";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct OrientationAnimation : OrientationAnimationT<OrientationAnimation, implementation::OrientationAnimation> {};
}
