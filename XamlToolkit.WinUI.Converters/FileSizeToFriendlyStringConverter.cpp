// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "FileSizeToFriendlyStringConverter.h"
#if __has_include("FileSizeToFriendlyStringConverter.g.cpp")
#include "FileSizeToFriendlyStringConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable FileSizeToFriendlyStringConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        if (auto size = value.try_as<int64_t>())
        {
            return winrt::box_value(ToFileSizeString(size.value()));
        }

        return winrt::box_value(winrt::hstring(L""));
    }

    winrt::IInspectable FileSizeToFriendlyStringConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }

    winrt::hstring FileSizeToFriendlyStringConverter::ToFileSizeString(int64_t size)
    {
        static constexpr auto units = std::to_array<std::wstring_view>({ L"bytes", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB" });
        double value = static_cast<double>(size);

        size_t index = 0;

        while (value >= 1024.0 && index < units.size() - 1)
        {
            value /= 1024.0;
            ++index;
        }

        if (index == 0)
        {
            return winrt::format(L"{} {}", size, units[index]);
        }

        return winrt::format(L"{:.2f} {}", value, units[index]);
    }
}