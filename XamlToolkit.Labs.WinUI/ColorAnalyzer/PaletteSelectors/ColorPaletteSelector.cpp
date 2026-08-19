#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorPaletteSelector.h"
#if __has_include("ColorPaletteSelector.g.cpp")
#include "ColorPaletteSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ColorPaletteSelector::SelectedColorsProperty =
        winrt::DependencyProperty::Register(
            L"SelectedColors",
            winrt::xaml_typename<winrt::IVector<winrt::Color>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> ColorPaletteSelector::MinColorCountProperty =
        winrt::DependencyProperty::Register(
            L"MinColorCount",
            winrt::xaml_typename<int32_t>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(1), &ColorPaletteSelector::OnMinColorCountChanged });

    winrt::IVector<winrt::Color> ColorPaletteSelector::SelectedColors()
    {
        return GetValue(SelectedColorsProperty()).try_as<winrt::IVector<winrt::Color>>();
    }

    void ColorPaletteSelector::SelectedColors(winrt::IVector<winrt::Color> const& value)
    {
        SetValue(SelectedColorsProperty(), value);
    }

    int32_t ColorPaletteSelector::MinColorCount()
    {
        return winrt::unbox_value<int32_t>(GetValue(MinColorCountProperty()));
    }

    void ColorPaletteSelector::MinColorCount(int32_t value)
    {
        SetValue(MinColorCountProperty(), winrt::box_value(value));
    }

    void ColorPaletteSelector::SelectColors(winrt::IIterable<PaletteColor> const& palette)
    {
        _palette = palette;
    }

    void ColorPaletteSelector::OnMinColorCountChanged(
        winrt::DependencyObject const& d,
        winrt::DependencyPropertyChangedEventArgs const&)
    {
        auto selector = d.try_as<ColorPaletteSelector>();
        if (!selector || !selector->_palette)
        {
            return;
        }

        selector->SelectColors(selector->_palette);
    }
}
