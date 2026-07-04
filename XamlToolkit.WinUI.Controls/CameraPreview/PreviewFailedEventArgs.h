#pragma once

#include "PreviewFailedEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct PreviewFailedEventArgs : PreviewFailedEventArgsT<PreviewFailedEventArgs>
    {
        PreviewFailedEventArgs() = default;

        PreviewFailedEventArgs(winrt::hstring const& error) : Error(error) {}

        wil::single_threaded_property<winrt::hstring> Error;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct PreviewFailedEventArgs : PreviewFailedEventArgsT<PreviewFailedEventArgs, implementation::PreviewFailedEventArgs>
    {
    };
}
