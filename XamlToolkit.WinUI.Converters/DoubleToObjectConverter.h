#pragma once

#include "DoubleToObjectConverter.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	/// <summary>
	/// This class converts a double value into an other object.
	/// Can be used to convert doubles to visibility, a couple of colors, couple of images, etc.
	/// If GreaterThan and LessThan are both set, the logic looks for a value between the two values.
	/// Otherwise the logic looks for the value to be GreaterThan or LessThan the specified value.
	/// The ConverterParameter can be used to invert the logic.
	/// </summary>
	struct DoubleToObjectConverter : DoubleToObjectConverterT<DoubleToObjectConverter>
	{
		DoubleToObjectConverter() = default;

		/// <summary>
		/// Gets or sets the value to be returned when the expression is true.
		/// </summary>
		winrt::IInspectable TrueValue() const;
		void TrueValue(winrt::IInspectable const& value);

		/// <summary>
		/// Gets or sets the value to be returned when the expression is false.
		/// </summary>
		winrt::IInspectable FalseValue() const;
		void FalseValue(winrt::IInspectable const& value);

		/// <summary>
		/// Gets or sets the value to be returned when the value passed is null.
		/// </summary>
		winrt::IInspectable NullValue() const;
		void NullValue(winrt::IInspectable const& value);

		/// <summary>
		/// Gets or sets the value to check if the value is GreaterThan this value.
		/// </summary>
		double GreaterThan() const;
		void GreaterThan(double value);

		/// <summary>
		/// Gets or sets the value to check if the value is LessThan this value.
		/// </summary>
		double LessThan() const;
		void LessThan(double value);

		static const wil::single_threaded_property<winrt::DependencyProperty> TrueValueProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> FalseValueProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> NullValueProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> GreaterThanProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> LessThanProperty;

		/// <summary>
		/// Convert a boolean value to an other object.
		/// </summary>
		/// <param name="value">The source data being passed to the target.</param>
		/// <param name="targetType">The type of the target property, as a type reference.</param>
		/// <param name="parameter">An optional parameter to be used to invert the converter logic.</param>
		/// <param name="language">The language of the conversion.</param>
		/// <returns>The value to be passed to the target dependency property.</returns>
		winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

		/// <summary>
		/// Convert back the value to a boolean.
		/// </summary>
		/// <param name="value">The target data being passed to the source.</param>
		/// <param name="targetType">The type of the target property.</param>
		/// <param name="parameter">An optional parameter to be used to invert the converter logic.</param>
		/// <param name="language">The language of the conversion.</param>
		/// <returns>The value to be passed to the source object.</returns>
		winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;
	};
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct DoubleToObjectConverter : DoubleToObjectConverterT<DoubleToObjectConverter, implementation::DoubleToObjectConverter>
	{

	};
}