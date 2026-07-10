#pragma once

#include "SymbolIconExtension.g.h"
#include "TextIconExtension.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct SymbolIconExtension : SymbolIconExtensionT<SymbolIconExtension, implementation::TextIconExtension>
    {
        SymbolIconExtension() = default;

        wil::single_threaded_rw_property<winrt::Symbol> Symbol;

        winrt::IInspectable ProvideValue() const;

        winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct SymbolIconExtension : SymbolIconExtensionT<SymbolIconExtension, implementation::SymbolIconExtension>
    {
    };
}
