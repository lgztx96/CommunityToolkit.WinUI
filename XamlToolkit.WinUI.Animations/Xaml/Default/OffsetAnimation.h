#pragma once

#include "OffsetAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/Vector3KeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct OffsetAnimation;

    struct OffsetAnimationTraits
    {
        using owner_type = OffsetAnimation;
        using class_type = Animations::OffsetAnimation;
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

    struct OffsetAnimation : OffsetAnimationT<OffsetAnimation, ImplicitAnimationBase<OffsetAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Offset";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct OffsetAnimation : OffsetAnimationT<OffsetAnimation, implementation::OffsetAnimation> {};
}
