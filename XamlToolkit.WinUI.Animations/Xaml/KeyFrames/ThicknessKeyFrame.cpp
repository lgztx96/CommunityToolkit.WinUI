#include "pch.h"
#include "winrt_module_imports.h"
#include "ThicknessKeyFrame.h"
#if __has_include("ThicknessKeyFrame.g.cpp")
#include "ThicknessKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> ThicknessKeyFrame::ValueProperty =
        DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<Thickness>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);
}
