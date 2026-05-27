#pragma once

#include "KeyDownTriggerBehavior.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Windows::Foundation;
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// This behavior listens to a key down event on the associated UIElement when it is loaded and executes an action.
    /// </summary>
    struct KeyDownTriggerBehavior : KeyDownTriggerBehaviorT<KeyDownTriggerBehavior>
    {
        KeyDownTriggerBehavior() = default;

        /// <summary>
        /// Gets or sets the key to listen when the associated object is loaded.
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
        /// Called after the behavior is attached to the AssociatedObject.
        /// </summary>
        void OnAttached();

        /// <summary>
        /// Called when the behavior is being detached from its AssociatedObject.
        /// </summary>
        void OnDetaching();

    private:
        /// <summary>
        /// Invokes the current actions when the Key is pressed.
        /// </summary>
        /// <param name="sender">The source UIElement instance.</param>
        /// <param name="args">The arguments for the event (unused).</param>
        void OnAssociatedObjectKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args);

        winrt::FrameworkElement::KeyDown_revoker _keyDownRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct KeyDownTriggerBehavior : KeyDownTriggerBehaviorT<KeyDownTriggerBehavior, implementation::KeyDownTriggerBehavior>
    {
    };
}
