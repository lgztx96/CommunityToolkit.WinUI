#pragma once

#include "MarkdownConfig.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct MarkdownConfig : MarkdownConfigT<MarkdownConfig>
    {
        MarkdownConfig() = default;

        static winrt::XamlToolkit::Labs::WinUI::MarkdownConfig Default();

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::MarkdownThemes> Themes = winrt::XamlToolkit::Labs::WinUI::MarkdownThemes::Default();

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::ISVGRenderer> SVGRenderer = nullptr;

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::IImageProvider> ImageProvider = nullptr;

        wil::single_threaded_rw_property<winrt::hstring> BaseUrl;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownConfig : MarkdownConfigT<MarkdownConfig, implementation::MarkdownConfig>
    {
    };
}
