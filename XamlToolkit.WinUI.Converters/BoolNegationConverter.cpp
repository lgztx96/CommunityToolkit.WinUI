// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "BoolNegationConverter.h"
#if __has_include("BoolNegationConverter.g.cpp")
#include "BoolNegationConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable BoolNegationConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        return winrt::box_value(!winrt::unbox_value_or<bool>(value, false));
    }

    winrt::IInspectable BoolNegationConverter::ConvertBack(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        return winrt::box_value(!winrt::unbox_value_or<bool>(value, false));
    }
}