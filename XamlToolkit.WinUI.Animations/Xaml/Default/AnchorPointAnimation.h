#pragma once

#include "AnchorPointAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/Vector2KeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct AnchorPointAnimation;

    struct AnchorPointAnimationTraits
    {
        using owner_type = AnchorPointAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::AnchorPointAnimation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::float2;
        using keyframe_impl_type = implementation::Vector2KeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::Vector2KeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return AnimationExtensions::ToVector2(value);
        }
    };

    struct AnchorPointAnimation : AnchorPointAnimationT<AnchorPointAnimation, ImplicitAnimationBase<AnchorPointAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"AnchorPoint";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnchorPointAnimation : AnchorPointAnimationT<AnchorPointAnimation, implementation::AnchorPointAnimation> {};
}
