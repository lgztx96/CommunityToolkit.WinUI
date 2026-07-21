#include "pch.h"
#include "winrt_module_imports.h"
#include "ThicknessKeyFrame.h"
#if __has_include("ThicknessKeyFrame.g.cpp")
#include "ThicknessKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ThicknessKeyFrame::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::IReference<winrt::Thickness>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));
}
