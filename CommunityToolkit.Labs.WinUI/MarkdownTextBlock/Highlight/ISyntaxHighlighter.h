#pragma once
#include <optional>
#include <string>

template<typename charT>
struct HighlightedToken {
    size_t start;
    size_t length;
    std::wstring_view token;
    std::string_view scope;
   
    std::wstring color;
};

using Utf16HighlightToken = HighlightedToken<wchar_t>;

template<typename charT>
class ISyntaxHighlighter {
public:
    virtual ~ISyntaxHighlighter() = default;

    virtual std::vector<HighlightedToken<charT>> Highlight(std::basic_string_view<charT> source, bool isDarkMode) const = 0;

    static std::unique_ptr<ISyntaxHighlighter> Create(std::basic_string_view<charT> language);
};

using IUtf16SyntaxHighlighter = ISyntaxHighlighter<wchar_t>;