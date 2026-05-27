#pragma once

#include "Vector2Animation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"
#include "../KeyFrames/Vector2KeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Vector2Animation;

    struct Vector2AnimationTraits
    {
        using owner_type = Vector2Animation;
        using class_type = Animations::Vector2Animation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::float2;
        using keyframe_impl_type = implementation::Vector2KeyFrame;
        using keyframe_type = Animations::Vector2KeyFrame;

        static std::optional<parsed_value_type> Parse(winrt::hstring const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return Animations::AnimationExtensions::ToVector2(value);
        }
    };

    struct Vector2Animation : Vector2AnimationT<Vector2Animation, CustomAnimationBase<Vector2AnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Vector2Animation : Vector2AnimationT<Vector2Animation, implementation::Vector2Animation> {};
}
