#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorKeyFrame.h"
#if __has_include("ColorKeyFrame.g.cpp")
#include "ColorKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ColorKeyFrame::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::IReference<winrt::Color>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));
}
