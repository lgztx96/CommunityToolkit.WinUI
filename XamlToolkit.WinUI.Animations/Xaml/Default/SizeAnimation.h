#pragma once

#include "SizeAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/Vector2KeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct SizeAnimation;

    struct SizeAnimationTraits
    {
        using owner_type = SizeAnimation;
        using class_type = Animations::SizeAnimation;
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

    struct SizeAnimation : SizeAnimationT<SizeAnimation, ImplicitAnimationBase<SizeAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Size";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct SizeAnimation : SizeAnimationT<SizeAnimation, implementation::SizeAnimation> {};
}
