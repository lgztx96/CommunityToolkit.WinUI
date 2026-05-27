#pragma once

#include "TokenizingTextBoxStyleSelector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct TokenizingTextBoxStyleSelector : TokenizingTextBoxStyleSelectorT<TokenizingTextBoxStyleSelector>
    {
        TokenizingTextBoxStyleSelector() = default;

        wil::single_threaded_rw_property<Style> TokenStyle;

        wil::single_threaded_rw_property<Style> TextStyle;

        Style SelectStyleCore(IInspectable const& item, DependencyObject const& container);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct TokenizingTextBoxStyleSelector : TokenizingTextBoxStyleSelectorT<TokenizingTextBoxStyleSelector, implementation::TokenizingTextBoxStyleSelector>
    {
    };
}
