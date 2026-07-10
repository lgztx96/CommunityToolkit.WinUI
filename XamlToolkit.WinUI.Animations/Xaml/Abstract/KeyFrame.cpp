#include "pch.h"
#include "winrt_module_imports.h"
#include "KeyFrame.h"
#if __has_include("KeyFrame.g.cpp")
#include "KeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> KeyFrame::KeyProperty =
        winrt::DependencyProperty::Register(
            L"Key",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<winrt::DependencyProperty> KeyFrame::ExpressionProperty =
        winrt::DependencyProperty::Register(
            L"Expression",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> KeyFrame::EasingTypeProperty =
        winrt::DependencyProperty::Register(
            L"EasingType",
            winrt::xaml_typename<winrt::IReference<enum EasingType>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> KeyFrame::EasingModeProperty =
        winrt::DependencyProperty::Register(
            L"EasingMode",
            winrt::xaml_typename<winrt::IReference<winrt::EasingMode>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr });
}

