#pragma once

#include "SettingsExpanderItemStyleSelector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SettingsExpanderItemStyleSelector : SettingsExpanderItemStyleSelectorT<SettingsExpanderItemStyleSelector>
    {
        SettingsExpanderItemStyleSelector() = default;

        wil::single_threaded_rw_property<winrt::Style> DefaultStyle{ nullptr };

        wil::single_threaded_rw_property<winrt::Style> ClickableStyle{ nullptr };

        winrt::Style SelectStyleCore(winrt::IInspectable const& item, winrt::DependencyObject const& container);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsExpanderItemStyleSelector : SettingsExpanderItemStyleSelectorT<SettingsExpanderItemStyleSelector, implementation::SettingsExpanderItemStyleSelector>
    {
    };
}
