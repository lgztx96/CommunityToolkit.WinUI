#pragma once

#include "TokenItemRemovingEventArgs.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs>
    {
        TokenItemRemovingEventArgs() = default;

        TokenItemRemovingEventArgs(IInspectable const& item, winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem const& token): Item(item), Token(token) {}

        wil::single_threaded_property<IInspectable> Item;

        wil::single_threaded_property<winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBoxItem> Token;

        wil::single_threaded_rw_property<bool> Cancel;
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs, implementation::TokenItemRemovingEventArgs>
    {
    };
}
