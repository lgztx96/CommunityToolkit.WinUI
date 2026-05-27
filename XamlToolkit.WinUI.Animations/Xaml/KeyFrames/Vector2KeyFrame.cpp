#include "pch.h"
#include "winrt_module_imports.h"
#include "Vector2KeyFrame.h"
#if __has_include("Vector2KeyFrame.g.cpp")
#include "Vector2KeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> Vector2KeyFrame::ValueProperty =
        DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            nullptr);
}
