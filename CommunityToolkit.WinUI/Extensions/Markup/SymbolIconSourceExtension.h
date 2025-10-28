#pragma once

#include "SymbolIconSourceExtension.g.h"
#include "TextIconExtension.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    struct SymbolIconSourceExtension : SymbolIconSourceExtensionT<SymbolIconSourceExtension, implementation::TextIconExtension>
    {
        SymbolIconSourceExtension() = default;

        wil::single_threaded_rw_property<winrt::Microsoft::UI::Xaml::Controls::Symbol> Symbol;

        IInspectable ProvideValue() const;

        IInspectable ProvideValue(IXamlServiceProvider const& provider);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct SymbolIconSourceExtension : SymbolIconSourceExtensionT<SymbolIconSourceExtension, implementation::SymbolIconSourceExtension>
    {
    };
}
