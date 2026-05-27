#include "pch.h"
#include "winrt_module_imports.h"
#include "StreamColorSource.h"
#if __has_include("StreamColorSource.g.cpp")
#include "StreamColorSource.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> StreamColorSource::SourceProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<winrt::Windows::Storage::Streams::IRandomAccessStream>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &StreamColorSource::OnSourceChanged });

    winrt::Windows::Storage::Streams::IRandomAccessStream StreamColorSource::Source() const
    {
        return GetValue(SourceProperty()).try_as<winrt::Windows::Storage::Streams::IRandomAccessStream>();
    }

    void StreamColorSource::Source(winrt::Windows::Storage::Streams::IRandomAccessStream const& value)
    {
        SetValue(SourceProperty(), value);
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::IRandomAccessStream> StreamColorSource::GetPixelDataAsync(int requestedSamples)
    {
        auto decoder = co_await winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(Source());
        auto pixelData = co_await decoder.GetPixelDataAsync();
        auto bytes = pixelData.DetachPixelData();
        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;
        Windows::Storage::Streams::DataWriter writer;
		writer.WriteBytes(bytes);

        co_await randomAccessStream.WriteAsync(writer.DetachBuffer());
        randomAccessStream.Seek(0);
        co_return randomAccessStream;
    }

    void StreamColorSource::OnSourceChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d, 
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto source = d.try_as<class_type>())
        {
			auto sourceImpl = winrt::get_self<implementation::StreamColorSource>(source);
            sourceImpl->InvokeSourceUpdated();
        }
    }
}
