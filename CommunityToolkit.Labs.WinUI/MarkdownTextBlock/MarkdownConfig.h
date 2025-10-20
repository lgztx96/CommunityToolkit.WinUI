#pragma once

#include "MarkdownConfig.g.h"

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    struct MarkdownConfig : MarkdownConfigT<MarkdownConfig>
    {
        MarkdownConfig() = default;

        static winrt::CommunityToolkit::Labs::WinUI::MarkdownConfig Default();

        wil::single_threaded_rw_property<winrt::CommunityToolkit::Labs::WinUI::MarkdownThemes> Themes = winrt::CommunityToolkit::Labs::WinUI::MarkdownThemes::Default();

        wil::single_threaded_rw_property<winrt::CommunityToolkit::Labs::WinUI::ISVGRenderer> SVGRenderer = nullptr;

        wil::single_threaded_rw_property<winrt::CommunityToolkit::Labs::WinUI::IImageProvider> ImageProvider = nullptr;

        wil::single_threaded_rw_property<winrt::hstring> BaseUrl;
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownConfig : MarkdownConfigT<MarkdownConfig, implementation::MarkdownConfig>
    {
    };
}
