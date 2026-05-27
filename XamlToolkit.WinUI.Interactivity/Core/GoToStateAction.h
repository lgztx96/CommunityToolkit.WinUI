#pragma once

#include "GoToStateAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that will transition a <see cref="FrameworkElement"/> to a specified <seealso cref="VisualState"/> when executed.
    /// </summary>
    /// <remarks>
    /// If the <seealso cref="TargetObject"/> property is set, this action will attempt to change the state of the targeted element. If it is not set, the action walks
    /// the element tree in an attempt to locate an alternative target that defines states. <see cref="ControlTemplate"/> and <see cref="UserControl"/> are
    /// two common results.
    /// </remarks>
    struct GoToStateAction : GoToStateActionT<GoToStateAction>
    {
        GoToStateAction() = default;

        /// <summary>
        /// Identifies the <seealso cref="UseTransitions"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> UseTransitionsProperty;
        /// <summary>
        /// Identifies the <seealso cref="StateName"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> StateNameProperty;
        /// <summary>
        /// Identifies the <seealso cref="TargetObject"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetObjectProperty;

        /// <summary>
        /// Gets or sets whether or not to use a <see cref="VisualTransition"/> to transition between states. This is a dependency property.
        /// </summary>
        bool UseTransitions() const;
        void UseTransitions(bool value);

        /// <summary>
        /// Gets or sets the name of the <see cref="VisualState"/>. This is a dependency property.
        /// </summary>
        winrt::hstring StateName() const;
        void StateName(winrt::hstring const& value);

        /// <summary>
        /// Gets or sets the target object. This is a dependency property.
        /// </summary>
        winrt::FrameworkElement TargetObject() const;
        void TargetObject(winrt::FrameworkElement const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if the transition to the specified state succeeds; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct GoToStateAction : GoToStateActionT<GoToStateAction, implementation::GoToStateAction>
    {
    };
}
