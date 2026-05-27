#pragma once

#include "StackedNotificationsBehavior.g.h"
#include "Notification.h"
#include "../BehaviorBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <list>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Dispatching;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    // Notification override flags
    enum class NotificationOverrides : uint32_t
    {
        None = 0,
        Severity = 1 << 0,
        IconVisible = 1 << 1,
        IconSource = 1 << 2,
        Content = 1 << 3,
        ContentTemplate = 1 << 4,
        ActionButton = 1 << 5
    };

    /// <summary>
    /// A behavior to add the stacked notification support to InfoBar.
    /// </summary>
    struct StackedNotificationsBehavior : StackedNotificationsBehaviorT<StackedNotificationsBehavior>, BehaviorBase<StackedNotificationsBehavior, winrt::InfoBar>
    {
        StackedNotificationsBehavior();

        /// <summary>
        /// Show notification using text as the Message of the notification.
        /// </summary>
        /// <param name="message">Message string to display as the notification.</param>
        /// <param name="duration">Optional, displayed duration of the notification in ms (less or equal 0 means infinite duration).</param>
        /// <param name="title">Optional, Title for the notification.</param>
        /// <returns>The constructed Notification added to the queue.</returns>
        winrt::XamlToolkit::WinUI::Behaviors::Notification Show(winrt::hstring const& message, int32_t duration, winrt::hstring const& title);

        /// <summary>
        /// Show notification using object or UIElement as the Content of the notification. Note, it is
        /// generally best to also specific a message and/or title.
        /// </summary>
        /// <param name="content">Content to display as the notification.</param>
        /// <param name="duration">Optional, displayed duration of the notification in ms (less or equal 0 means infinite duration).</param>
        /// <param name="title">Optional, Title for the notification.</param>
        /// <param name="message">Optional, Message string to display as the notification.</param>
        /// <returns>The constructed Notification added to the queue.</returns>
        winrt::XamlToolkit::WinUI::Behaviors::Notification Show(winrt::IInspectable const& content, int32_t duration, winrt::hstring const& title, winrt::hstring const& message);

        /// <summary>
        /// Show the specified notification.
        /// </summary>
        /// <param name="notification">The notification to display.</param>
        /// <returns>The Notification added to the queue.</returns>
        winrt::XamlToolkit::WinUI::Behaviors::Notification Show(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification);

        /// <summary>
        /// Remove the notification.
        /// If the notification is displayed, it will be closed.
        /// If the notification is still in the queue, it will be removed.
        /// </summary>
        /// <param name="notification">The notification to remove.</param>
        void Remove(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification);

        /// <summary>
        /// Clears all notifications and closes any open notification.
        /// </summary>
        void Clear();

    protected:
        /// <summary>
        /// Initializes the behavior to the associated object.
        /// </summary>
        /// <returns>true if initialization succeeded; otherwise false.</returns>
        bool Initialize() override;

        /// <summary>
        /// Uninitializes the behavior from the associated object.
        /// </summary>
        /// <returns>true if uninitialization succeeded; otherwise false.</returns>
        bool Uninitialize() override;

    private:
        void ShowNext();
        void SetNotification(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification);
        void RestoreOverridenProperties();
        void StartTimer(winrt::IReference<winrt::TimeSpan> const& duration);
        void StopTimer();

        void OnInfoBarClosed(winrt::InfoBar const& sender, winrt::InfoBarClosedEventArgs const& args);
        void OnPointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void OnPointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void OnTimerTick(winrt::IInspectable const& sender, winrt::IInspectable const& args);

        std::list<winrt::XamlToolkit::WinUI::Behaviors::Notification> _stackedNotifications;
        winrt::DispatcherQueueTimer _dismissTimer{ nullptr };
        winrt::XamlToolkit::WinUI::Behaviors::Notification _currentNotification{ nullptr };

        winrt::hstring _initialTitle;
        winrt::IReference<winrt::InfoBarSeverity> _initialSeverity;
        bool _initialIconVisible{ true };
        winrt::IconSource _initialIconSource{ nullptr };
        winrt::IInspectable _initialContent{ nullptr };
        winrt::DataTemplate _initialContentTemplate{ nullptr };
        winrt::ButtonBase _initialActionButton{ nullptr };

        winrt::InfoBar::Closed_revoker _closedRevoker;
        winrt::InfoBar::PointerEntered_revoker _pointerEnteredRevoker;
        winrt::InfoBar::PointerExited_revoker _pointerExitedRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct StackedNotificationsBehavior : StackedNotificationsBehaviorT<StackedNotificationsBehavior, implementation::StackedNotificationsBehavior>
    {
    };
}
