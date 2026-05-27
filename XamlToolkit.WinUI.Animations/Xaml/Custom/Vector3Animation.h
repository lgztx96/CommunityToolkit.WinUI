#pragma once

#include "Vector3Animation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"
#include "../KeyFrames/Vector3KeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Vector3Animation;

    struct Vector3AnimationTraits
    {
        using owner_type = Vector3Animation;
        using class_type = Animations::Vector3Animation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::float3;
        using keyframe_impl_type = implementation::Vector3KeyFrame;
        using keyframe_type = Animations::Vector3KeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::hstring const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return Animations::AnimationExtensions::ToVector3(value);
        }
    };

    struct Vector3Animation : Vector3AnimationT<Vector3Animation, CustomAnimationBase<Vector3AnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Vector3Animation : Vector3AnimationT<Vector3Animation, implementation::Vector3Animation> {};
}
