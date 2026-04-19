// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "ResourceNameToResourceStringConverter.h"
#if __has_include("ResourceNameToResourceStringConverter.g.cpp")
#include "ResourceNameToResourceStringConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable ResourceNameToResourceStringConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
		auto stringValue = ConverterTools::TryConvertToString(value);
        if (!stringValue || stringValue->empty())
        {
            return winrt::box_value(L"");
        }

        auto result = _resourceManager.MainResourceMap().TryGetValue(*stringValue).ValueAsString();
        return winrt::box_value(result);
    }

    winrt::IInspectable ResourceNameToResourceStringConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }
}