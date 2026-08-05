// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Windows.UI.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#else
import std;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Animations;
#endif
#include "../../XamlToolkit.WinUI.Animations/Builders/AnimationBuilder.h"
#include "../../XamlToolkit.WinUI.Animations/Xaml/Interfaces/ITimeline.h"

namespace winrt 
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::XamlToolkit::WinUI::Animations;
}

namespace winrt::XamlToolkit::WinUI::Media::Animations::implementation
{
    using winrt::XamlToolkit::WinUI::Animations::implementation::ITimeline;

    template<typename TTraits>
    concept AnimationBaseTraits =
        requires
        {
            typename TTraits::class_type;
            typename TTraits::public_value_type;
            typename TTraits::parsed_value_type;
            typename TTraits::keyframe_type;
            typename TTraits::effect_type;
            { TTraits::Parse(std::declval<typename TTraits::public_value_type const&>()) } -> std::same_as<std::optional<typename TTraits::parsed_value_type>>;
        };

    /// <summary>
    /// A base class for effect animations targeting IPipelineEffect instances.
    /// </summary>
    template<AnimationBaseTraits TTraits>
    struct EffectAnimationBase
    {
        using traits_type = TTraits;
        using effect_type = typename traits_type::effect_type;
        using public_value_type = typename traits_type::public_value_type;
        using parsed_value_type = typename traits_type::parsed_value_type;
        using keyframe_type = typename traits_type::keyframe_type;

