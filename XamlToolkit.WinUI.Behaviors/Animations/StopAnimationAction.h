#pragma once

#include "StopAnimationAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Animations.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// An IAction implementation that can stop a target AnimationSet instance.
    /// </summary>
    struct StopAnimationAction : StopAnimationActionT<StopAnimationAction>
    {
        StopAnimationAction() = default;

        /// <summary>
        /// Gets or sets the linked AnimationSet instance to stop.
        /// </summary>
        winrt::XamlToolkit::WinUI::Animations::AnimationSet Animation() const
        {
            return GetValue(AnimationProperty()).try_as<winrt::XamlToolkit::WinUI::Animations::AnimationSet>();
        }
        void Animation(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
        {
            SetValue(AnimationProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Identifies the Animation dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> AnimationProperty;

        /// <summary>
        /// Gets or sets the object to stop the specified animation on. If not specified, will use the current object the parent animation is running on.
        /// </summary>
        winrt::UIElement TargetObject() const
        {
            return GetValue(TargetObjectProperty()).try_as<winrt::UIElement>();
        }
        void TargetObject(winrt::UIElement const& value)
        {
            SetValue(TargetObjectProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Identifies the TargetObject dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetObjectProperty;

        /// <summary>
        /// Executes the action.
        /// </summary>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct StopAnimationAction : StopAnimationActionT<StopAnimationAction, implementation::StopAnimationAction>
    {
    };
}
