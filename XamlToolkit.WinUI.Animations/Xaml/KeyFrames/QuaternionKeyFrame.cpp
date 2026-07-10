#include "pch.h"
#include "winrt_module_imports.h"
#include "QuaternionKeyFrame.h"
#if __has_include("QuaternionKeyFrame.g.cpp")
#include "QuaternionKeyFrame.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> QuaternionKeyFrame::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));
}
