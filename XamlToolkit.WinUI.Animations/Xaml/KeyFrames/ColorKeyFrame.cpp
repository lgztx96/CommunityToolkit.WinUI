#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorKeyFrame.h"
#if __has_include("ColorKeyFrame.g.cpp")
#include "ColorKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> ColorKeyFrame::ValueProperty =
        DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<Windows::UI::Color>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);
}
