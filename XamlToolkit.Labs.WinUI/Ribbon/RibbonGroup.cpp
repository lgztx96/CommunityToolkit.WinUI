#include "pch.h"
#include "winrt_module_imports.h"
#include "RibbonGroup.h"
#if __has_include("RibbonGroup.g.cpp")
#include "RibbonGroup.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> RibbonGroup::ContentProperty =
        winrt::DependencyProperty::Register(
            L"Content",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonGroup>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonGroup::LabelProperty =
        winrt::DependencyProperty::Register(
            L"Label",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(L"") });

    RibbonGroup::RibbonGroup()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void RibbonGroup::OnApplyTemplate() { base_type::OnApplyTemplate(); }
}
