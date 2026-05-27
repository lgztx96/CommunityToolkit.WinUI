#pragma once

#include "TranslationAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/Vector3KeyFrame.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct TranslationAnimation;

    struct TranslationAnimationTraits
    {
        using owner_type = TranslationAnimation;
        using class_type = Animations::TranslationAnimation;
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

    /// <summary>
    /// A translation animation working on the composition layer.
    /// </summary>
    struct TranslationAnimation : TranslationAnimationT<TranslationAnimation, ImplicitAnimationBase<TranslationAnimationTraits>>
    {
        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"Translation";
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct TranslationAnimation : TranslationAnimationT<TranslationAnimation, implementation::TranslationAnimation> {};
}
