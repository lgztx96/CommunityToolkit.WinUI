// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "EmptyStringToObjectConverter.h"
#if __has_include("EmptyStringToObjectConverter.g.cpp")
#include "EmptyStringToObjectConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    bool EmptyStringToObjectConverter::CheckValueIsEmpty(winrt::IInspectable const& value) const
    {
        if (!value)
        {
            return true;
        }

        auto strValue = value.try_as<winrt::hstring>();
        return !strValue || strValue->empty();
    }
}