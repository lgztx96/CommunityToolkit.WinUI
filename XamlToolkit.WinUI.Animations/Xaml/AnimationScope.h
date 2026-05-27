#pragma once

#include "AnimationScope.g.h"
#include "Interfaces/ITimeline.h"
#include "Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A container of ITimeline elements that can be used to conceptually group animations
    /// together and to assign shared properties to be applied to all the contained items automatically.
    /// Implements ITimeline as a C++ interface for AnimationBuilder integration.
    /// </summary>
    struct AnimationScope : AnimationScopeT<AnimationScope>, public ITimeline
    {
    public:
        AnimationScope() = default;

        /// <summary>
        /// Gets or sets the optional initial delay for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<Windows::Foundation::TimeSpan> Delay() const
        {
            return GetValue(DelayProperty).try_as<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>();
        }
        void Delay(winrt::Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
        {
            SetValue(DelayProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the animation duration.
        /// </summary>
        winrt::Windows::Foundation::IReference<Windows::Foundation::TimeSpan> Duration() const
        {
            return GetValue(DurationProperty).try_as<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>();
        }
        void Duration(winrt::Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
        {
            SetValue(DurationProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function type for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<enum EasingType> EasingType() const
        {
            return GetValue(EasingTypeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingType>>();
        }
        void EasingType(winrt::Windows::Foundation::IReference<enum EasingType> const& value)
        {
            SetValue(EasingTypeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function mode for the animation.
        /// </summary>
        winrt::Windows::Foundation::IReference<enum EasingMode> EasingMode() const
        {
            return GetValue(EasingModeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingMode>>();
        }
        void EasingMode(winrt::Windows::Foundation::IReference<enum EasingMode> const& value)
        {
            SetValue(EasingModeProperty, winrt::box_value(value));
        }

        /// <inheritdoc/>
        Animations::AnimationBuilder& AppendToBuilder(
            Animations::AnimationBuilder& builder,
            std::optional<Windows::Foundation::TimeSpan> delayHint = std::nullopt,
            std::optional<Windows::Foundation::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<enum EasingMode> easingModeHint = std::nullopt) override;

        /// <summary>
        /// Dependency property for Delay.
        /// </summary>
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> DelayProperty;

        /// <summary>
        /// Dependency property for Duration.
        /// </summary>
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> DurationProperty;

        /// <summary>
        /// Dependency property for EasingType.
        /// </summary>
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> EasingTypeProperty;

        /// <summary>
        /// Dependency property for EasingMode.
        /// </summary>
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> EasingModeProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationScope : AnimationScopeT<AnimationScope, implementation::AnimationScope> {};
}
