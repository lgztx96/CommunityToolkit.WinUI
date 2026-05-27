#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorPaletteSelector.h"
#if __has_include("ColorPaletteSelector.g.cpp")
#include "ColorPaletteSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ColorPaletteSelector::SelectedColorsProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"SelectedColors",
            winrt::xaml_typename<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color>>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ColorPaletteSelector::MinColorCountProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"MinColorCount",
            winrt::xaml_typename<int32_t>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(1), &ColorPaletteSelector::OnMinColorCountChanged });

    winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color> ColorPaletteSelector::SelectedColors()
    {
        return GetValue(SelectedColorsProperty).try_as<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color>>();
    }

    void ColorPaletteSelector::SelectedColors(winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Color> const& value)
    {
        SetValue(SelectedColorsProperty, value);
    }

    int32_t ColorPaletteSelector::MinColorCount()
    {
        return winrt::unbox_value<int32_t>(GetValue(MinColorCountProperty));
    }

    void ColorPaletteSelector::MinColorCount(int32_t value)
    {
        SetValue(MinColorCountProperty, winrt::box_value(value));
    }

    void ColorPaletteSelector::SelectColors(winrt::Windows::Foundation::Collections::IIterable<PaletteColor> const& palette)
    {
        _palette = palette;
    }

    void ColorPaletteSelector::OnMinColorCountChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const&)
    {
        auto selector = d.try_as<ColorPaletteSelector>();
        if (!selector || !selector->_palette)
        {
            return;
        }

        selector->SelectColors(selector->_palette);
    }
}
