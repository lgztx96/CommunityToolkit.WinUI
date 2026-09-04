#pragma once

#include "ColorHelper.g.h"

#ifdef __INTELLISENSE__
#include <string_view>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#else
import std;
import winrt.XamlToolkit.WinUI;
#endif

namespace winrt
{
    using namespace Windows::UI;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct ColorHelper
    {
        static winrt::Color ToColor(winrt::hstring const& colorString);

        static winrt::Color ToColor(std::string_view colorString);

        static winrt::hstring ToHex(winrt::Color const& color);

        static int ToInt(winrt::Color const& color) noexcept;

        static HslColor ToHsl(winrt::Color const& color) noexcept;

        static HsvColor ToHsv(winrt::Color const& color) noexcept;

        static winrt::Color FromHsl(double hue, double saturation, double lightness, double alpha = 1.0);

        static winrt::Color FromHsv(double hue, double saturation, double value, double alpha = 1.0);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct ColorHelper : ColorHelperT<ColorHelper, implementation::ColorHelper>
    {
    };
}
