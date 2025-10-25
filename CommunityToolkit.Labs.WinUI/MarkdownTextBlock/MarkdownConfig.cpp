#include "pch.h"
#include "MarkdownConfig.h"
#if __has_include("MarkdownConfig.g.cpp")
#include "MarkdownConfig.g.cpp"
#endif
#include "LinkClickedEventArgs.h"

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    winrt::CommunityToolkit::Labs::WinUI::MarkdownConfig MarkdownConfig::Default()
    {
        static auto config = winrt::make<MarkdownConfig>();
        return config;
    }
}
