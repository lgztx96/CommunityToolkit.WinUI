#pragma once

#include "ColorHelper.g.h"
#include <winrt/Windows.UI.h>
#undef max
#undef min

using namespace winrt::Windows::UI;

namespace winrt::CommunityToolkit::WinUI::Helpers::implementation
{
    struct ColorHelper : ColorHelperT<ColorHelper>
    {
        static Color ToColor(winrt::hstring const& colorString);

        static Color ToColor(std::string_view colorString);

        static winrt::hstring ToHex(Color const& color);

        static int ToInt(Color const& color);

        static HslColor ToHsl(Color const& color);

        static HsvColor ToHsv(Color const& color);

        static Color FromHsl(double hue, double saturation, double lightness, double alpha = 1.0);

        static Color FromHsv(double hue, double saturation, double value, double alpha = 1.0);
    };
}

namespace winrt::CommunityToolkit::WinUI::Helpers::factory_implementation
{
    struct ColorHelper : ColorHelperT<ColorHelper, implementation::ColorHelper>
    {
    };
}
