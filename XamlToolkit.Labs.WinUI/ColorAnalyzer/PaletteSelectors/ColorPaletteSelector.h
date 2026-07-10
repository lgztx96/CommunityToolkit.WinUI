#pragma once

#include "ColorPaletteSelector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ColorPaletteSelector : ColorPaletteSelectorT<ColorPaletteSelector>
    {
        ColorPaletteSelector() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> SelectedColorsProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> MinColorCountProperty;

        winrt::IVector<winrt::Color> SelectedColors();

    protected:
        void SelectedColors(winrt::IVector<winrt::Color> const& value);

    public:

        int32_t MinColorCount();
        void MinColorCount(int32_t value);

        virtual void SelectColors(winrt::IIterable<PaletteColor> const& palette);

    private:
        static void OnMinColorCountChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

    protected:
        winrt::IIterable<PaletteColor> _palette{ nullptr };

    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorPaletteSelector : ColorPaletteSelectorT<ColorPaletteSelector, implementation::ColorPaletteSelector>
    {
    };
}
