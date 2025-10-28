#pragma once

#include "SymbolIconExtension.g.h"
#include "TextIconExtension.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    struct SymbolIconExtension : SymbolIconExtensionT<SymbolIconExtension, implementation::TextIconExtension>
    {
        SymbolIconExtension() = default;

        wil::single_threaded_rw_property<winrt::Microsoft::UI::Xaml::Controls::Symbol> Symbol;

        IInspectable ProvideValue() const;

        IInspectable ProvideValue(IXamlServiceProvider const& provider);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct SymbolIconExtension : SymbolIconExtensionT<SymbolIconExtension, implementation::SymbolIconExtension>
    {
    };
}
