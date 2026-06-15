#pragma once

#include "NullableBoolExtension.g.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct NullableBoolExtension : NullableBoolExtensionT<NullableBoolExtension>
    {
        NullableBoolExtension() = default;

        wil::single_threaded_rw_property<bool> Value{ false };

        wil::single_threaded_rw_property<bool> IsNull{ false };

		winrt::IInspectable ProvideValue() const;

		winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider) const;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct NullableBoolExtension : NullableBoolExtensionT<NullableBoolExtension, implementation::NullableBoolExtension>
    {
    };
}
