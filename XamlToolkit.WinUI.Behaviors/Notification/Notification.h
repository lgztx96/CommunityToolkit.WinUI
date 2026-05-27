#pragma once

#include "Notification.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// The content of a notification to display in StackedNotificationsBehavior.
    /// The Message and Duration values will always be applied to the targeted InfoBar.
    /// The Title, Severity, IsIconVisible, IconSource, Content, ContentTemplate and ActionButton values
    /// will be applied only if set, otherwise the parent InfoBar values will be used, if available.
    /// </summary>
    struct Notification : NotificationT<Notification>
    {
        Notification() = default;

        /// <summary>
        /// Gets or sets the notification title.
        /// </summary>
        winrt::hstring Title() const;
        void Title(winrt::hstring const& value);

        /// <summary>
        /// Gets or sets the notification message.
        /// </summary>
        winrt::hstring Message() const;
        void Message(winrt::hstring const& value);

        /// <summary>
        /// Gets or sets the duration of the notification.
        /// Set to null for persistent notification (default).
        /// </summary>
        winrt::IReference<winrt::TimeSpan> Duration() const;
        void Duration(winrt::IReference<winrt::TimeSpan> const& value);

        /// <summary>
        /// Gets or sets the type of the InfoBar.Severity to apply consistent status color, icon,
        /// and assistive technology settings dependent on the criticality of the notification.
        /// By default the parent InfoBar.Severity property's value will be used.
        /// </summary>
        winrt::IReference<winrt::InfoBarSeverity> Severity() const;
        void Severity(winrt::IReference<winrt::InfoBarSeverity> const& value);

        /// <summary>
        /// Gets or sets a value indicating whether the icon is visible or not.
        /// True if the icon is visible; otherwise, false. The default is true.
        /// </summary>
        bool IsIconVisible() const;
        void IsIconVisible(bool value);

        /// <summary>
        /// Gets or sets a value for an IconSource to use as the InfoBar.IconSource of the InfoBar for this notification.
        /// </summary>
        winrt::IconSource IconSource() const;
        void IconSource(winrt::IconSource const& value);

        /// <summary>
        /// Gets or sets the XAML Content that is displayed below the title and message in the InfoBar.
        /// </summary>
        winrt::IInspectable Content() const;
        void Content(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the data template for the Content.
        /// </summary>
        winrt::DataTemplate ContentTemplate() const;
        void ContentTemplate(winrt::DataTemplate const& value);

        /// <summary>
        /// Gets or sets the action button of the InfoBar.
        /// </summary>
        winrt::ButtonBase ActionButton() const;
        void ActionButton(winrt::ButtonBase const& value);

        uint32_t Overrides() const noexcept { return _overrides; }

        static const wil::single_threaded_property<winrt::DependencyProperty> TitleProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> MessageProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> DurationProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> SeverityProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> IsIconVisibleProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> IconSourceProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ContentProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ContentTemplateProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ActionButtonProperty;

    private:
        uint32_t _overrides{ 0 };
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct Notification : NotificationT<Notification, implementation::Notification>
    {
    };
}
