// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "EmptyObjectToObjectConverter.h"
#if __has_include("EmptyObjectToObjectConverter.g.cpp")
#include "EmptyObjectToObjectConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable EmptyObjectToObjectConverter::NotEmptyValue() const
    {
        return GetValue(NotEmptyValueProperty);
    }

    void EmptyObjectToObjectConverter::NotEmptyValue(winrt::IInspectable const& value)
    {
        SetValue(NotEmptyValueProperty, value);
    }

    winrt::IInspectable EmptyObjectToObjectConverter::EmptyValue() const
    {
        return GetValue(EmptyValueProperty);
    }

    void EmptyObjectToObjectConverter::EmptyValue(winrt::IInspectable const& value)
    {
        SetValue(EmptyValueProperty, value);
    }

    const wil::single_threaded_property<winrt::DependencyProperty> EmptyObjectToObjectConverter::NotEmptyValueProperty = winrt::DependencyProperty::Register(
        L"NotEmptyValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> EmptyObjectToObjectConverter::EmptyValueProperty = winrt::DependencyProperty::Register(
        L"EmptyValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    winrt::IInspectable EmptyObjectToObjectConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        auto isEmpty = CheckValueIsEmpty(value);

        // Negate if needed
        if (ConverterTools::TryParseBool(parameter))
        {
            isEmpty = !isEmpty;
        }

        return ConverterTools::TryConvertValue(isEmpty ? EmptyValue() : NotEmptyValue(), targetType);
    }

    winrt::IInspectable EmptyObjectToObjectConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }

    bool EmptyObjectToObjectConverter::CheckValueIsEmpty(winrt::IInspectable const& value) const
    {
        return !value;
    }
}