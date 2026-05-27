#pragma once

#include "QuaternionAnimation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"
#include "../KeyFrames/QuaternionKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct QuaternionAnimation;

    struct QuaternionAnimationTraits
    {
        using owner_type = QuaternionAnimation;
        using class_type = Animations::QuaternionAnimation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::quaternion;
        using keyframe_impl_type = implementation::QuaternionKeyFrame;
        using keyframe_type = Animations::QuaternionKeyFrame;

        static std::optional<parsed_value_type> Parse(winrt::hstring const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return Animations::AnimationExtensions::ToQuaternion(value);
        }
    };

    struct QuaternionAnimation : QuaternionAnimationT<QuaternionAnimation, CustomAnimationBase<QuaternionAnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct QuaternionAnimation : QuaternionAnimationT<QuaternionAnimation, implementation::QuaternionAnimation> {};
}
