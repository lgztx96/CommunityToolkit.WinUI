// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "Animations.ColorEffectAnimation.g.h"
#include "EffectAnimationBase.h"
#include "../Effects/TintEffect.h"
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
    struct ColorEffectAnimationTraits
    {
		using effect_type = TintEffect;
        using class_type = winrt::XamlToolkit::WinUI::Media::Animations::ColorEffectAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<winrt::Windows::UI::Color>;
        using parsed_value_type = winrt::Windows::UI::Color;
        using keyframe_type = winrt::XamlToolkit::WinUI::Animations::ColorKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<winrt::Windows::UI::Color> const& value)
        {
            return value;
        }
    };

    struct ColorEffectAnimation : ColorEffectAnimationT<ColorEffectAnimation, ITimeline>, EffectAnimationBase<ColorEffectAnimationTraits>
    {
        ColorEffectAnimation() = default;

        AnimationBuilder& AppendToBuilder(
            AnimationBuilder& builder,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<winrt::XamlToolkit::WinUI::Animations::EasingType> easingType,
            std::optional<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> easingMode) override
        {
            return EffectAnimationBase<ColorEffectAnimationTraits>::AppendToBuilder(builder, delay, duration, easingType, easingMode);
        }

        winrt::hstring ExplicitTarget() const override
        {
            auto targetImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Media::implementation::TintEffect>(Target());
            return targetImpl->Id();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::Animations::factory_implementation
{
    struct ColorEffectAnimation : ColorEffectAnimationT<ColorEffectAnimation, implementation::ColorEffectAnimation>
    {
    };
}