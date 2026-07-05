#pragma once

#include "SuggestionRequestedEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SuggestionRequestedEventArgs : SuggestionRequestedEventArgsT<SuggestionRequestedEventArgs>, winrt::deferrable_event_args<SuggestionRequestedEventArgs>
    {
        SuggestionRequestedEventArgs() = default;

        wil::single_threaded_rw_property<winrt::hstring> Prefix;

        wil::single_threaded_rw_property<winrt::hstring> QueryText;

        wil::single_threaded_rw_property<bool> Cancel{ false };
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SuggestionRequestedEventArgs : SuggestionRequestedEventArgsT<SuggestionRequestedEventArgs, implementation::SuggestionRequestedEventArgs>
    {
    };
}
