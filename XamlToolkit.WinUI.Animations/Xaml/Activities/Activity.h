#pragma once

#include "Activity.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Activity : ActivityT<Activity>
    {
    public:
        Activity() = default;

        winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> Delay();
        void Delay(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const& value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> DelayProperty;

        virtual winrt::Windows::Foundation::IAsyncAction InvokeAsync(Microsoft::UI::Xaml::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Activity : ActivityT<Activity, implementation::Activity> {};
}
