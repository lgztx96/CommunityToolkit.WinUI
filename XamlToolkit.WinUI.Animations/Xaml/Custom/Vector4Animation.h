#pragma once

#include "Vector4Animation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"
#include "../KeyFrames/Vector4KeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Vector4Animation;

    struct Vector4AnimationTraits
    {
        using owner_type = Vector4Animation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::Vector4Animation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::float4;
        using keyframe_impl_type = implementation::Vector4KeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::Vector4KeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return AnimationExtensions::ToVector4(value);
        }
    };

    struct Vector4Animation : Vector4AnimationT<Vector4Animation, CustomAnimationBase<Vector4AnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Vector4Animation : Vector4AnimationT<Vector4Animation, implementation::Vector4Animation> {};
}
