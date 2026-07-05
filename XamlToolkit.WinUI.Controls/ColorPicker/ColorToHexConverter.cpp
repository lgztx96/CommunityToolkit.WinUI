#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorToHexConverter.h"
#if __has_include("ColorToHexConverter.g.cpp")
#include "ColorToHexConverter.g.cpp"
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace XamlToolkit::WinUI;
	using namespace XamlToolkit::WinUI::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::IInspectable ColorToHexConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        winrt::Color color;

        if (const auto valueColor = value.try_as<winrt::Color>())
        {
            color = *valueColor;
        }
        else if (const auto valueBrush = value.try_as<winrt::SolidColorBrush>())
        {
            color = valueBrush.Color();
        }
        else
        {
            // Invalid color value provided
            return winrt::DependencyProperty::UnsetValue();
        }

        const auto hexColor = winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHex(color);
        return winrt::box_value(winrt::hstring{ hexColor.data() + 1, hexColor.size() - 1 });
	}

	winrt::IInspectable ColorToHexConverter::ConvertBack(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        const auto hexValue = winrt::unbox_value<winrt::hstring>(value);

        if (hexValue.starts_with(L"#"))
        {
            try
            {
                return winrt::box_value(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(hexValue));
            }
            catch(...)
            {
                // Invalid hex color value provided
                return winrt::DependencyProperty::UnsetValue();
            }
        }
        else
        {
            try
            {
                return winrt::box_value(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(L"#" + hexValue));
            }
            catch(...)
            {
                // Invalid hex color value provided
                return winrt::DependencyProperty::UnsetValue();
            }
        }
	}
}
