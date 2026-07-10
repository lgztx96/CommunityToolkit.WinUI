#include "pch.h"
#include "winrt_module_imports.h"
#include "StreamColorSource.h"
#if __has_include("StreamColorSource.g.cpp")
#include "StreamColorSource.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StreamColorSource::SourceProperty =
        winrt::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<winrt::IRandomAccessStream>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr, &StreamColorSource::OnSourceChanged });

    winrt::IRandomAccessStream StreamColorSource::Source() const
    {
        return GetValue(SourceProperty()).try_as<winrt::IRandomAccessStream>();
    }

    void StreamColorSource::Source(winrt::IRandomAccessStream const& value)
    {
        SetValue(SourceProperty(), value);
    }

    winrt::IAsyncOperation<winrt::IRandomAccessStream> StreamColorSource::GetPixelDataAsync(int requestedSamples)
    {
        auto decoder = co_await winrt::BitmapDecoder::CreateAsync(Source());
        auto pixelData = co_await decoder.GetPixelDataAsync();
        auto bytes = pixelData.DetachPixelData();
        winrt::InMemoryRandomAccessStream randomAccessStream;
        winrt::DataWriter writer;
		writer.WriteBytes(bytes);

        co_await randomAccessStream.WriteAsync(writer.DetachBuffer());
        randomAccessStream.Seek(0);
        co_return randomAccessStream;
    }

    void StreamColorSource::OnSourceChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto source = d.try_as<class_type>())
        {
			auto sourceImpl = winrt::get_self<implementation::StreamColorSource>(source);
            sourceImpl->InvokeSourceUpdated();
        }
    }
}
