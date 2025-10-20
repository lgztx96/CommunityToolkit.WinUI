#pragma once

#include "LinkClickedEventArgs.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    struct LinkClickedEventArgs : LinkClickedEventArgsT<LinkClickedEventArgs>
    {
        LinkClickedEventArgs() : Uri(nullptr), Handled(false) {}
        LinkClickedEventArgs(Windows::Foundation::Uri const& uri) : Uri(uri), Handled(false) {}
        wil::single_threaded_property<Windows::Foundation::Uri> Uri;
        wil::single_threaded_rw_property<bool> Handled;
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct LinkClickedEventArgs : LinkClickedEventArgsT<LinkClickedEventArgs, implementation::LinkClickedEventArgs>
    {
    };
}
