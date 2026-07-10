#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "ColorExtensions.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::Color ColorExtensions::ToColor(winrt::float3 color)
    {
        color *= 255;
        return winrt::Microsoft::UI::ColorHelper::FromArgb(255, 
            static_cast<uint8_t>(color.x),
            static_cast<uint8_t>(color.y),
            static_cast<uint8_t>(color.z));
    }

    winrt::float3 ColorExtensions::ToVector3(winrt::Color color)
    {
        auto vector = winrt::float3(color.R, color.G, color.B);
        return vector / 255;
    }

    /// <summary>
    /// Get WCAG contrast ratio between two colors.
    /// </summary>
    double ColorExtensions::ContrastRatio(winrt::Color color1, winrt::Color color2)
    {
        // Using the formula for contrast ratio
        // Source WCAG guidelines: https://www.w3.org/TR/WCAG20/#contrast-ratiodef

        // Calculate perceived luminance for both colors
        double luminance1 = RelativeLuminance(color1);
        double luminance2 = RelativeLuminance(color2);

        // Determine lighter and darker luminance
        double lighter = std::max(luminance1, luminance2);
        double darker = std::min(luminance1, luminance2);

        // Calculate contrast ratio
        return (lighter + 0.05) / (darker + 0.05);
    }

    double ColorExtensions::RelativeLuminance(winrt::Color color)
    {
        // Color theory is a massive iceberg. Here's a peek at the tippy top:

        // There's two (main) standards for calculating percieved luminance from RGB values.

        // + ------------- + ------------------------------------ + ------------------ + ------------------------------------------------------------------------------- +
        // | Standard      | Formula                              | Ref. Section       | Ref. Link                                                                       |
        // + ------------- + ------------------------------------ + ------------------ + ------------------------------------------------------------------------------- +
        // | ITU Rec. 709  | Y = 0.2126 R + 0.7152 G + 0.0722 B   | Page 4/Item 3.2    | https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.709-6-201506-I!!PDF-E.pdf  |
        // + ------------- + ------------------------------------ + ------------------ + ------------------------------------------------------------------------------- +
        // | ITU Rec. 601  | Y = 0.299 R + 0.587 G + 0.114 B      | Page 2/Item 2.5.1  | https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.601-7-201103-I!!PDF-E.pdf  |
        // + ------------- + ------------------------------------ + ------------------ + ------------------------------------------------------------------------------- +

        // They're based on the standard ability of the human eye to perceive brightness,
        // from different colors, as well as the average monitor's ability to produce them.
        // Both standards produce similar results, but Rec. 709 is more accurate for modern displays.

        // NOTE: If we for whatever reason we ever need to optimize code,
        // we can make approximations using integer math instead of floating point math.
        // The precise values are not critical, as long as the proportions are similar and sum to 1.
        // Like so: return (2 * color.R + 7 * color.G + color.B);

        // Adjust channels relative luminance out of sRGB:
        // https://www.w3.org/WAI/GL/wiki/Relative_luminance#Definition_as_Stated_in_WCAG_2.x
        auto sRGBtoRGB = [](float s) -> float
        {
            if (s <= 0.03928f)
            {
                return s / 12.92f;
            }
            return std::pow(((s + 0.055f) / 1.055f), 2.4f);
        };

        auto vec = ToVector3(color);
        auto r = sRGBtoRGB(vec.x);
        auto g = sRGBtoRGB(vec.y);
        auto b = sRGBtoRGB(vec.z);

        // TLDR: We use ITU Rec. 709 standard formula for perceived luminance.
        return (0.2126f * r + 0.7152f * g + 0.0722 * b);
    }

    float ColorExtensions::FindColorfulness(winrt::Color color)
    {
        auto vectorColor = ToVector3(color);
        auto rg = vectorColor.x - vectorColor.y;
        auto yb = ((vectorColor.x + vectorColor.y) / 2) - vectorColor.z;
        return 0.3f * length(winrt::float2(rg, yb));
    }

    float ColorExtensions::FindColorfulness(std::span<winrt::Color> colors)
    {
        auto vectorColors = colors | std::views::transform(ToVector3);

        // Isolate rg and yb
        auto rg = vectorColors | std::views::transform([](const winrt::float3& x)
        {
            return std::abs(x.x - x.y);
        });

        auto yb = vectorColors | std::views::transform([](const winrt::float3& x)
        {
            return std::abs(0.5f * (x.x + x.y) - x.z);
        });

        // Evaluate rg and yb mean and std
        float rg_mean;
        float yb_mean;
        auto rg_std = FindStandardDeviation(rg, rg_mean);
        auto yb_std = FindStandardDeviation(yb, yb_mean);

        // Combine means and standard deviations
        auto std = length(winrt::float2(rg_mean, yb_mean));
        auto mean = length(winrt::float2(rg_std, yb_std));

        // Return colorfulness
        return std + (0.3f * mean);
    }
}
