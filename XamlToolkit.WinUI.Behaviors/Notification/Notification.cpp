#include "pch.h"
#include "winrt_module_imports.h"
#include "Notification.h"
#if __has_include("Notification.g.cpp")
#include "Notification.g.cpp"
#endif

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

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::TitleProperty =
        winrt::DependencyProperty::Register(
            L"Title",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::MessageProperty =
        winrt::DependencyProperty::Register(
            L"Message",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::DurationProperty =
        winrt::DependencyProperty::Register(
            L"Duration",
            winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::SeverityProperty =
        winrt::DependencyProperty::Register(
            L"Severity",
            winrt::xaml_typename<winrt::IReference<winrt::InfoBarSeverity>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::IsIconVisibleProperty =
        winrt::DependencyProperty::Register(
            L"IsIconVisible",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(true)));

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::IconSourceProperty =
        winrt::DependencyProperty::Register(
            L"IconSource",
            winrt::xaml_typename<winrt::IconSource>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::ContentProperty =
        winrt::DependencyProperty::Register(
            L"Content",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::ContentTemplateProperty =
        winrt::DependencyProperty::Register(
            L"ContentTemplate",
            winrt::xaml_typename<winrt::DataTemplate>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> Notification::ActionButtonProperty =
        winrt::DependencyProperty::Register(
            L"ActionButton",
            winrt::xaml_typename<winrt::ButtonBase>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::hstring Notification::Title() const
    {
        return winrt::unbox_value_or<winrt::hstring>(GetValue(TitleProperty()), L"");
    }

    void Notification::Title(winrt::hstring const& value)
    {
        SetValue(TitleProperty(), winrt::box_value(value));
    }

    winrt::hstring Notification::Message() const
    {
        return winrt::unbox_value_or<winrt::hstring>(GetValue(MessageProperty()), L"");
    }

    void Notification::Message(winrt::hstring const& value)
    {
        SetValue(MessageProperty(), winrt::box_value(value));
    }

    winrt::IReference<winrt::TimeSpan> Notification::Duration() const
    {
        return GetValue(DurationProperty()).try_as<winrt::IReference<winrt::TimeSpan>>();
    }

    void Notification::Duration(winrt::IReference<winrt::TimeSpan> const& value)
    {
        SetValue(DurationProperty(), value);
    }

    winrt::IReference<winrt::InfoBarSeverity> Notification::Severity() const
    {
        return GetValue(SeverityProperty()).try_as<winrt::IReference<winrt::InfoBarSeverity>>();
    }

    void Notification::Severity(winrt::IReference<winrt::InfoBarSeverity> const& value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::Severity);
        SetValue(SeverityProperty(), value);
    }

    bool Notification::IsIconVisible() const
    {
        return winrt::unbox_value<bool>(GetValue(IsIconVisibleProperty()));
    }

    void Notification::IsIconVisible(bool value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::IconVisible);
        SetValue(IsIconVisibleProperty(), winrt::box_value(value));
    }

    winrt::IconSource Notification::IconSource() const
    {
        return GetValue(IconSourceProperty()).try_as<winrt::IconSource>();
    }

    void Notification::IconSource(winrt::IconSource const& value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::IconSource);
        SetValue(IconSourceProperty(), value);
    }

    winrt::IInspectable Notification::Content() const
    {
        return GetValue(ContentProperty());
    }

    void Notification::Content(winrt::IInspectable const& value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::Content);
        SetValue(ContentProperty(), value);
    }

    winrt::DataTemplate Notification::ContentTemplate() const
    {
        return GetValue(ContentTemplateProperty()).try_as<winrt::DataTemplate>();
    }

    void Notification::ContentTemplate(winrt::DataTemplate const& value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::ContentTemplate);
        SetValue(ContentTemplateProperty(), value);
    }

    winrt::ButtonBase Notification::ActionButton() const
    {
        return GetValue(ActionButtonProperty()).try_as<winrt::ButtonBase>();
    }

    void Notification::ActionButton(winrt::ButtonBase const& value)
    {
        _overrides |= static_cast<uint32_t>(NotificationOverrides::ActionButton);
        SetValue(ActionButtonProperty(), value);
    }
}
