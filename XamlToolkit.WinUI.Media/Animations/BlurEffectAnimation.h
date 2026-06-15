// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "Animations.BlurEffectAnimation.g.h"
#include "EffectAnimationBase.h"
#include "../Effects/BlurEffect.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import std;
import winrt.Windows.Foundation;
import winrt.XamlToolkit.WinUI.Animations;
import winrt.Microsoft.UI.Xaml.Media.Animation;

namespace winrt::XamlToolkit::WinUI::Media::Animations::implementation
{
    struct BlurEffectAnimationTraits
    {
		using effect_type = BlurEffect;
        using class_type = winrt::XamlToolkit::WinUI::Media::Animations::BlurEffectAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(public_value_type const& value)
        {
            return value;
        }
    };

    struct BlurEffectAnimation : BlurEffectAnimationT<BlurEffectAnimation, ITimeline>, EffectAnimationBase<BlurEffectAnimationTraits>
    {
        BlurEffectAnimation() = default;

        AnimationBuilder& AppendToBuilder(
            AnimationBuilder& builder,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<winrt::XamlToolkit::WinUI::Animations::EasingType> easingType,
            std::optional<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> easingMode) override
        {
            return EffectAnimationBase<BlurEffectAnimationTraits>::AppendToBuilder(builder, delay, duration, easingType, easingMode);
        }

        winrt::hstring ExplicitTarget() const override
        {
			auto targetImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Media::implementation::BlurEffect>(Target());
            return targetImpl->Id();
        } 
    };
}

namespace winrt::XamlToolkit::WinUI::Media::Animations::factory_implementation
{
    struct BlurEffectAnimation : BlurEffectAnimationT<BlurEffectAnimation, implementation::BlurEffectAnimation>
    {
    };
}