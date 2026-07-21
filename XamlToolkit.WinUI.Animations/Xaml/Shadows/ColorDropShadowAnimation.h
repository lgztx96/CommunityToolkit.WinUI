#pragma once

#include "ColorDropShadowAnimation.g.h"
#include "../Abstract/ShadowAnimation{TValue,TKeyFrame}.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../KeyFrames/ColorKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ColorDropShadowAnimation;

    struct ColorDropShadowAnimationTraits
    {
        using owner_type = ColorDropShadowAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::ColorDropShadowAnimation;
        using public_value_type = winrt::IReference<winrt::Windows::UI::Color>;
        using parsed_value_type = winrt::Windows::UI::Color;
        using keyframe_impl_type = implementation::ColorKeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ColorKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::IReference<winrt::Windows::UI::Color> const& value)
        {
            return value;
        }
    };

    struct ColorDropShadowAnimation : ColorDropShadowAnimationT<ColorDropShadowAnimation, ShadowAnimationBase<ColorDropShadowAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Color";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ColorDropShadowAnimation : ColorDropShadowAnimationT<ColorDropShadowAnimation, implementation::ColorDropShadowAnimation> {};
}
