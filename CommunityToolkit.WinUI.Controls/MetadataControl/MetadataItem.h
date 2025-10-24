#pragma once

#include "MetadataItem.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct MetadataItem : MetadataItemT<MetadataItem, implementation::MetadataItem>
    {
    };
}
