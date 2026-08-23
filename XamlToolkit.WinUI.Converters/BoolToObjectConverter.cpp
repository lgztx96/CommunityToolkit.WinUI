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

	winrt::IInspectable BoolToObjectConverter::Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
	{
		bool boolValue = winrt::unbox_value_or(value, false);

		// Negate if needed
		if (ConverterTools::TryParseBool(parameter))
		{
			boolValue = !boolValue;
		}

		return ConverterTools::TryConvertValue(boolValue ? TrueValue() : FalseValue(), targetType);
	}

	winrt::IInspectable BoolToObjectConverter::ConvertBack(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) const
	{
		bool result = ConverterTools::AreValuesEqual(value, TrueValue());

		if (ConverterTools::TryParseBool(parameter))
		{
			result = !result;
		}

		return winrt::box_value(result);
	}
}
