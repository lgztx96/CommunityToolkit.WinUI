#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorToDisplayNameConverter.h"
#if __has_include("ColorToDisplayNameConverter.g.cpp")
#include "ColorToDisplayNameConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	winrt::IInspectable ColorToDisplayNameConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		winrt::Color color;

		if (auto valueColor = value.try_as<winrt::Color>())
		{
			color = *valueColor;
		}
		else if (auto valueBrush = value.try_as<winrt::SolidColorBrush>())
		{
			color = valueBrush.Color();
		}
		else
		{
			// Invalid color value provided
			return winrt::DependencyProperty::UnsetValue();
		}

		return winrt::box_value(winrt::ColorHelper::ToDisplayName(color));
	}

	winrt::IInspectable ColorToDisplayNameConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		return winrt::DependencyProperty::UnsetValue();
	}
}
