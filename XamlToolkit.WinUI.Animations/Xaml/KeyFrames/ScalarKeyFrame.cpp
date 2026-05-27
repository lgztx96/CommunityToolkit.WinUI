#include "pch.h"
#include "winrt_module_imports.h"
#include "ScalarKeyFrame.h"
#if __has_include("ScalarKeyFrame.g.cpp")
#include "ScalarKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> ScalarKeyFrame::ValueProperty =
        DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<double>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);
}
