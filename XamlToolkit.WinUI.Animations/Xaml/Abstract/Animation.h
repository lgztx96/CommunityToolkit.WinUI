#pragma once

#include "Animation.g.h"
#include "../Interfaces/ITimeline.h"
#include "../Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
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
        winrt::IReference<winrt::TimeSpan> Delay() const
        {
            return GetValue(DelayProperty).try_as<winrt::IReference<winrt::TimeSpan>>();
        }
        /// <summary>
        /// Gets or sets the initial delay before the animation starts.
        /// </summary>
        void Delay(winrt::IReference<winrt::TimeSpan> const& value)
        {
            SetValue(DelayProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the duration for the animation.
        /// </summary>
        winrt::IReference<winrt::TimeSpan> Duration() const
        {
            return GetValue(DurationProperty).try_as<winrt::IReference<winrt::TimeSpan>>();
        }
        /// <summary>
        /// Gets or sets the duration for the animation.
        /// </summary>
        void Duration(winrt::IReference<winrt::TimeSpan> const& value)
        {
            SetValue(DurationProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function type for the animation.
        /// </summary>
        winrt::IReference<EasingType> EasingType() const
        {
            return GetValue(EasingTypeProperty).try_as<winrt::IReference<enum EasingType>>();
        }
        /// <summary>
        /// Gets or sets the optional easing function type for the animation.
        /// </summary>
        void EasingType(winrt::IReference<enum EasingType> const& value)
        {
            SetValue(EasingTypeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function mode for the animation.
        /// </summary>
        winrt::IReference<winrt::EasingMode> EasingMode() const
        {
            return GetValue(EasingModeProperty).try_as<winrt::IReference<winrt::EasingMode>>();
        }
        /// <summary>
        /// Gets or sets the optional easing function mode for the animation.
        /// </summary>
        void EasingMode(winrt::IReference<winrt::EasingMode> const& value)
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
        winrt::AnimationDelayBehavior DelayBehavior()
        {
            return winrt::unbox_value<winrt::AnimationDelayBehavior>(GetValue(DelayBehaviorProperty));
        }
        /// <summary>
        /// Gets or sets the delay behavior for the animation.
        /// </summary>
        void DelayBehavior(winrt::AnimationDelayBehavior const& value)
        {
            SetValue(DelayBehaviorProperty, winrt::box_value(value));
        }

        /// <inheritdoc/>
        virtual winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            [[maybe_unused]] std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            [[maybe_unused]] std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            [[maybe_unused]] std::optional<enum EasingType> easingTypeHint = std::nullopt,
            [[maybe_unused]] std::optional<winrt::EasingMode> easingModeHint = std::nullopt)
        {
            return builder;
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> DelayProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> DurationProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> EasingTypeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> EasingModeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> RepeatProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> DelayBehaviorProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Animation : AnimationT<Animation, implementation::Animation>
    {
    };
}
