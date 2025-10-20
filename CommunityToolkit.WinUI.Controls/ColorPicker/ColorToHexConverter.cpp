#include "pch.h"
#include "ColorToHexConverter.h"
#if __has_include("ColorToHexConverter.g.cpp")
#include "ColorToHexConverter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	IInspectable ColorToHexConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
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

        auto hexColor = Helpers::ColorHelper::ToHex(color);
        return winrt::box_value(winrt::hstring{ hexColor.data() + 1, hexColor.size() - 1 });
	}

	IInspectable ColorToHexConverter::ConvertBack(IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) {

        auto hexValue = winrt::unbox_value<winrt::hstring>(value);

        if (hexValue.starts_with(L"#"))
        {
            try
            {
                return winrt::box_value(Helpers::ColorHelper::ToColor(hexValue));
            }
            catch(...)
            {
                // Invalid hex color value provided
                return DependencyProperty::UnsetValue();
            }
        }
        else
        {
            try
            {
                return winrt::box_value(Helpers::ColorHelper::ToColor(L"#" + hexValue));
            }
            catch(...)
            {
                // Invalid hex color value provided
                return DependencyProperty::UnsetValue();
            }
        }
	}
}
