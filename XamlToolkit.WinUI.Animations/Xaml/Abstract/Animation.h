#pragma once

#include "Animation.g.h"
#include "../Interfaces/ITimeline.h"
#include "../Builders/AnimationBuilder.h"
#include "../Options/RepeatOptionHelper.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A base model representing an animation that can be used in XAML.
    /// Implements ITimelineInternal as a C++ interface for AnimationBuilder integration.
    /// </summary>
    struct Animation : AnimationT<Animation, ITimeline>
    {
    public:
        Animation() = default;

        /// <summary>
        /// Gets or sets the initial delay before the animation starts.
        /// </summary>
        winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> Delay() const
        {
            return GetValue(DelayProperty).try_as<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>();
        }
        /// <summary>
        /// Gets or sets the initial delay before the animation starts.
        /// </summary>
        void Delay(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const& value)
        {
            SetValue(DelayProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the duration for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> Duration() const
        {
            return GetValue(DurationProperty).try_as<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>();
        }
        /// <summary>
        /// Gets or sets the duration for the animation.
        /// </summary>
        void Duration(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const& value)
        {
            SetValue(DurationProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function type for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<EasingType> EasingType() const
        {
            return GetValue(EasingTypeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingType>>();
        }
        /// <summary>
        /// Gets or sets the optional easing function type for the animation.
        /// </summary>
        void EasingType(winrt::Windows::Foundation::IReference<enum EasingType> const& value)
        {
            SetValue(EasingTypeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function mode for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<EasingMode> EasingMode() const
        {
            return GetValue(EasingModeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingMode>>();
        }
        /// <summary>
        /// Gets or sets the optional easing function mode for the animation.
        /// </summary>
        void EasingMode(winrt::Windows::Foundation::IReference<enum EasingMode> const& value)
        {
            SetValue(EasingModeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the repeat behavior for the animation.
        /// </summary>
        winrt::XamlToolkit::WinUI::Animations::RepeatOption Repeat()
        {
            return winrt::unbox_value<winrt::XamlToolkit::WinUI::Animations::RepeatOption>(GetValue(RepeatProperty));
        }
        /// <summary>
        /// Gets or sets the repeat behavior for the animation.
        /// </summary>
        void Repeat(winrt::XamlToolkit::WinUI::Animations::RepeatOption const& value)
        {
            SetValue(RepeatProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the delay behavior for the animation.
        /// </summary>
        AnimationDelayBehavior DelayBehavior()
        {
            return winrt::unbox_value<AnimationDelayBehavior>(GetValue(DelayBehaviorProperty));
        }
        /// <summary>
        /// Gets or sets the delay behavior for the animation.
        /// </summary>
        void DelayBehavior(AnimationDelayBehavior const& value)
        {
            SetValue(DelayBehaviorProperty, winrt::box_value(value));
        }

        /// <inheritdoc/>
        virtual Animations::AnimationBuilder& AppendToBuilder(
            Animations::AnimationBuilder& builder,
            [[maybe_unused]] std::optional<winrt::Windows::Foundation::TimeSpan> delayHint = std::nullopt,
            [[maybe_unused]] std::optional<winrt::Windows::Foundation::TimeSpan> durationHint = std::nullopt,
            [[maybe_unused]] std::optional<enum EasingType> easingTypeHint = std::nullopt,
            [[maybe_unused]] std::optional<enum EasingMode> easingModeHint = std::nullopt)
        {
            return builder;
        }

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> DelayProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> DurationProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> EasingTypeProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> EasingModeProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> RepeatProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> DelayBehaviorProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Animation : AnimationT<Animation, implementation::Animation>
    {
    };
}
