#include "pch.h"
#include "winrt_module_imports.h"
#include "MarkdownConfig.h"
#if __has_include("MarkdownConfig.g.cpp")
#include "MarkdownConfig.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::XamlToolkit::Labs::WinUI::MarkdownConfig MarkdownConfig::Default()
    {
        static auto config = winrt::make<MarkdownConfig>();
        return config;
    }
}
