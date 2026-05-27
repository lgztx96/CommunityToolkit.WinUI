#pragma once

#include "BaseColorPaletteSelector.g.h"
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
    struct BaseColorPaletteSelector : BaseColorPaletteSelectorT<BaseColorPaletteSelector, implementation::ColorPaletteSelector>
    {
        BaseColorPaletteSelector() = default;

        void SelectColors(winrt::Windows::Foundation::Collections::IIterable<PaletteColor> const& palette) override
        {
            // Get base color
            std::vector<winrt::Windows::UI::Color> colors;
            for (auto const& item : palette)
            {
                colors.push_back(item.Color);
            }

            std::sort(colors.begin(), colors.end(),
                [](auto const& a, auto const& b)
                {
                    return ColorExtensions::FindColorfulness(a)
                        < ColorExtensions::FindColorfulness(b);
                });

            auto result = single_threaded_vector<Color>(std::move(colors));

            ColorPaletteSelectorExtensions::EnsureMinColorCount(result, MinColorCount());

            SelectedColors(result);
        }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct BaseColorPaletteSelector : BaseColorPaletteSelectorT<BaseColorPaletteSelector, implementation::BaseColorPaletteSelector>
    {
    };
}
