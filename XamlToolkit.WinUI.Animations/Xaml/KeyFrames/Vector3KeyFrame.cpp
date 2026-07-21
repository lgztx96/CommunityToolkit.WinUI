#include "pch.h"
#include "winrt_module_imports.h"
#include "Vector3KeyFrame.h"
#if __has_include("Vector3KeyFrame.g.cpp")
#include "Vector3KeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Vector3KeyFrame::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));
}
