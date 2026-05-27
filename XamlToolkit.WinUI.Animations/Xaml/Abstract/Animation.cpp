#include "pch.h"
#include "winrt_module_imports.h"
#include "Animation.h"
#if __has_include("Animation.g.cpp")
#include "Animation.g.cpp"
#endif
#include "../Options/RepeatOptionHelper.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> Animation::DelayProperty =
        DependencyProperty::Register(
            L"Delay",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> Animation::DurationProperty =
        DependencyProperty::Register(
            L"Duration",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> Animation::EasingTypeProperty =
        DependencyProperty::Register(
            L"EasingType",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> Animation::EasingModeProperty =
        DependencyProperty::Register(
            L"EasingMode",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> Animation::RepeatProperty =
        DependencyProperty::Register(
            L"Repeat",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::RepeatOption>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(RepeatOptionHelper::Once())));

    const wil::single_threaded_property<DependencyProperty> Animation::DelayBehaviorProperty =
        DependencyProperty::Register(
            L"DelayBehavior",
            winrt::xaml_typename<winrt::Microsoft::UI::Composition::AnimationDelayBehavior>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(AnimationDelayBehavior::SetInitialValueBeforeDelay)));
}
