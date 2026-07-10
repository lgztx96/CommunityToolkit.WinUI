#pragma once

#include "FontIconSourceExtension.g.h"
#include "TextIconExtension.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Text.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Text;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct FontIconSourceExtension : FontIconSourceExtensionT<FontIconSourceExtension, implementation::TextIconExtension>
    {
        FontIconSourceExtension() = default;

        wil::single_threaded_rw_property<winrt::hstring> Glyph;

        wil::single_threaded_rw_property<winrt::FontFamily> FontFamily{ nullptr };

        winrt::IInspectable ProvideValue() const;

        winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct FontIconSourceExtension : FontIconSourceExtensionT<FontIconSourceExtension, implementation::FontIconSourceExtension>
    {
    };
}
