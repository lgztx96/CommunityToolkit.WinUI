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
        using class_type = winrt::XamlToolkit::WinUI::Animations::ClipAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<Thickness>;
        using parsed_value_type = winrt::Microsoft::UI::Xaml::Thickness;
        using keyframe_impl_type = implementation::ThicknessKeyFrame;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ThicknessKeyFrame;
        static std::optional<parsed_value_type> Parse([[maybe_unused]] public_value_type const& value)
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
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt) override;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ClipAnimation : ClipAnimationT<ClipAnimation, implementation::ClipAnimation> {};
}
