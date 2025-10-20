#include "pch.h"
#include "MarkdownThemes.h"
#if __has_include("MarkdownThemes.g.cpp")
#include "MarkdownThemes.g.cpp"
#endif

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    CommunityToolkit::Labs::WinUI::MarkdownThemes MarkdownThemes::Default()
    {
        static auto themes = winrt::make<MarkdownThemes>();
        return themes;
    }
}
