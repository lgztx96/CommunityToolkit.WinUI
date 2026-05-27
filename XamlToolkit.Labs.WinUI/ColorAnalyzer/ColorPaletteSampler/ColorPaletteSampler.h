#pragma once

#include "ColorPaletteSampler.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <span>
#include <vector>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ColorPaletteSampler : ColorPaletteSamplerT<ColorPaletteSampler>
    {
        ColorPaletteSampler();

        void UpdatePalette();

        IAsyncAction UpdatePaletteAsync();

#pragma region KMeans
        std::vector<float3> KMeansCluster(std::span<float3> points, int k, std::vector<int>& counts);

        void Split(int k, std::vector<int>& clusterIds);

        void CalculateCentroidsAndPrune(std::span<float3>& centroids, std::vector<int>& counts, std::span<float3> points, const std::vector<int>& clusterIds);

        int FindNearestClusterIndex(float3 point, std::span<float3> centroids);
#pragma endregion

#pragma region Properties
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SourceProperty;

        wil::untyped_event<IInspectable> PaletteUpdated;

        ColorSource Source() const;
        void Source(ColorSource const& value);

        wil::single_threaded_rw_property<winrt::Windows::Foundation::Collections::IVector<ColorPaletteSelector>> PaletteSelectors;

        winrt::Windows::Foundation::Collections::IVectorView<PaletteColor> Palette() const;

        void OnSourceUpdated(IInspectable const& sender, IInspectable const& e);
#pragma endregion

    private:
        IAsyncOperation<winrt::Windows::Foundation::Collections::IVector<float3>> SampleSourcePixelColorsAsync(int sampleCount);

        static void OnSourceChanged(
            winrt::Microsoft::UI::Xaml::DependencyObject const& d,
            winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

		winrt::Windows::Foundation::Collections::IVector<PaletteColor> _palette{ nullptr };
		ColorSource::SourceUpdated_revoker _sourceUpdatedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorPaletteSampler : ColorPaletteSamplerT<ColorPaletteSampler, implementation::ColorPaletteSampler>
    {
    };
}
