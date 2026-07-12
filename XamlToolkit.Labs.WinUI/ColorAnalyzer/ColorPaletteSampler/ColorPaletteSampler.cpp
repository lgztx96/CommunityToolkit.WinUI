#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorPaletteSampler.h"
#if __has_include("ColorPaletteSampler.g.cpp")
#include "ColorPaletteSampler.g.cpp"
#endif
#include "ColorPaletteSampler.DBScan.h"
#include "../ColorExtensions.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    ColorPaletteSampler::ColorPaletteSampler()
    {
        PaletteSelectors(winrt::single_threaded_vector<ColorPaletteSelector>());
    }

    void ColorPaletteSampler::UpdatePalette()
    {
        UpdatePaletteAsync();
    }

    winrt::IAsyncAction ColorPaletteSampler::UpdatePaletteAsync()
    {
        auto strongThis = get_strong();
        // No palettes to update.
        // Skip a lot of unnecessary computation
        if (PaletteSelectors().Size() == 0)
            co_return;

        static constexpr int sampleCount = 4096;
        static constexpr size_t k = 8;
        static constexpr float mergeDistance = 0.12f;

        // Retreive pixel samples from source
        auto winrtSamples = co_await SampleSourcePixelColorsAsync(sampleCount);
        
        // Failed to retreive pixel data. Cancel
        if (winrtSamples.Size() == 0)
            co_return;

        std::vector<winrt::float3> samples(winrtSamples.begin(), winrtSamples.end());

        // Cluster samples in RGB floating-point color space
        // With Euclidean Squared distance function, then construct palette data.
        // Merge KMeans results that are too similar, using DBScan
		std::vector<int> counts;
        auto kClusters = KMeansCluster(samples, k, counts);

        std::vector<float> weights;
        weights.reserve(counts.size());
        for (int c : counts)
        {
            weights.push_back(static_cast<float>(c) / static_cast<float>(samples.size()));
        }

        // DBScan
        auto dbClusters = DBScan::Cluster(
            std::span<winrt::float3>{ kClusters },
            mergeDistance,
            0,
            weights);

        // dbClusters + weights -> colorData
        std::vector<PaletteColor> colorData;
        colorData.reserve(dbClusters.size());

        for (size_t i = 0; i < dbClusters.size(); i++)
        {
            colorData.emplace_back(ColorExtensions::ToColor(dbClusters[i]), weights[i]);
        }

        // Update palettes on the UI thread
		auto dispatcherQueue = DispatcherQueue().GetForCurrentThread();
        for (const auto& palette : PaletteSelectors())
        {
            dispatcherQueue.TryEnqueue([=]()
            {
                palette.SelectColors(colorData);
            });
        }

        // Update palette property
        // Not a dependency property, so no need to update from the UI Thread
        _palette = winrt::single_threaded_vector(std::move(colorData));

        // Invoke palette updated event from the UI thread
        dispatcherQueue.TryEnqueue([=]()
        {
            PaletteUpdated.invoke(*this, nullptr);
        });
    }

    winrt::IAsyncOperation<winrt::IVector<winrt::float3>> ColorPaletteSampler::SampleSourcePixelColorsAsync(int sampleCount)
    {
        if (Source() == nullptr)
            co_return winrt::single_threaded_vector<winrt::float3>();

        auto pixelByteStream = co_await Source().GetPixelDataAsync(sampleCount);

        // Something went wrong
        if (pixelByteStream == nullptr || pixelByteStream.Size() == 0)
            co_return winrt::single_threaded_vector<winrt::float3>();

        // Read the stream into a a color array
        const int bytesPerPixel = 4;
        std::vector<winrt::float3> samples;
		samples.resize(sampleCount);
        // Iterate through the stream reading a pixel (4 bytes) at a time
        // and storing them as a Vector3. Opacity info is dropped.
        int colorIndex = 0;
        auto step = (pixelByteStream.Size() / sampleCount);
        step -= step % 4;

        auto pixelBytes = winrt::Windows::Storage::Streams::Buffer(bytesPerPixel);

        while (true)
        {
            winrt::Windows::Storage::Streams::IBuffer result = co_await pixelByteStream.ReadAsync(
                pixelBytes,
                bytesPerPixel,
                winrt::Windows::Storage::Streams::InputStreamOptions::None);

            if (result.Length() != bytesPerPixel)
                break;

            auto bytes = result.data();

            // Skip fully transparent pixels
            if (bytes[3] == 0)
                continue;

            // Take the red, green, and blue channels to make a floating-point space color.
            samples[colorIndex] =
                winrt::float3{
                    static_cast<float>(bytes[2]),
                    static_cast<float>(bytes[1]),
                    static_cast<float>(bytes[0])
            } / 255.0f;

            colorIndex++;

            // Advance by step amount
            auto pos = pixelByteStream.Position();
            pixelByteStream.Seek(pos + step);
        }

        // If we skipped any pixels, trim the span
        samples.resize(colorIndex);

        co_return winrt::single_threaded_vector<winrt::float3>(std::move(samples));
    }
}
