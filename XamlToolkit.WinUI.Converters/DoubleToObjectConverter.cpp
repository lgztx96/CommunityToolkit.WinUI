// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "DoubleToObjectConverter.h"
#if __has_include("DoubleToObjectConverter.g.cpp")
#include "DoubleToObjectConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable DoubleToObjectConverter::TrueValue() const
    {
        return GetValue(TrueValueProperty);
    }

    void DoubleToObjectConverter::TrueValue(winrt::IInspectable const& value)
    {
        SetValue(TrueValueProperty, value);
    }

    winrt::IInspectable DoubleToObjectConverter::FalseValue() const
    {
        return GetValue(FalseValueProperty);
    }

    void DoubleToObjectConverter::FalseValue(winrt::IInspectable const& value)
    {
        SetValue(FalseValueProperty, value);
    }

    winrt::IInspectable DoubleToObjectConverter::NullValue() const
    {
        return GetValue(NullValueProperty);
    }

    void DoubleToObjectConverter::NullValue(winrt::IInspectable const& value)
    {
        SetValue(NullValueProperty, value);
    }

    double DoubleToObjectConverter::GreaterThan() const
    {
        return winrt::unbox_value<double>(GetValue(GreaterThanProperty));
    }

    void DoubleToObjectConverter::GreaterThan(double value)
    {
        SetValue(GreaterThanProperty, winrt::box_value(value));
    }

    double DoubleToObjectConverter::LessThan() const
    {
        return winrt::unbox_value<double>(GetValue(LessThanProperty));
    }

    void DoubleToObjectConverter::LessThan(double value)
    {
        SetValue(LessThanProperty, winrt::box_value(value));
    }

    const wil::single_threaded_property<winrt::DependencyProperty> DoubleToObjectConverter::TrueValueProperty = winrt::DependencyProperty::Register(
        L"TrueValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> DoubleToObjectConverter::FalseValueProperty = winrt::DependencyProperty::Register(
        L"FalseValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> DoubleToObjectConverter::NullValueProperty = winrt::DependencyProperty::Register(
        L"NullValue",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ nullptr }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> DoubleToObjectConverter::GreaterThanProperty = winrt::DependencyProperty::Register(
        L"GreaterThan",
        winrt::xaml_typename<double>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()) }
    );

    const wil::single_threaded_property<winrt::DependencyProperty> DoubleToObjectConverter::LessThanProperty = winrt::DependencyProperty::Register(
        L"LessThan",
        winrt::xaml_typename<double>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()) }
    );

    winrt::IInspectable DoubleToObjectConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        if (!value)
        {
            return NullValue();
        }

        double vd = 0.0; // DEFAULT?
        auto doubleValue = value.try_as<double>();
        if (doubleValue)
        {
            vd = doubleValue.value();
        }
        else
        {
            try
            {
                if (auto strValue = ConverterTools::TryConvertToString(value))
                {
                    vd = std::stod(strValue->data());
                }
            }
            catch (...)
            {
                vd = 0.0;
            }
        }

        auto greaterThan = GreaterThan();
        auto lessThan = LessThan();
        bool boolValue = false;

        if (!std::isnan(greaterThan) && !std::isnan(lessThan))
        {
            if (vd > greaterThan && vd < lessThan)
            {
                boolValue = true;
            }
        }
        else if (!std::isnan(greaterThan) && vd > greaterThan)
        {
            boolValue = true;
        }
        else if (!std::isnan(lessThan) && vd < lessThan)
        {
            boolValue = true;
        }

        // Negate if needed
        if (ConverterTools::TryParseBool(parameter))
        {
            boolValue = !boolValue;
        }

        return ConverterTools::Convert(boolValue ? TrueValue() : FalseValue(), targetType);
    }

    winrt::IInspectable DoubleToObjectConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
    {
        throw hresult_not_implemented();
    }
}