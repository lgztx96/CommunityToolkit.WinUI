// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "Animations.OpacityEffectAnimation.g.h"
#include "EffectAnimationBase.h"
#include "../Effects/OpacityEffect.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import std;
import winrt.Windows.Foundation;
import winrt.XamlToolkit.WinUI.Animations;
import winrt.Microsoft.UI.Xaml.Media.Animation;
#endif

namespace winrt::XamlToolkit::WinUI::Media::Animations::implementation
{
    struct OpacityEffectAnimationTraits
    {
		using effect_type = OpacityEffect;
        using class_type = winrt::XamlToolkit::WinUI::Media::Animations::OpacityEffectAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            return value;
        }
    };

    struct OpacityEffectAnimation : OpacityEffectAnimationT<OpacityEffectAnimation, ITimeline>, EffectAnimationBase<OpacityEffectAnimationTraits>
    {
        OpacityEffectAnimation() = default;

        AnimationBuilder& AppendToBuilder(
            AnimationBuilder& builder,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<winrt::XamlToolkit::WinUI::Animations::EasingType> easingType,
            std::optional<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> easingMode) override
        {
            return EffectAnimationBase<OpacityEffectAnimationTraits>::AppendToBuilder(builder, delay, duration, easingType, easingMode);
        }

        winrt::hstring ExplicitTarget() const override
        {
			auto targetImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Media::implementation::OpacityEffect>(Target());
            return targetImpl->Id();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::Animations::factory_implementation
{
    struct OpacityEffectAnimation : OpacityEffectAnimationT<OpacityEffectAnimation, implementation::OpacityEffectAnimation>
    {
    };
}