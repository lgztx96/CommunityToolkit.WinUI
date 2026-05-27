#pragma once

#include "MetadataItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Input.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct MetadataItem : MetadataItemT<MetadataItem>
    {
        MetadataItem() = default;
        wil::single_threaded_rw_property<winrt::hstring> Label;
        wil::single_threaded_rw_property<winrt::hstring> AccessibleLabel;
        wil::single_threaded_rw_property<ICommand> Command;
        wil::single_threaded_rw_property<IInspectable> CommandParameter;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct MetadataItem : MetadataItemT<MetadataItem, implementation::MetadataItem>
    {
    };
}
