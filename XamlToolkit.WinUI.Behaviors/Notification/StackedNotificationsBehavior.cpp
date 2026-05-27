#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <chrono>
#endif
#include "StackedNotificationsBehavior.h"
#if __has_include("StackedNotificationsBehavior.g.cpp")
#include "StackedNotificationsBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    StackedNotificationsBehavior::StackedNotificationsBehavior()
    {
        _dismissTimer = winrt::DispatcherQueue::GetForCurrentThread().CreateTimer();
        _dismissTimer.Tick({ this, &StackedNotificationsBehavior::OnTimerTick });
    }

    bool StackedNotificationsBehavior::Initialize()
    {
        auto infoBar = BehaviorBase::AssociatedObject();
        if (!infoBar) return false;

        _closedRevoker = infoBar.Closed(winrt::auto_revoke, { this, &StackedNotificationsBehavior::OnInfoBarClosed });
        _pointerEnteredRevoker = infoBar.PointerEntered(winrt::auto_revoke, { this, &StackedNotificationsBehavior::OnPointerEntered });
        _pointerExitedRevoker = infoBar.PointerExited(winrt::auto_revoke, { this, &StackedNotificationsBehavior::OnPointerExited });

        return true;
    }

    bool StackedNotificationsBehavior::Uninitialize()
    {
        StopTimer();

        _closedRevoker.revoke();
        _pointerEnteredRevoker.revoke();
        _pointerExitedRevoker.revoke();

        return true;
    }

    winrt::XamlToolkit::WinUI::Behaviors::Notification StackedNotificationsBehavior::Show(winrt::hstring const& message, int32_t duration, winrt::hstring const& title)
    {
        auto notification = winrt::make<winrt::XamlToolkit::WinUI::Behaviors::implementation::Notification>();
        notification.Title(title);
        notification.Message(message);
        if (duration > 0)
        {
            notification.Duration(winrt::TimeSpan{ std::chrono::milliseconds(duration) });
        }

        return Show(notification);
    }

    winrt::XamlToolkit::WinUI::Behaviors::Notification StackedNotificationsBehavior::Show(winrt::IInspectable const& content, int32_t duration, winrt::hstring const& title, winrt::hstring const& message)
    {
        auto notification = winrt::make<winrt::XamlToolkit::WinUI::Behaviors::implementation::Notification>();
        notification.Title(title);
        notification.Message(message);
        notification.Content(content);
        if (duration > 0)
        {
            notification.Duration(winrt::TimeSpan{ std::chrono::milliseconds(duration) });
        }

        return Show(notification);
    }

    winrt::XamlToolkit::WinUI::Behaviors::Notification StackedNotificationsBehavior::Show(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification)
    {
        if (!notification)
        {
            throw winrt::hresult_invalid_argument(L"notification");
        }

        _stackedNotifications.push_back(notification);
        ShowNext();

        return notification;
    }

    void StackedNotificationsBehavior::Remove(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification)
    {
        if (!notification)
        {
			throw winrt::hresult_invalid_argument(L"notification");
        }

        if (notification == _currentNotification)
        {
            // We close the notification. This will trigger the display of the next one.
            // See OnInfoBarClosed.
            auto infoBar = BehaviorBase::AssociatedObject();
            infoBar.IsOpen(false);
            return;
        }

        _stackedNotifications.remove(notification);
    }

    void StackedNotificationsBehavior::Clear()
    {
        _stackedNotifications.clear();

        BehaviorBase::AssociatedObject().IsOpen(false);
    }

    void StackedNotificationsBehavior::ShowNext()
    {
        auto infoBar = BehaviorBase::AssociatedObject();

        if (!infoBar || infoBar.IsOpen())
        {
            // One notification is already displayed. We wait for it to close
            return;
        }

        StopTimer();
        infoBar.IsOpen(false);
        RestoreOverridenProperties();

        if (_stackedNotifications.empty())
        {
            _currentNotification = nullptr;
            return;
        }

        auto notificationToDisplay = _stackedNotifications.front();
        _stackedNotifications.pop_front();

        _currentNotification = notificationToDisplay;
        SetNotification(notificationToDisplay);
        infoBar.IsOpen(true);

        StartTimer(notificationToDisplay.Duration());
    }

    void StackedNotificationsBehavior::SetNotification(winrt::XamlToolkit::WinUI::Behaviors::Notification const& notification)
    {
        auto infoBar = BehaviorBase::AssociatedObject();
        if (!infoBar) return;

        auto impl = winrt::get_self<Notification>(notification);
        uint32_t overrides = impl->Overrides();

        if (!notification.Title().empty())
        {
            _initialTitle = infoBar.Title();
            infoBar.Title(notification.Title());
        }

        infoBar.Message(notification.Message());

        if (overrides & static_cast<uint32_t>(NotificationOverrides::Severity))
        {
            _initialSeverity = infoBar.Severity();
            infoBar.Severity(notification.Severity().Value());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::IconVisible))
        {
            _initialIconVisible = infoBar.IsIconVisible();
            infoBar.IsIconVisible(notification.IsIconVisible());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::IconSource))
        {
            _initialIconSource = infoBar.IconSource();
            infoBar.IconSource(notification.IconSource());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::Content))
        {
            _initialContent = infoBar.Content();
            infoBar.Content(notification.Content());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::ContentTemplate))
        {
            _initialContentTemplate = infoBar.ContentTemplate();
            infoBar.ContentTemplate(notification.ContentTemplate());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::ActionButton))
        {
            _initialActionButton = infoBar.ActionButton();
            infoBar.ActionButton(notification.ActionButton());
        }
    }

    void StackedNotificationsBehavior::RestoreOverridenProperties()
    {
        if (!_currentNotification) return;

        auto infoBar = BehaviorBase::AssociatedObject();
        auto impl = winrt::get_self<Notification>(_currentNotification);
        uint32_t overrides = impl->Overrides();

        if (!_currentNotification.Title().empty())
        {
            infoBar.Title(_initialTitle.empty() ? L"" : _initialTitle);
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::Severity))
        {
            infoBar.Severity(_initialSeverity.Value());
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::IconVisible))
        {
            infoBar.IsIconVisible(_initialIconVisible);
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::IconSource))
        {
            infoBar.IconSource(_initialIconSource);
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::Content))
        {
            infoBar.Content(_initialContent);
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::ContentTemplate))
        {
            infoBar.ContentTemplate(_initialContentTemplate);
        }

        if (overrides & static_cast<uint32_t>(NotificationOverrides::ActionButton))
        {
            infoBar.ActionButton(_initialActionButton);
        }
    }

    void StackedNotificationsBehavior::StartTimer(winrt::IReference<winrt::TimeSpan> const& duration)
    {
        if (!duration) return;

        _dismissTimer.Interval(duration.Value());
        _dismissTimer.Start();
    }

    void StackedNotificationsBehavior::StopTimer()
    {
        _dismissTimer.Stop();
    }

    void StackedNotificationsBehavior::OnInfoBarClosed([[maybe_unused]] winrt::InfoBar const& sender, [[maybe_unused]] winrt::InfoBarClosedEventArgs const& args)
    {
        // We display the next notification.
        ShowNext();
    }

    void StackedNotificationsBehavior::OnPointerEntered([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        StopTimer();
    }

    void StackedNotificationsBehavior::OnPointerExited([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        if (_currentNotification)
        {
            StartTimer(_currentNotification.Duration());
        }
    }

    void StackedNotificationsBehavior::OnTimerTick([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& e)
    {
        BehaviorBase::AssociatedObject().IsOpen(false);
    }
}
