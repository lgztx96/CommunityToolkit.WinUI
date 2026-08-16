// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "TypeToObjectConverter.h"
#if __has_include("TypeToObjectConverter.g.cpp")
#include "TypeToObjectConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable TypeToObjectConverter::TrueValue() const
    {
        return GetValue(TrueValueProperty);
    }

    void TypeToObjectConverter::TrueValue(winrt::IInspectable const& value)
    {
        SetValue(TrueValueProperty, value);
    }

    winrt::IInspectable TypeToObjectConverter::FalseValue() const
    {
        return GetValue(FalseValueProperty);
    }

    void TypeToObjectConverter::FalseValue(winrt::IInspectable const& value)
    {
        SetValue(FalseValueProperty, value);
    }

    winrt::TypeName TypeToObjectConverter::Type() const
    {
        return winrt::unbox_value<winrt::TypeName>(GetValue(TypeProperty));
    }

    void TypeToObjectConverter::Type(winrt::TypeName const& value)
    {
        SetValue(TypeProperty, winrt::box_value(value));
    }

    const wil::single_threaded_property<winrt::DependencyProperty> TypeToObjectConverter::TrueValueProperty = winrt::DependencyProperty::Register(
        L"TrueValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> TypeToObjectConverter::FalseValueProperty = winrt::DependencyProperty::Register(
        L"FalseValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> TypeToObjectConverter::TypeProperty = winrt::DependencyProperty::Register(
        L"Type",
        winrt::xaml_typename<winrt::TypeName>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ winrt::box_value(winrt::TypeName{ L"Object" }) }
    );

    winrt::IInspectable TypeToObjectConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        auto typeMatches = value && Type() == winrt::TypeName{ winrt::get_class_name(value) };

        // Negate if needed
        if (ConverterTools::TryParseBool(parameter))
        {
            typeMatches = !typeMatches;
        }

        return ConverterTools::TryConvertValue(typeMatches ? TrueValue() : FalseValue(), targetType);
    }

    winrt::IInspectable TypeToObjectConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }
}