#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorPaletteSampler.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::Windows::Foundation::Collections::IVectorView<PaletteColor> ColorPaletteSampler::Palette() const
    {
		return _palette ? _palette.GetView() : nullptr;
    }

    ColorSource ColorPaletteSampler::Source() const
    {
        return GetValue(SourceProperty).try_as<ColorSource>();
    }

    void ColorPaletteSampler::Source(ColorSource const& value)
    {
        SetValue(SourceProperty, value);
    }

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ColorPaletteSampler::SourceProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<ColorSource>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &ColorPaletteSampler::OnSourceChanged });

    void ColorPaletteSampler::OnSourceChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto analyzer = d.try_as<winrt::XamlToolkit::Labs::WinUI::ColorPaletteSampler>();
        if (!analyzer)
            return;

		auto analyzerImpl = get_self<ColorPaletteSampler>(analyzer);

        if (auto oldSource = e.OldValue().try_as<ColorSource>())
        {
			analyzerImpl->_sourceUpdatedRevoker.revoke();
        }

        if (auto newSource = e.NewValue().try_as<ColorSource>())
        {
            analyzerImpl->_sourceUpdatedRevoker =
                newSource.SourceUpdated(winrt::auto_revoke, { analyzerImpl, &ColorPaletteSampler::OnSourceUpdated });
        }

        analyzerImpl->UpdatePaletteAsync();
    }

    void ColorPaletteSampler::OnSourceUpdated([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IInspectable const& e)
    {
        UpdatePaletteAsync();
    }
}
