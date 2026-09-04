#pragma once

#include "StopAnimationActivity.g.h"
#include "Activity.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct StopAnimationActivity : StopAnimationActivityT<StopAnimationActivity, Activity>
    {
    public:
        StopAnimationActivity() = default;

        winrt::XamlToolkit::WinUI::Animations::AnimationSet Animation() const
        {
            return GetValue(AnimationProperty()).try_as<winrt::XamlToolkit::WinUI::Animations::AnimationSet>();
        }
        void Animation(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
        {
            SetValue(AnimationProperty(), winrt::box_value(value));
        }

        winrt::UIElement TargetObject() const
        {
            return GetValue(TargetObjectProperty()).try_as<winrt::UIElement>();
        }
        void TargetObject(winrt::UIElement const& value)
        {
            SetValue(TargetObjectProperty(), winrt::box_value(value));
        }

        winrt::IAsyncAction InvokeAsync(winrt::UIElement element) override;

        static const wil::single_threaded_property<winrt::DependencyProperty> AnimationProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetObjectProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct StopAnimationActivity : StopAnimationActivityT<StopAnimationActivity, implementation::StopAnimationActivity> {};
}
