#pragma once

#include "KeyDownTriggerBehavior.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Input;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Windows::Foundation;
    using namespace Windows::UI::Core;
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// A behavior that listens to PreviewKeyDown on the associated
    /// FrameworkElement and executes its actions when the specified key and
    /// optional modifier keys are pressed. Supports capturing handled events.
    /// </summary>
    struct KeyDownTriggerBehavior : KeyDownTriggerBehaviorT<KeyDownTriggerBehavior>
    {
        KeyDownTriggerBehavior() = default;

        /// <summary>
        /// Gets or sets the key that triggers the behavior.
        /// </summary>
        winrt::VirtualKey Key() const
        {
            return winrt::unbox_value<winrt::VirtualKey>(GetValue(KeyProperty()));
        }

        void Key(winrt::VirtualKey const& value)
        {
            SetValue(KeyProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> KeyProperty;

        /// <summary>
        /// Gets or sets the modifier keys that must be pressed together with Key.
        /// </summary>
        winrt::VirtualKeyModifiers Modifiers() const
        {
            return winrt::unbox_value<winrt::VirtualKeyModifiers>(GetValue(ModifiersProperty()));
        }

        void Modifiers(winrt::VirtualKeyModifiers const& value)
        {
            SetValue(ModifiersProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ModifiersProperty;

        /// <summary>
        /// Gets or sets a value indicating whether the behavior should evaluate
        /// the current modifier key state when matching the Key.
        /// </summary>
        bool ModifiersEnabled() const
        {
            return winrt::unbox_value<bool>(GetValue(ModifiersEnabledProperty()));
        }

        void ModifiersEnabled(bool value)
        {
            SetValue(ModifiersEnabledProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ModifiersEnabledProperty;

        /// <summary>
        /// Gets or sets a value indicating whether the behavior should receive
        /// PreviewKeyDown events even if they were already handled.
        /// </summary>
        bool HandledEventsToo() const
        {
            return winrt::unbox_value<bool>(GetValue(HandledEventsTooProperty()));
        }

        void HandledEventsToo(bool value)
        {
            SetValue(HandledEventsTooProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> HandledEventsTooProperty;

        /// <summary>
        /// Called after the behavior is attached to the AssociatedObject.
        /// </summary>
        void OnAttached();

        /// <summary>
        /// Called when the behavior is being detached from its AssociatedObject.
        /// </summary>
        void OnDetaching();

    private:
        /// <summary>
        /// Handles the PreviewKeyDown event and executes the associated actions
        /// when the specified Key and Modifiers match.
        /// </summary>
        /// <param name="sender">The source UIElement instance.</param>
        /// <param name="args">The arguments for the event.</param>
        void OnAssociatedObjectPreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args);

        /// <summary>
        /// Checks whether all required modifier keys specified in Modifiers
        /// are currently pressed.
        /// </summary>
        /// <returns>true if the current modifier state matches; otherwise, false.</returns>
        bool CheckModifiers();

        /// <summary>
        /// Compares whether a specific modifier flag is required and whether the
        /// corresponding key is currently pressed.
        /// </summary>
        /// <param name="mod">The modifier flag to evaluate.</param>
        /// <param name="isDown">The current physical key state for that modifier.</param>
        /// <returns>true if the requirement matches the key state; otherwise, false.</returns>
        bool Match(winrt::VirtualKeyModifiers mod, bool isDown) const;

        winrt::IInspectable _previewKeyDownHandler{ nullptr };
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct KeyDownTriggerBehavior : KeyDownTriggerBehaviorT<KeyDownTriggerBehavior, implementation::KeyDownTriggerBehavior>
    {
    };
}
