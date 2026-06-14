#pragma once

#ifdef __INTELLISENSE__
#include <string>
#include <string_view>
#include <winrt/Windows.Foundation.h>
#endif

import std;
import winrt.Windows.Foundation;

namespace winrt::XamlToolkit::WinUI::Media
{
    struct UriExtensions
    {
        static winrt::Windows::Foundation::Uri ToAppxUri(winrt::Windows::Foundation::Uri const& uri)
        {
            if (uri.SchemeName() == L"ms-resource")
            {
                winrt::hstring path_str(uri.Path());
                std::wstring_view path{ path_str };
                if (path.starts_with(L"/Files"))
                {
                    path.remove_prefix(std::size(L"/Files"));
                }

                return winrt::Windows::Foundation::Uri(winrt::format(L"ms-appx://{}", path));
            }

            return uri;
        }

        static winrt::Windows::Foundation::Uri ToAppxUri(std::wstring_view path)
        {
            std::wstring full = L"ms-appx://";
            if (!path.starts_with(L'/'))
            {
                full += L"/";
            }

            full.append(path);

            return winrt::Windows::Foundation::Uri(full);
        }
    };
}