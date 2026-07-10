#pragma once

#include "Activity.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Activity : ActivityT<Activity>
    {
    public:
        Activity() = default;

        winrt::IReference<winrt::TimeSpan> Delay() const;
        void Delay(winrt::IReference<winrt::TimeSpan> const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> DelayProperty;

        virtual winrt::IAsyncAction InvokeAsync(winrt::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Activity : ActivityT<Activity, implementation::Activity> {};
}
