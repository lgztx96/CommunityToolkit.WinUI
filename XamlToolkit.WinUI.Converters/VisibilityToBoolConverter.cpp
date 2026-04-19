// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "VisibilityToBoolConverter.h"
#if __has_include("VisibilityToBoolConverter.g.cpp")
#include "VisibilityToBoolConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable VisibilityToBoolConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        auto visibility = winrt::unbox_value_or<winrt::Visibility>(value, winrt::Visibility::Collapsed);
        return winrt::box_value(visibility == winrt::Visibility::Visible);
    }

    winrt::IInspectable VisibilityToBoolConverter::ConvertBack(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        auto bl = winrt::unbox_value_or<bool>(value, false);
        return winrt::box_value(bl ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
    }
}