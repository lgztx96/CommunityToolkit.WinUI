#pragma once

#include "ColorPaletteSelector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ColorPaletteSelector : ColorPaletteSelectorT<ColorPaletteSelector>
    {
        ColorPaletteSelector() = default;

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SelectedColorsProperty;

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MinColorCountProperty;

        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color> SelectedColors();

    protected:
        void SelectedColors(winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color> const& value);

    public:

        int32_t MinColorCount();
        void MinColorCount(int32_t value);

        virtual void SelectColors(winrt::Windows::Foundation::Collections::IIterable<PaletteColor> const& palette);

    private:
        static void OnMinColorCountChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

    protected:
        winrt::Windows::Foundation::Collections::IIterable<PaletteColor> _palette{ nullptr };

    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorPaletteSelector : ColorPaletteSelectorT<ColorPaletteSelector, implementation::ColorPaletteSelector>
    {
    };
}
