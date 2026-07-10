#include "pch.h"
#include "winrt_module_imports.h"
#include "ContrastHelper.h"
#if __has_include("ContrastHelper.g.cpp")
#include "ContrastHelper.g.cpp"
#endif
#include "../ColorExtensions.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void ContrastHelper::ApplyContrastCheck(winrt::DependencyObject const& d)
    {
        winrt::DependencyProperty dp{ nullptr };
        // Grab brush to update
        auto brush = FindBrush(d, &dp);
        if (brush == nullptr)
            return;

        // Retrieve colors to compare
        winrt::Color base = GetOriginalColor(d);
        winrt::Color opponent = GetOpponent(d);

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

    winrt::SolidColorBrush ContrastHelper::FindBrush(winrt::DependencyObject const& d, winrt::DependencyProperty* dp)
    {
        if (auto brush = d.try_as<winrt::SolidColorBrush>())
        {
            *dp = winrt::SolidColorBrush::ColorProperty();
            return brush;
        }

        if (auto tb = d.try_as<winrt::TextBlock>())
        {
            *dp = winrt::TextBlock::ForegroundProperty();
            return tb.Foreground().try_as<winrt::SolidColorBrush>();
        }

        if (auto ctrl = d.try_as<winrt::Control>())
        {
            *dp = winrt::Control::ForegroundProperty();
            return ctrl.Foreground().try_as<winrt::SolidColorBrush>();
        }

        return nullptr;
    }

    void ContrastHelper::UpdateContrastedProperties(winrt::DependencyObject const& d, winrt::Windows::UI::Color color)
    {
        // Block the original color from updating
        _selfUpdate = true;

        if (auto b = d.try_as<winrt::SolidColorBrush>())
        {
            b.Color(color);
        }
        else if (auto t = d.try_as<winrt::TextBlock>())
        {
            t.Foreground(winrt::SolidColorBrush{ color });
        }
        else if (auto c = d.try_as<winrt::Control>())
        {
            c.Foreground(winrt::SolidColorBrush{ color });
        }

        // Calculate the actual ratio, between the opponent and the actual color
        auto opponent = GetOpponent(d);
        auto actualRatio = ColorExtensions::ContrastRatio(color, opponent);
        SetContrastRatio(d, actualRatio);

        // Unlock the original color updates
        _selfUpdate = false;
    }

}
