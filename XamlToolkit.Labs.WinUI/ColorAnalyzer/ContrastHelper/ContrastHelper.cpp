#include "pch.h"
#include "winrt_module_imports.h"
#include "ContrastHelper.h"
#if __has_include("ContrastHelper.g.cpp")
#include "ContrastHelper.g.cpp"
#endif
#include "../ColorExtensions.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void ContrastHelper::ApplyContrastCheck(DependencyObject const& d)
    {
        DependencyProperty dp{ nullptr };
        // Grab brush to update
        auto brush = FindBrush(d, &dp);
        if (brush == nullptr)
            return;

        // Retrieve colors to compare
        Color base = GetOriginalColor(d);
        Color opponent = GetOpponent(d);

        // Transparent is a sentinel value to say contrast ensurance should applied
        // regardless of contrast ratio
        if (base != winrt::Microsoft::UI::Colors::Transparent())
        {
            // Calculate the WCAG contrast ratio
            auto ratio = ColorExtensions::ContrastRatio(base, opponent);
            SetOriginalContrastRatio(d, ratio);

            // Use original color if the contrast is in the acceptable range
            if (ratio >= GetMinRatio(d))
            {
                UpdateContrastedProperties(d, base);
                return;
            }
        }

        // Current contrast is too small.
        // Select either black or white backed on the opponent luminance
        auto luminance = ColorExtensions::RelativeLuminance(opponent);
        auto contrastingColor = luminance < 0.5f ? Colors::White() : Colors::Black();
        UpdateContrastedProperties(d, contrastingColor);
    }

    SolidColorBrush ContrastHelper::FindBrush(DependencyObject const& d, DependencyProperty* dp)
    {
        if (auto brush = d.try_as<SolidColorBrush>())
        {
            *dp = SolidColorBrush::ColorProperty();
            return brush;
        }

        if (auto tb = d.try_as<TextBlock>())
        {
            *dp = TextBlock::ForegroundProperty();
            return tb.Foreground().try_as<SolidColorBrush>();
        }

        if (auto ctrl = d.try_as<Control>())
        {
            *dp = Control::ForegroundProperty();
            return ctrl.Foreground().try_as<SolidColorBrush>();
        }

        return nullptr;
    }

    void ContrastHelper::UpdateContrastedProperties(DependencyObject const& d, winrt::Windows::UI::Color color)
    {
        // Block the original color from updating
        _selfUpdate = true;

        if (auto b = d.try_as<SolidColorBrush>())
        {
            b.Color(color);
        }
        else if (auto t = d.try_as<TextBlock>())
        {
            t.Foreground(SolidColorBrush{ color });
        }
        else if (auto c = d.try_as<Control>())
        {
            c.Foreground(SolidColorBrush{ color });
        }

        // Calculate the actual ratio, between the opponent and the actual color
        auto opponent = GetOpponent(d);
        auto actualRatio = ColorExtensions::ContrastRatio(color, opponent);
        SetContrastRatio(d, actualRatio);

        // Unlock the original color updates
        _selfUpdate = false;
    }

}
