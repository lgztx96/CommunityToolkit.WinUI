#include "pch.h"
#include "winrt_module_imports.h"
#include "Vector4KeyFrame.h"
#if __has_include("Vector4KeyFrame.g.cpp")
#include "Vector4KeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> Vector4KeyFrame::ValueProperty =
        DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            nullptr);
}
