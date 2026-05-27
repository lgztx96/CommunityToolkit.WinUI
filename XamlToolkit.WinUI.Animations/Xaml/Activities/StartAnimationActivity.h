#pragma once

#include "StartAnimationActivity.g.h"
#include "Activity.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct StartAnimationActivity : StartAnimationActivityT<StartAnimationActivity, Activity>
    {
    public:
        StartAnimationActivity() = default;

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
            return GetValue(TargetObjectProperty).try_as<winrt::Microsoft::UI::Xaml::UIElement>();
		}

        void TargetObject(winrt::Microsoft::UI::Xaml::UIElement const& value)
        {
            SetValue(TargetObjectProperty, winrt::box_value(value));
		}

        winrt::Windows::Foundation::IAsyncAction InvokeAsync(Microsoft::UI::Xaml::UIElement const& element) override;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> AnimationProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> TargetObjectProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct StartAnimationActivity : StartAnimationActivityT<StartAnimationActivity, implementation::StartAnimationActivity> {};
}
