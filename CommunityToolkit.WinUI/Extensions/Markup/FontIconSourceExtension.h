#pragma once

#include "FontIconSourceExtension.g.h"
#include "TextIconExtension.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt::Microsoft::UI::Text;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct FontIconSourceExtension : FontIconSourceExtensionT<FontIconSourceExtension, implementation::TextIconExtension>
    {
        FontIconSourceExtension() = default;

        wil::single_threaded_rw_property<winrt::hstring> Glyph;

        wil::single_threaded_rw_property<FontFamily> FontFamily{ nullptr };

        IInspectable ProvideValue() const;

        IInspectable ProvideValue(IXamlServiceProvider const& provider);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct FontIconSourceExtension : FontIconSourceExtensionT<FontIconSourceExtension, implementation::FontIconSourceExtension>
    {
    };
}
