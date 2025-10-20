#include "pch.h"
#include "ContrastBrushConverter.h"
#if __has_include("ContrastBrushConverter.g.cpp")
#include "ContrastBrushConverter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	IInspectable ContrastBrushConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName targetType, IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) {
        Color comparisonColor;
        std::optional<Color> defaultColor;

        // Get the changing color to compare against
        if (auto valueColor = value.try_as<Color>())
        {
            comparisonColor = *valueColor;
        }
        else if (auto valueBrush = value.try_as<SolidColorBrush>())
        {
            comparisonColor = valueBrush.Color();
        }
        else
        {
            // Invalid color value provided
            return DependencyProperty::UnsetValue();
        }

        // Get the default color when transparency is high
        if (auto parameterColor = parameter.try_as<Color>())
        {
            defaultColor = parameterColor;
        }
        else if (auto parameterBrush = parameter.try_as<SolidColorBrush>())
        {
            defaultColor = parameterBrush.Color();
        }

        if (comparisonColor.A < AlphaThreshold && defaultColor.has_value())
        {
            // If the transparency is less than 50 %, just use the default brush
            // This can commonly be something like the TextControlForeground brush
            return SolidColorBrush(defaultColor.value());
        }
        else
        {
            // Chose a white/black brush based on contrast to the base color
            if (UseLightContrastColor(comparisonColor))
            {
                return SolidColorBrush(Colors::White());
            }
            else
            {
                return SolidColorBrush(Colors::Black());
            }
        }
	}

	IInspectable ContrastBrushConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        return DependencyProperty::UnsetValue();
	}

    bool ContrastBrushConverter::UseLightContrastColor(Color displayedColor)
    {
        // The selection ellipse should be light if and only if the chosen color
        // contrasts more with black than it does with white.
        // To find how much something contrasts with white, we use the equation
        // for relative luminance, which is given by
        //
        // L = 0.2126 * Rg + 0.7152 * Gg + 0.0722 * Bg
        //
        // where Xg = { X/3294 if X <= 10, (R/269 + 0.0513)^2.4 otherwise }
        //
        // If L is closer to 1, then the color is closer to white; if it is closer to 0,
        // then the color is closer to black.  This is based on the fact that the human
        // eye perceives green to be much brighter than red, which in turn is perceived to be
        // brighter than blue.
        //
        // If the third dimension is value, then we won't be updating the spectrum's displayed colors,
        // so in that case we should use a value of 1 when considering the backdrop
        // for the selection ellipse.
        double rg = displayedColor.R <= 10 ? displayedColor.R / 3294.0 : std::pow((displayedColor.R / 269.0) + 0.0513, 2.4);
        double gg = displayedColor.G <= 10 ? displayedColor.G / 3294.0 : std::pow((displayedColor.G / 269.0) + 0.0513, 2.4);
        double bg = displayedColor.B <= 10 ? displayedColor.B / 3294.0 : std::pow((displayedColor.B / 269.0) + 0.0513, 2.4);

        return (0.2126 * rg) + (0.7152 * gg) + (0.0722 * bg) <= 0.5;
    }
}
