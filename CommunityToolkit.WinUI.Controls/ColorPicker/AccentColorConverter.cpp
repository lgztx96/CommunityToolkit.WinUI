#include "pch.h"
#include "AccentColorConverter.h"
#if __has_include("AccentColorConverter.g.cpp")
#include "AccentColorConverter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    HsvColor AccentColorConverter::GetAccent(HsvColor hsvColor, int accentStep)
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

            return HsvColor
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

    IInspectable AccentColorConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName targetType, IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        int accentStep;
        std::optional<Color> rgbColor;
        std::optional<HsvColor> hsvColor;

        // Get the current color in HSV
        if (auto valueColor = value.try_as<Color>())
        {
            rgbColor = valueColor;
        }
        else if (auto valueHsvColor = value.try_as<HsvColor>())
        {
            hsvColor = valueHsvColor;
        }
        else if (auto valueBrush = value.try_as<SolidColorBrush>())
        {
            rgbColor = valueBrush.Color();
        }
        else
        {
            // Invalid color value provided
            return DependencyProperty::UnsetValue();
        }

        // Get the value component delta
        try
        {
            accentStep = std::stoi((winrt::unbox_value<winrt::hstring>(parameter)).data());
        }
        catch(...)
        {
            // Invalid parameter provided, unable to convert to integer
            return DependencyProperty::UnsetValue();
        }

        if (hsvColor == std::nullopt &&
            rgbColor != std::nullopt)
        {
            hsvColor = Helpers::ColorHelper::ToHsv(rgbColor.value());
        }

        if (hsvColor != std::nullopt)
        {
            auto hsv = AccentColorConverter::GetAccent(hsvColor.value(), accentStep);

            return winrt::box_value(Helpers::ColorHelper::FromHsv(hsv.H, hsv.S, hsv.V, hsv.A));
        }
        else
        {
            return DependencyProperty::UnsetValue();
        }
    }

    IInspectable AccentColorConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        return DependencyProperty::UnsetValue();
    }
}
