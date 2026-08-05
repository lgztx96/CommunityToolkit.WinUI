#pragma once

#include "AccentColorPaletteSelector.g.h"
#include "ColorPaletteSelector.h"
#include "../ColorExtensions.h"
#include "ColorPaletteSelectorExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <algorithm>
#include <vector>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct AccentColorPaletteSelector : AccentColorPaletteSelectorT<AccentColorPaletteSelector, implementation::ColorPaletteSelector>
    {
        AccentColorPaletteSelector() = default;

        void SelectColors(winrt::Windows::Foundation::Collections::IIterable<PaletteColor> const& palette) override
        {
            // Select accent colors
            std::vector<winrt::Windows::UI::Color> colors;
            for (const auto& item : palette)
            {
                colors.push_back(item.Color);
            }

            std::sort(colors.begin(), colors.end(), [](const auto& a, const auto& b)
            {
                return ColorExtensions::FindColorfulness(a) > ColorExtensions::FindColorfulness(b);
            });

            auto result = single_threaded_vector<winrt::Windows::UI::Color>(std::move(colors));

            ColorPaletteSelectorExtensions::EnsureMinColorCount(result, MinColorCount());

            SelectedColors(result);
        }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct AccentColorPaletteSelector : AccentColorPaletteSelectorT<AccentColorPaletteSelector, implementation::AccentColorPaletteSelector>
    {
    };
}
