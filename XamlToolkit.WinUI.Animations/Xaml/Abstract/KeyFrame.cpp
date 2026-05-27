#include "pch.h"
#include "winrt_module_imports.h"
#include "KeyFrame.h"
#if __has_include("KeyFrame.g.cpp")
#include "KeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> KeyFrame::KeyProperty = DependencyProperty::Register(
        L"Key",
        winrt::xaml_typename<double>(),
        winrt::xaml_typename<class_type>(),
        PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<DependencyProperty> KeyFrame::ExpressionProperty = DependencyProperty::Register(
        L"Expression",
        winrt::xaml_typename<winrt::hstring>(),
        winrt::xaml_typename<class_type>(),
        nullptr);

    const wil::single_threaded_property<DependencyProperty> KeyFrame::EasingTypeProperty = DependencyProperty::Register(
        L"EasingType",
        winrt::xaml_typename<winrt::Windows::Foundation::IReference<enum EasingType>>(),
        winrt::xaml_typename<class_type>(),
        nullptr);

    const wil::single_threaded_property<DependencyProperty> KeyFrame::EasingModeProperty = DependencyProperty::Register(
        L"EasingMode",
        winrt::xaml_typename<winrt::Windows::Foundation::IReference<enum EasingMode>>(),
        winrt::xaml_typename<class_type>(),
        nullptr);
}

