#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#include <optional>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif
#include "ContrastBrushConverter.h"
#if __has_include("ContrastBrushConverter.g.cpp")
#include "ContrastBrushConverter.g.cpp"
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::IInspectable ContrastBrushConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) 
    {
        winrt::Color comparisonColor;
        std::optional<winrt::Color> defaultColor;

        // Get the changing color to compare against
        if (auto valueColor = value.try_as<winrt::Color>())
        {
            comparisonColor = *valueColor;
        }
        else if (auto valueBrush = value.try_as<winrt::SolidColorBrush>())
        {
            comparisonColor = valueBrush.Color();
        }
        else
        {
            // Invalid color value provided
            return winrt::DependencyProperty::UnsetValue();
        }

        // Get the default color when transparency is high
        if (auto parameterColor = parameter.try_as<winrt::Color>())
        {
            defaultColor = parameterColor;
        }
        else if (auto parameterBrush = parameter.try_as<winrt::SolidColorBrush>())
        {
            defaultColor = parameterBrush.Color();
        }

        if (comparisonColor.A < AlphaThreshold && defaultColor.has_value())
        {
            // If the transparency is less than 50 %, just use the default brush
            // This can commonly be something like the TextControlForeground brush
            return winrt::SolidColorBrush(defaultColor.value());
        }
        else
        {
            // Chose a white/black brush based on contrast to the base color
            if (UseLightContrastColor(comparisonColor))
            {
                return winrt::SolidColorBrush(winrt::Microsoft::UI::Colors::White());
            }
            else
            {
                return winrt::SolidColorBrush(winrt::Microsoft::UI::Colors::Black());
            }
        }
	}

	winrt::IInspectable ContrastBrushConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        return winrt::DependencyProperty::UnsetValue();
	}

    bool ContrastBrushConverter::UseLightContrastColor(winrt::Color displayedColor) const
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
