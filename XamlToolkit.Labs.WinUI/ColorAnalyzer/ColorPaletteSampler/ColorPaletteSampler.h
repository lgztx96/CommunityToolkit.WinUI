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
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ColorPaletteSampler : ColorPaletteSamplerT<ColorPaletteSampler>
    {
        ColorPaletteSampler();

        void UpdatePalette();

        winrt::IAsyncAction UpdatePaletteAsync();

#pragma region KMeans
        std::vector<winrt::float3> KMeansCluster(std::span<winrt::float3> points, int k, std::vector<int>& counts);

        void Split(int k, std::vector<int>& clusterIds);

        void CalculateCentroidsAndPrune(std::span<winrt::float3>& centroids, std::vector<int>& counts, std::span<winrt::float3> points, const std::vector<int>& clusterIds);

        int FindNearestClusterIndex(winrt::float3 point, std::span<winrt::float3> centroids);
#pragma endregion

#pragma region Properties
        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;

        wil::untyped_event<winrt::IInspectable> PaletteUpdated;

        ColorSource Source() const;
        void Source(ColorSource const& value);

        wil::single_threaded_rw_property<winrt::IVector<ColorPaletteSelector>> PaletteSelectors;

        winrt::IVectorView<PaletteColor> Palette() const;

        void OnSourceUpdated(winrt::IInspectable const& sender, winrt::IInspectable const& e);
#pragma endregion

    private:
        winrt::IAsyncOperation<winrt::IVector<winrt::float3>> SampleSourcePixelColorsAsync(int sampleCount);

        static void OnSourceChanged(
            winrt::DependencyObject const& d,
            winrt::DependencyPropertyChangedEventArgs const& e);

		winrt::IVector<PaletteColor> _palette{ nullptr };
		ColorSource::SourceUpdated_revoker _sourceUpdatedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorPaletteSampler : ColorPaletteSamplerT<ColorPaletteSampler, implementation::ColorPaletteSampler>
    {
    };
}
