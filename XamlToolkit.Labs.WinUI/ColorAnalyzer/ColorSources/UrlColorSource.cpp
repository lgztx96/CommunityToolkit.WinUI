#include "pch.h"
#include "winrt_module_imports.h"
#include "UrlColorSource.h"
#if __has_include("UrlColorSource.g.cpp")
#include "UrlColorSource.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UrlColorSource::SourceProperty =
        winrt::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(L""), &UrlColorSource::OnSourceChanged });

    winrt::hstring UrlColorSource::Source() const
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(SourceProperty()));
    }

    void UrlColorSource::Source(winrt::hstring const& value)
    {
        SetValue(SourceProperty(), winrt::box_value(value));
    }

    winrt::IAsyncOperation<winrt::IRandomAccessStream> UrlColorSource::GetPixelDataAsync(int requestedSamples)
    {
        // Ensure the source is populated
		auto source = Source();
        if (source.empty())
            co_return nullptr;

		auto uri = winrt::Uri(source);

        winrt::IRandomAccessStream stream{ nullptr };

        if (uri.SchemeName() == L"file") {
            auto file = co_await winrt::StorageFile::GetFileFromPathAsync(uri.AbsoluteUri());
            stream = co_await file.OpenAsync(winrt::FileAccessMode::Read);
        }
        else {
            stream = co_await winrt::RandomAccessStreamReference::CreateFromUri(uri).OpenReadAsync();
        }

        auto decoder = co_await winrt::BitmapDecoder::CreateAsync(stream);
        auto pixelData = co_await decoder.GetPixelDataAsync();
        auto bytes = pixelData.DetachPixelData();

        winrt::InMemoryRandomAccessStream randomAccessStream;
        winrt::DataWriter writer;
        writer.WriteBytes(bytes);

        co_await randomAccessStream.WriteAsync(writer.DetachBuffer());
        randomAccessStream.Seek(0);
        co_return randomAccessStream;
    }

    void UrlColorSource::OnSourceChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto source = d.try_as<class_type>())
        {
            auto sourceImpl = winrt::get_self<implementation::UrlColorSource>(source);
            sourceImpl->InvokeSourceUpdated();
        }
    }
}
