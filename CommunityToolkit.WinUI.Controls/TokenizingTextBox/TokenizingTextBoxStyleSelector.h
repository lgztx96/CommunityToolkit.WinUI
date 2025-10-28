#pragma once

#include "TokenizingTextBoxStyleSelector.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct TokenizingTextBoxStyleSelector : TokenizingTextBoxStyleSelectorT<TokenizingTextBoxStyleSelector>
    {
        TokenizingTextBoxStyleSelector() = default;

        wil::single_threaded_rw_property<Style> TokenStyle;

        wil::single_threaded_rw_property<Style> TextStyle;

        Style SelectStyleCore(IInspectable const& item, DependencyObject const& container);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct TokenizingTextBoxStyleSelector : TokenizingTextBoxStyleSelectorT<TokenizingTextBoxStyleSelector, implementation::TokenizingTextBoxStyleSelector>
    {
    };
}
