#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsGroup.h"


namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::RootGridCornerRadiusProperty =
        winrt::DependencyProperty::Register(
            L"RootGridCornerRadius",
            winrt::xaml_typename<winrt::CornerRadius>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(winrt::CornerRadius{ 0 }) });

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::ItemsProperty =
        winrt::DependencyProperty::Register(
            L"Items",
            winrt::xaml_typename<winrt::IObservableVector<winrt::IInspectable>>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ nullptr, &SettingsGroup::OnItemsChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::ContentProperty =
        winrt::DependencyProperty::Register(
            L"Content",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ winrt::IInspectable{}, &SettingsGroup::OnContentChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::HeaderIconProperty =
        winrt::DependencyProperty::Register(
            L"HeaderIcon",
            winrt::xaml_typename<winrt::IconElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ nullptr, &SettingsGroup::OnHeaderIconChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::HeaderProperty =
        winrt::DependencyProperty::Register(
            L"Header",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ nullptr, &SettingsGroup::OnHeaderChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsGroup::DescriptionProperty =
        winrt::DependencyProperty::Register(
            L"Description",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsGroup>(),
            winrt::PropertyMetadata{ nullptr, &SettingsGroup::OnDescriptionChanged });

    winrt::CornerRadius SettingsGroup::RootGridCornerRadius() const noexcept
    {
        return winrt::unbox_value<winrt::CornerRadius>(GetValue(RootGridCornerRadiusProperty));
    }

    void SettingsGroup::RootGridCornerRadius(winrt::CornerRadius const& value) noexcept
    {
        SetValue(RootGridCornerRadiusProperty, winrt::box_value(value));
    }

    winrt::IObservableVector<winrt::IInspectable> SettingsGroup::Items() const noexcept
    {
        return winrt::unbox_value<winrt::IObservableVector<winrt::IInspectable>>(GetValue(ItemsProperty));
    }

    void SettingsGroup::Items(winrt::IObservableVector<winrt::IInspectable> const& value) noexcept
    {
        SetValue(ItemsProperty, winrt::box_value(value));
    }

    winrt::IInspectable SettingsGroup::Content() const noexcept
    {
        return winrt::unbox_value<winrt::IInspectable>(GetValue(ContentProperty));
    }

    void SettingsGroup::Content(winrt::IInspectable const& value) noexcept
    {
        SetValue(ContentProperty, winrt::box_value(value));
    }

    winrt::IconElement SettingsGroup::HeaderIcon() const noexcept
    {
        return winrt::unbox_value<winrt::IconElement>(GetValue(HeaderIconProperty));
    }

    void SettingsGroup::HeaderIcon(winrt::IconElement const& value) noexcept
    {
        SetValue(HeaderIconProperty, winrt::box_value(value));
    }

    winrt::IInspectable SettingsGroup::Header() const noexcept
    {
        return winrt::unbox_value<winrt::IInspectable>(GetValue(HeaderProperty));
    }

    void SettingsGroup::Header(winrt::IInspectable const& value) noexcept
    {
        SetValue(HeaderProperty, winrt::box_value(value));
    }

    winrt::IInspectable SettingsGroup::Description() const noexcept
    {
        return winrt::unbox_value<winrt::IInspectable>(GetValue(DescriptionProperty));
    }

    void SettingsGroup::Description(winrt::IInspectable const& value) noexcept
    {
        SetValue(DescriptionProperty, winrt::box_value(value));
    }
}