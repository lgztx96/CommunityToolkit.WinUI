#pragma once

#include "DoubleToVisibilityConverter.g.h"
#include "DoubleToObjectConverter.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	/// <summary>
	/// This class converts a double value into a Visibility enumeration.
	/// </summary>
	struct DoubleToVisibilityConverter : DoubleToVisibilityConverterT<DoubleToVisibilityConverter, DoubleToObjectConverter>
	{
		DoubleToVisibilityConverter()
		{
			TrueValue(winrt::box_value(Visibility::Visible));
			FalseValue(winrt::box_value(Visibility::Collapsed));
			NullValue(winrt::box_value(Visibility::Collapsed));
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct DoubleToVisibilityConverter : DoubleToVisibilityConverterT<DoubleToVisibilityConverter, implementation::DoubleToVisibilityConverter>
	{

	};
}