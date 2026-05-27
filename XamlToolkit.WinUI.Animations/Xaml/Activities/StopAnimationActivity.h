#pragma once

#include "StopAnimationActivity.g.h"
#include "Activity.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct StopAnimationActivity : StopAnimationActivityT<StopAnimationActivity, Activity>
    {
    public:
        StopAnimationActivity() = default;

        winrt::XamlToolkit::WinUI::Animations::AnimationSet Animation() const
        {
            return GetValue(AnimationProperty).try_as<winrt::XamlToolkit::WinUI::Animations::AnimationSet>();
        }
        void Animation(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
        {
            SetValue(AnimationProperty, winrt::box_value(value));
        }

        winrt::Microsoft::UI::Xaml::UIElement TargetObject() const
        {
            return GetValue(TargetObjectProperty).try_as<Microsoft::UI::Xaml::UIElement>();
        }
        void TargetObject(Microsoft::UI::Xaml::UIElement const& value)
        {
            SetValue(TargetObjectProperty, winrt::box_value(value));
        }

        winrt::Windows::Foundation::IAsyncAction InvokeAsync(winrt::Microsoft::UI::Xaml::UIElement const& element) override;

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AnimationProperty;
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TargetObjectProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct StopAnimationActivity : StopAnimationActivityT<StopAnimationActivity, implementation::StopAnimationActivity> {};
}
