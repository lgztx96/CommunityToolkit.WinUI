#pragma once

#include "LinkClickedEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct LinkClickedEventArgs : LinkClickedEventArgsT<LinkClickedEventArgs>
    {
        LinkClickedEventArgs() : Uri(nullptr), Handled(false) {}

        LinkClickedEventArgs(winrt::Windows::Foundation::Uri const& uri) : Uri(uri), Handled(false) {}

        wil::single_threaded_property<winrt::Windows::Foundation::Uri> Uri;

        wil::single_threaded_rw_property<bool> Handled;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct LinkClickedEventArgs : LinkClickedEventArgsT<LinkClickedEventArgs, implementation::LinkClickedEventArgs>
    {
    };
}
