#pragma once

#include "CenterPointAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/Vector3KeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct CenterPointAnimation;

    struct CenterPointAnimationTraits
    {
        using owner_type = CenterPointAnimation;
        using class_type = Animations::CenterPointAnimation;
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

    struct CenterPointAnimation : CenterPointAnimationT<CenterPointAnimation, ImplicitAnimationBase<CenterPointAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"CenterPoint";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct CenterPointAnimation : CenterPointAnimationT<CenterPointAnimation, implementation::CenterPointAnimation> {};
}
