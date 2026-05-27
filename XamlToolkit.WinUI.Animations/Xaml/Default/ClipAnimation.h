#pragma once

#include "ClipAnimation.g.h"
#include "../Abstract/ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/ThicknessKeyFrame.h"
#include "../Builders/AnimationBuilder.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ClipAnimation;

    struct ClipAnimationTraits
    {
        using owner_type = ClipAnimation;
        using class_type = Animations::ClipAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<Thickness>;
        using parsed_value_type = Thickness;
        using keyframe_impl_type = implementation::ThicknessKeyFrame;
        using keyframe_type = Animations::ThicknessKeyFrame;
        static std::optional<parsed_value_type> Parse([[maybe_unused]] winrt::Windows::Foundation::IReference<Thickness> const& value)
        {
			throw winrt::hresult_not_implemented();
        }
    };

    /// <summary>
    /// A clip animation working on the composition layer.
    /// </summary>
    struct ClipAnimation : ClipAnimationT<ClipAnimation, AnimationBase<ClipAnimationTraits>>
    {
        ClipAnimation() = default;

        winrt::hstring ExplicitTarget() const noexcept
        {
            return L"";
        }

        Animations::AnimationBuilder& AppendToBuilder(
            Animations::AnimationBuilder& builder,
            std::optional<winrt::Windows::Foundation::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<enum EasingMode> easingModeHint = std::nullopt) override;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ClipAnimation : ClipAnimationT<ClipAnimation, implementation::ClipAnimation> {};
}
