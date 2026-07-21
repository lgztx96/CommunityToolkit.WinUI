#pragma once

#include "ColorAnimation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/ColorKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ColorAnimation;

    struct ColorAnimationTraits
    {
        using owner_type = ColorAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::ColorAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<winrt::Windows::UI::Color>;
        using parsed_value_type = winrt::Windows::UI::Color;
        using keyframe_impl_type = implementation::ColorKeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ColorKeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            return value;
        }
    };

    struct ColorAnimation : ColorAnimationT<ColorAnimation, CustomAnimationBase<ColorAnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ColorAnimation : ColorAnimationT<ColorAnimation, implementation::ColorAnimation> {};
}