        EffectAnimationBase() = default;

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TargetProperty =
            winrt::DependencyProperty::Register(
                L"Target",
                winrt::xaml_typename<effect_type>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                winrt::PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ToProperty =
            winrt::DependencyProperty::Register(
                L"To",
                winrt::xaml_typename<public_value_type>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                winrt::PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> FromProperty =
            winrt::DependencyProperty::Register(
                L"From",
                winrt::xaml_typename<public_value_type>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                winrt::PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> KeyFramesProperty =
            winrt::DependencyProperty::Register(
                L"KeyFrames",
                winrt::xaml_typename<winrt::IVector<keyframe_type>>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                winrt::PropertyMetadata(nullptr));

        effect_type Target(this const auto& self)
        {
            return self.GetValue(TargetProperty()).try_as<effect_type>();
        }

        void Target(this const auto& self, effect_type const& value)
        {
            self.SetValue(TargetProperty(), value);
        }

        public_value_type To(this const auto& self)
        {
            return UnboxOrDefault<public_value_type>(self.GetValue(ToProperty()));
        }

        void To(this const auto& self, public_value_type const& value)
        {
            self.SetValue(ToProperty(), winrt::box_value(value));
        }

        public_value_type From(this const auto& self)
        {
            return UnboxOrDefault<public_value_type>(self.GetValue(FromProperty()));
        }

        void From(this const auto& self, public_value_type const& value)
        {
            self.SetValue(FromProperty(), winrt::box_value(value));
        }

        winrt::IVector<keyframe_type> KeyFrames(this const auto& self)
        {
            auto keyFrames = self.GetValue(KeyFramesProperty()).try_as<winrt::IVector<keyframe_type>>();

            if (!keyFrames)
            {
                keyFrames = winrt::single_threaded_vector<keyframe_type>();
                self.SetValue(KeyFramesProperty(), winrt::box_value(keyFrames));
            }

            return keyFrames;
        }

        void KeyFrames(this const auto& self, winrt::IVector<keyframe_type> const& value)
        {
            self.SetValue(KeyFramesProperty(), winrt::box_value(value));
        }

        virtual winrt::hstring ExplicitTarget() const = 0;

        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            this auto& self,
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt)
        {
            auto target = self.Target();
            if (!target)
            {
                throw winrt::hresult_invalid_argument(L"The target effect is null, make sure to set the Target property");
            }

            auto explicitTarget = self.ExplicitTarget();
            if (explicitTarget.empty())
            {
                throw winrt::hresult_invalid_argument(
                    L"The target effect cannot be animated at this time. If you're targeting one of the "
                    L"built-in effects, make sure that the PipelineEffect.IsAnimatable property is set to true.");
            }

            auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderComposition<parsed_value_type>(
                explicitTarget,
                self.Delay() ? self.Delay().Value() : delayHint.value_or(AnimationExtensions::DefaultDelay()),
                self.Duration() ? self.Duration().Value() : durationHint.value_or(AnimationExtensions::DefaultDuration()),
                self.Repeat(),
                self.DelayBehavior());

            self.EffectAnimationBase<TTraits>::AppendToBuilder(keyFrameBuilder, easingTypeHint, easingModeHint);

            winrt::CompositionObject compositionTarget{ nullptr };
            winrt::CompositionAnimation animation = keyFrameBuilder.GetAnimation(target.Brush(), compositionTarget);

            return builder.ExternalAnimation(target.Brush(), animation);
        }

    protected:
        template<typename TValue>
        static TValue UnboxOrDefault(winrt::IInspectable const& value)
        {
            if constexpr (std::is_same_v<TValue, winrt::hstring>)
            {
                return winrt::unbox_value_or<winrt::hstring>(value, L"");
            }
            else
            {
                return value.try_as<TValue>();
            }
        }

        auto GetParsedValues(this const auto& self)
        {
            return std::make_tuple(traits_type::Parse(self.To()), traits_type::Parse(self.From()));
        }

        enum EasingType ResolveEasingType(this const auto& self, std::optional<enum EasingType> easingTypeHint)
        {
            auto easingType = self.EasingType();
            return easingType ? easingType.Value() : easingTypeHint.value_or(AnimationExtensions::DefaultEasingType());
        }

        winrt::EasingMode ResolveEasingMode(this const auto& self, std::optional<winrt::EasingMode> easingModeHint)
        {
            auto easingMode = self.EasingMode();
            return easingMode ? easingMode.Value() : easingModeHint.value_or(AnimationExtensions::DefaultEasingMode());
        }

        INormalizedKeyFrameAnimationBuilder<parsed_value_type>& AppendToBuilder(
            keyframe_type const& keyFrame,
            INormalizedKeyFrameAnimationBuilder<parsed_value_type>& builder) const
        {
            auto keyFrameEasingType = keyFrame.EasingType();
            auto keyFrameEasingMode = keyFrame.EasingMode();
            enum EasingType itemEasingType = keyFrameEasingType ? keyFrameEasingType.Value() : AnimationExtensions::DefaultEasingType();
            winrt::EasingMode itemEasingMode = keyFrameEasingMode ? keyFrameEasingMode.Value() : AnimationExtensions::DefaultEasingMode();

            if (auto expression = keyFrame.Expression(); !expression.empty())
            {
                return builder.ExpressionKeyFrame(keyFrame.Key(), expression, itemEasingType, itemEasingMode);
            }

            if (auto parsedValue = traits_type::Parse(keyFrame.Value()))
            {
                return builder.KeyFrame(keyFrame.Key(), parsedValue.value(), itemEasingType, itemEasingMode);
            }

            return builder;
        }

        INormalizedKeyFrameAnimationBuilder<parsed_value_type>& AppendToBuilder(
			this const auto& self,
            INormalizedKeyFrameAnimationBuilder<parsed_value_type>& builder,
            std::optional<enum EasingType> easingTypeHint,
            std::optional<winrt::EasingMode> easingModeHint)
        {
            for (const auto& keyFrame : self.KeyFrames())
            {
                self.EffectAnimationBase<TTraits>::AppendToBuilder(keyFrame, builder);
            }

            auto [to, from] = self.GetParsedValues();

            if (to.has_value())
            {
                builder.KeyFrame(1.0, to.value(), self.ResolveEasingType(easingTypeHint), self.ResolveEasingMode(easingModeHint));
            }

            if (from.has_value())
            {
                builder.KeyFrame(0.0, from.value());
            }

            return builder;
        }
    };
}
