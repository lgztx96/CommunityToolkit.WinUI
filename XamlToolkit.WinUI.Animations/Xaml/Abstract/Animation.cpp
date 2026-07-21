#include "pch.h"
#include "winrt_module_imports.h"
#include "Animation.h"
#if __has_include("Animation.g.cpp")
#include "Animation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Animation::DelayProperty =
        winrt::DependencyProperty::Register(
            L"Delay",
            winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Animation::DurationProperty =
        winrt::DependencyProperty::Register(
            L"Duration",
            winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Animation::EasingTypeProperty =
        winrt::DependencyProperty::Register(
            L"EasingType",
            winrt::xaml_typename<winrt::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Animation::EasingModeProperty =
        winrt::DependencyProperty::Register(
            L"EasingMode",
            winrt::xaml_typename<winrt::IReference<winrt::EasingMode>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Animation::RepeatProperty =
        winrt::DependencyProperty::Register(
            L"Repeat",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::RepeatOption>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(RepeatOptionHelper::Once())));

    const wil::single_threaded_property<winrt::DependencyProperty> Animation::DelayBehaviorProperty =
        winrt::DependencyProperty::Register(
            L"DelayBehavior",
            winrt::xaml_typename<winrt::AnimationDelayBehavior>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::AnimationDelayBehavior::SetInitialValueBeforeDelay)));
}
