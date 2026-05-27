#include "pch.h"
#include "winrt_module_imports.h"
#include "UrlColorSource.h"
#if __has_include("UrlColorSource.g.cpp")
#include "UrlColorSource.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> UrlColorSource::SourceProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L""), &UrlColorSource::OnSourceChanged});

    winrt::hstring UrlColorSource::Source() const
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(SourceProperty()));
    }

    void UrlColorSource::Source(winrt::hstring const& value)
    {
        SetValue(SourceProperty(), winrt::box_value(value));
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::IRandomAccessStream> UrlColorSource::GetPixelDataAsync(int requestedSamples)
    {
        // Ensure the source is populated
        if (Source().empty())
            co_return nullptr;

		auto uri = winrt::Windows::Foundation::Uri(Source());

        winrt::Windows::Storage::Streams::IRandomAccessStream stream{ nullptr };

        if (uri.SchemeName() == L"file") {
            auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(uri.AbsoluteUri());
            stream = co_await file.OpenAsync(winrt::Windows::Storage::FileAccessMode::Read);
        }
        else {
            stream = co_await winrt::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromUri(uri).OpenReadAsync();
        }

        auto decoder = co_await winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(stream);
        auto pixelData = co_await decoder.GetPixelDataAsync();
        auto bytes = pixelData.DetachPixelData();

        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;
        Windows::Storage::Streams::DataWriter writer;
        writer.WriteBytes(bytes);

        co_await randomAccessStream.WriteAsync(writer.DetachBuffer());
        randomAccessStream.Seek(0);
        co_return randomAccessStream;
    }

    void UrlColorSource::OnSourceChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d, 
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto source = d.try_as<class_type>())
        {
            auto sourceImpl = winrt::get_self<implementation::UrlColorSource>(source);
            sourceImpl->InvokeSourceUpdated();
        }
    }
}
