#include "pch.h"
#include "ColorToDisplayNameConverter.h"
#if __has_include("ColorToDisplayNameConverter.g.cpp")
#include "ColorToDisplayNameConverter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Converters::implementation
{
	IInspectable ColorToDisplayNameConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		Color color;

		if (auto valueColor = value.try_as<Color>())
		{
			color = *valueColor;
		}
		else if (auto valueBrush = value.try_as<SolidColorBrush>())
		{
			color = valueBrush.Color();
		}
		else
		{
			// Invalid color value provided
			return DependencyProperty::UnsetValue();
		}

		return winrt::box_value(Microsoft::UI::ColorHelper::ToDisplayName(color));
	}

	IInspectable ColorToDisplayNameConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		return DependencyProperty::UnsetValue();
	}
}
