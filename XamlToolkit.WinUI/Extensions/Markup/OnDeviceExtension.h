#pragma once

#include "OnDeviceExtension.g.h"

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

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct OnDeviceExtension : OnDeviceExtensionT<OnDeviceExtension>
    {
        OnDeviceExtension() = default;

        wil::single_threaded_rw_property<winrt::IInspectable> Default{ nullptr };
        wil::single_threaded_rw_property<winrt::IInspectable> Desktop{ nullptr };
        wil::single_threaded_rw_property<winrt::IInspectable> Holographic{ nullptr };
        wil::single_threaded_rw_property<winrt::IInspectable> IoT{ nullptr };
        wil::single_threaded_rw_property<winrt::IInspectable> Team{ nullptr };
        wil::single_threaded_rw_property<winrt::IInspectable> Xbox{ nullptr };

        winrt::IInspectable ProvideValue() const;

        winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider) const;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct OnDeviceExtension : OnDeviceExtensionT<OnDeviceExtension, implementation::OnDeviceExtension>
    {
    };
}
