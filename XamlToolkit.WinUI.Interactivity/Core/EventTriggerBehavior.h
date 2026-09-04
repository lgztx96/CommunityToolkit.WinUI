#pragma once

#include "EventTriggerBehavior.g.h"
#include "../Trigger.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// A behavior that listens for a specified event on its source and executes its actions when that event is fired.
    /// </summary>
    struct EventTriggerBehavior : EventTriggerBehaviorT<EventTriggerBehavior, winrt::XamlToolkit::WinUI::Interactivity::implementation::Trigger>
    {
        /// <summary>
        /// Identifies the <seealso cref="EventName"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> EventNameProperty;

        /// <summary>
        /// Identifies the <seealso cref="SourceObject"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> SourceObjectProperty;

        EventTriggerBehavior() = default;

        /// <summary>
        /// Gets or sets the name of the event to listen for. This is a dependency property.
        /// </summary>
        winrt::hstring EventName() const;
        void EventName(winrt::hstring const& value);

        /// <summary>
        /// Gets or sets the source object from which this behavior listens for events.
        /// If <seealso cref="SourceObject"/> is not set, the source will default to <seealso cref="Behavior.AssociatedObject"/>. This is a dependency property.
        /// </summary>
        winrt::IInspectable SourceObject() const;
        void SourceObject(winrt::IInspectable const& value);

        /// <summary>
        /// Called after the behavior is attached to the <see cref="Behavior.AssociatedObject"/>.
        /// </summary>
        void OnAttached();

        /// <summary>
        /// Called when the behavior is being detached from its <see cref="Behavior.AssociatedObject"/>.
        /// </summary>
        void OnDetaching();

        static bool IsElementLoaded(winrt::FrameworkElement const& element);

    private:
        void SetResolvedSource(winrt::IInspectable const& newSource);
        winrt::IInspectable ComputeResolvedSource();
        void RegisterEvent(winrt::hstring const& eventName);
        void UnregisterEvent(winrt::hstring const& eventName);
        void OnEvent(winrt::IInspectable const& sender, winrt::IInspectable const& eventArgs);

        static void OnSourceObjectChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args);
        static void OnEventNameChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args);

        winrt::IInspectable _resolvedSource{ nullptr };
        winrt::event_token _registeredToken{};
        winrt::event_token _loadedToken{};
        bool _isLoadedEventRegistered{ false };
        winrt::hstring _currentEventName;
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct EventTriggerBehavior : EventTriggerBehaviorT<EventTriggerBehavior, implementation::EventTriggerBehavior>
    {
    };
}
