#pragma once

#include "OffsetDropShadowAnimation.g.h"
#include "../Abstract/ShadowAnimation{TValue,TKeyFrame}.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../KeyFrames/Vector3KeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct OffsetDropShadowAnimation;

    struct OffsetDropShadowAnimationTraits
    {
        using owner_type = OffsetDropShadowAnimation;
        using class_type = winrt::XamlToolkit::WinUI::Animations::OffsetDropShadowAnimation;
        using public_value_type = winrt::hstring;
        using parsed_value_type = winrt::Windows::Foundation::Numerics::float3;
        using keyframe_impl_type = implementation::Vector3KeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::Vector3KeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::hstring const& value)
        {
            if (value.empty())
            {
                return std::nullopt;
            }

            return AnimationExtensions::ToVector3(value);
        }
    };

    struct OffsetDropShadowAnimation : OffsetDropShadowAnimationT<OffsetDropShadowAnimation, ShadowAnimationBase<OffsetDropShadowAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Offset";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct OffsetDropShadowAnimation : OffsetDropShadowAnimationT<OffsetDropShadowAnimation, implementation::OffsetDropShadowAnimation> {};
}
