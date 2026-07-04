#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <optional>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif
#include "AccentColorConverter.h"
#if __has_include("AccentColorConverter.g.cpp")
#include "AccentColorConverter.g.cpp"
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    winrt::HsvColor AccentColorConverter::GetAccent(winrt::HsvColor hsvColor, int accentStep)
    {
        if (accentStep != 0)
        {
            double colorValue = hsvColor.V;
            colorValue += accentStep * AccentColorConverter::ValueDelta;

            static auto round = [](double value, int decimals) -> double
            {
                double factor = std::pow(10.0, decimals);
                return std::round(value * factor) / factor;
            };

            colorValue = round(colorValue, 2);

            return winrt::HsvColor
            {
                .H = std::clamp(hsvColor.H, 0.0, 360.0),
                .S = std::clamp(hsvColor.S, 0.0, 1.0),
                .V = std::clamp(colorValue, 0.0, 1.0),
                .A = std::clamp(hsvColor.A, 0.0, 1.0),
            };
        }
        else
        {
            return hsvColor;
        }
    }

    winrt::IInspectable AccentColorConverter::Convert(
        winrt::IInspectable const& value, 
        [[maybe_unused]] winrt::TypeName targetType, 
        winrt::IInspectable const& parameter, 
        [[maybe_unused]] winrt::hstring const& language)
    {
        int accentStep;
        std::optional<winrt::Color> rgbColor;
        std::optional<winrt::HsvColor> hsvColor;

        // Get the current color in HSV
        if (const auto valueColor = value.try_as<winrt::Color>())
        {
            rgbColor = valueColor;
        }
        else if (const auto valueHsvColor = value.try_as<winrt::HsvColor>())
        {
            hsvColor = valueHsvColor;
        }
        else if (const auto valueBrush = value.try_as<winrt::SolidColorBrush>())
        {
            rgbColor = valueBrush.Color();
        }
        else
        {
            // Invalid color value provided
            return winrt::DependencyProperty::UnsetValue();
        }

        // Get the value component delta
        try
        {
            const auto stepStr = winrt::unbox_value<winrt::hstring>(parameter);
            accentStep = std::stoi({ stepStr.data(), stepStr.size() });
        }
        catch(...)
        {
            // Invalid parameter provided, unable to convert to integer
            return winrt::DependencyProperty::UnsetValue();
        }

        if (!hsvColor.has_value() && rgbColor.has_value())
        {
            hsvColor = winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHsv(rgbColor.value());
        }

        if (hsvColor.has_value())
        {
            const auto hsv = AccentColorConverter::GetAccent(hsvColor.value(), accentStep);
			const auto rgb = winrt::XamlToolkit::WinUI::Helpers::ColorHelper::FromHsv(hsv.H, hsv.S, hsv.V, hsv.A);
            return winrt::box_value(rgb);
        }
        else
        {
            return winrt::DependencyProperty::UnsetValue();
        }
    }

    winrt::IInspectable AccentColorConverter::ConvertBack(
        [[maybe_unused]] winrt::IInspectable const& value, 
        [[maybe_unused]] winrt::TypeName targetType, 
        [[maybe_unused]] winrt::IInspectable const& parameter, 
        [[maybe_unused]] winrt::hstring const& language)
    {
        return winrt::DependencyProperty::UnsetValue();
    }
}
