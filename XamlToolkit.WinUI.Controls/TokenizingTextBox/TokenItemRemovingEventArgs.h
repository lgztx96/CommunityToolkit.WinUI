#pragma once

#include "TokenItemRemovingEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs>, winrt::deferrable_event_args<TokenItemRemovingEventArgs>
    {
        TokenItemRemovingEventArgs() = default;

        TokenItemRemovingEventArgs(winrt::Windows::Foundation::IInspectable const& item, winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem const& token): Item(item), Token(token) {}

        wil::single_threaded_property<winrt::Windows::Foundation::IInspectable> Item;

        wil::single_threaded_property<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem> Token;

        wil::single_threaded_rw_property<bool> Cancel;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs, implementation::TokenItemRemovingEventArgs>
    {
    };
}
