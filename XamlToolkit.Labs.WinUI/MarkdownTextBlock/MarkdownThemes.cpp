#include "pch.h"
#include "winrt_module_imports.h"
#include "MarkdownThemes.h"
#if __has_include("MarkdownThemes.g.cpp")
#include "MarkdownThemes.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    XamlToolkit::Labs::WinUI::MarkdownThemes MarkdownThemes::Default()
    {
        static auto themes = winrt::make<MarkdownThemes>();
        return themes;
    }
}
