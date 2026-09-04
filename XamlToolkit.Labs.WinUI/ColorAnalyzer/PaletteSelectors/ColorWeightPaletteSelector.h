#pragma once

#include "ColorWeightPaletteSelector.g.h"
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
    struct ColorWeightPaletteSelector : ColorWeightPaletteSelectorT<ColorWeightPaletteSelector, implementation::ColorPaletteSelector>
    {
        ColorWeightPaletteSelector() = default;

        void SelectColors(winrt::Windows::Foundation::Collections::IIterable<PaletteColor> const& palette) override
        {
            // Order by weight and ensure we have at least MinColorCount colors
            std::vector<PaletteColor> temp(palette.begin(), palette.end());

            std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b)
            {
                return a.Weight > b.Weight;
            });

            std::vector<winrt::Windows::UI::Color> colors;
            for (const auto& item : temp)
            {
                colors.push_back(item.Color);
            }

            auto result = single_threaded_vector<winrt::Windows::UI::Color>(std::move(colors));

            ColorPaletteSelectorExtensions::EnsureMinColorCount(result, MinColorCount());

            SelectedColors(result);
        }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorWeightPaletteSelector : ColorWeightPaletteSelectorT<ColorWeightPaletteSelector, implementation::ColorWeightPaletteSelector>
    {
    };
}
