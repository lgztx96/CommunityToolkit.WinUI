#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "UIColorSource.h"
#if __has_include("UIColorSource.g.cpp")
#include "UIColorSource.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UIColorSource::SourceProperty =
        winrt::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr, &UIColorSource::OnSourceChanged });

    winrt::UIElement UIColorSource::Source() const
    {
        return GetValue(SourceProperty()).try_as<winrt::UIElement>();
    }

    void UIColorSource::Source(winrt::UIElement const& value)
    {
        SetValue(SourceProperty(), value);
    }

    winrt::IAsyncOperation<winrt::IRandomAccessStream> UIColorSource::GetPixelDataAsync(int requestedSamples)
    {
        // Ensure the source is populated
		auto source = Source();
        if (source == nullptr)
            co_return nullptr;

        // Grab actual size
        // If actualSize is 0, replace with 1:1 aspect ratio
        auto sourceSize = source.ActualSize();
        sourceSize = sourceSize != winrt::float2::zero() ? sourceSize : winrt::float2::one();

        // Calculate size of scaled rerender using the actual size
        // scaled down to the sample count, maintaining aspect ration
        auto sourceArea = sourceSize.x * sourceSize.y;
        auto sampleScale = std::sqrt(requestedSamples / sourceArea);
        auto sampleSize = sourceSize * sampleScale;

        // Rerender the UIElement to a bitmap of about sampleCount pixels
        // Note: RenderTargetBitmap is not supported with Uno Platform.
        winrt::RenderTargetBitmap bitmap;
        co_await bitmap.RenderAsync(source, (int)sampleSize.x, (int)sampleSize.y);

        // Create a stream from the bitmap
        auto pixels = co_await bitmap.GetPixelsAsync();

        winrt::InMemoryRandomAccessStream randomAccessStream;

        co_await randomAccessStream.WriteAsync(pixels);
        randomAccessStream.Seek(0);
        co_return randomAccessStream;
    }

    void UIColorSource::OnSourceChanged(
        winrt::DependencyObject const& d,
        [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto source = d.try_as<class_type>())
        {
            auto sourceImpl = winrt::get_self<implementation::UIColorSource>(source);
            sourceImpl->InvokeSourceUpdated();
        }
    }
}
