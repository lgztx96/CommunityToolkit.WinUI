#pragma once

#include "EventManager.g.h"

#ifdef __INTELLISENSE__
#include <string_view>
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    using Action = winrt::XamlToolkit::WinUI::Interactivity::EventAction;
    using RegisterHandler = winrt::event_token(*)(winrt::Windows::Foundation::IInspectable const&, Action const&);
    using UnregisterHandler = void (*)(winrt::Windows::Foundation::IInspectable const&, winrt::event_token);

    /// <summary>
    /// A manager class that handles event registration and unregistration for EventTriggerBehavior.
    /// </summary>
    struct EventManager
    {
        /// <summary>
        /// Adds an event description with its registration and unregistration handlers.
        /// </summary>
        /// <param name="eventName">The name of the event.</param>
        /// <param name="handler">The handler to register the event.</param>
        /// <param name="unregisterHandler">The handler to unregister the event.</param>
        static void AddDescription(
            std::wstring_view eventName,
            RegisterHandler handler,
            UnregisterHandler unregisterHandler);

        /// <summary>
        /// Registers an event handler for the specified event name.
        /// </summary>
        /// <param name="eventName">The name of the event to register.</param>
        /// <param name="sender">The object that raises the event.</param>
        /// <param name="action">The action to execute when the event is raised.</param>
        /// <returns>The event token that can be used to unregister the event.</returns>
        static winrt::event_token Register(
            winrt::hstring const& eventName,
            winrt::Windows::Foundation::IInspectable const& sender,
            Action const& action);

        /// <summary>
        /// Unregisters an event handler for the specified event name.
        /// </summary>
        /// <param name="eventName">The name of the event to unregister.</param>
        /// <param name="sender">The object that raised the event.</param>
        /// <param name="token">The event token returned from Register.</param>
        static void Unregister(
            winrt::hstring const& eventName,
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::event_token token);

    private:
        static void AddDefaultEvents();
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct EventManager : EventManagerT<EventManager, implementation::EventManager>
    {
    };
}
