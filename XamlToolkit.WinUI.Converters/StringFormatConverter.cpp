// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <format>
#endif
#include "StringFormatConverter.h"
#if __has_include("StringFormatConverter.g.cpp")
#include "StringFormatConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable StringFormatConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        if (!value)
        {
            return nullptr;
        }

        // Retrieve the format string and use it to format the value.
        const auto formatString = parameter.try_as<winrt::hstring>();

        if (!formatString || formatString->empty())
        {
            // If the format string is null or empty, simply call ToString()
            // on the value.
            return value;
        }

        const auto formatValue = ConverterTools::TryConvertToFormatValue(value);

        try
        {
            return std::visit([&](auto const& formatValue) -> winrt::IInspectable
            {
                using T = std::decay_t<decltype(formatValue)>;

                if constexpr (std::is_same_v<T, std::monostate>)
                {
                    return value;
                }
                else
                {
                    std::wstring_view formatView(*formatString);
                    auto formatted = std::vformat(formatView, std::make_wformat_args(formatValue));
                    return winrt::box_value(winrt::hstring(formatted));
                }
            }, formatValue);
        }
        catch (...)
        {
            return value;
        }
    }

    winrt::IInspectable StringFormatConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }
}