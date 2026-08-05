#include "pch.h"
#include "winrt_module_imports.h"
#include "BoolToObjectConverter.h"
#if __has_include("BoolToObjectConverter.g.cpp")
#include "BoolToObjectConverter.g.cpp"
#endif
#include "ConverterTools.h"

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> BoolToObjectConverter::TrueValueProperty =
		winrt::DependencyProperty::Register(
			L"TrueValue",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> BoolToObjectConverter::FalseValueProperty =
		winrt::DependencyProperty::Register(
			L"FalseValue",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	winrt::IInspectable BoolToObjectConverter::TrueValue() const
	{
		return GetValue(TrueValueProperty());
	}

	void BoolToObjectConverter::TrueValue(winrt::IInspectable const& value)
	{
		SetValue(TrueValueProperty(), value);
	}

	winrt::IInspectable BoolToObjectConverter::FalseValue() const
	{
		return GetValue(FalseValueProperty());
	}

	void BoolToObjectConverter::FalseValue(winrt::IInspectable const& value)
	{
		SetValue(FalseValueProperty(), value);
	}

	winrt::IInspectable BoolToObjectConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
	{
		bool boolValue = winrt::unbox_value_or(value, false);

		// Negate if needed
		if (ConverterTools::TryParseBool(parameter))
		{
			boolValue = !boolValue;
		}

		return ConverterTools::Convert(boolValue ? TrueValue() : FalseValue(), targetType);
	}

	winrt::IInspectable BoolToObjectConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
	{
		winrt::TypeName valueType{ winrt::get_class_name(value) };
		auto converted = ConverterTools::Convert(TrueValue(), valueType);

		auto pv1 = value.try_as<winrt::IPropertyValue>();
		auto pv2 = converted.try_as<winrt::IPropertyValue>();

		bool result = (pv1 && pv2) ? ConverterTools::ValueEquals(pv1, pv2) : (value == converted);

		if (ConverterTools::TryParseBool(parameter))
		{
			result = !result;
		}

		return winrt::box_value(result);
	}
}
