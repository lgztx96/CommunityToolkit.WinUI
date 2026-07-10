#pragma once

#include "UrlColorSource.g.h"
#include "ColorSource.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Storage;
    using namespace Windows::Storage::Streams;
    using namespace Windows::Graphics::Imaging;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Imaging;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct UrlColorSource : UrlColorSourceT<UrlColorSource, implementation::ColorSource>
    {
        UrlColorSource() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;

        winrt::hstring Source() const;
        void Source(winrt::hstring const& value);

        winrt::IAsyncOperation<winrt::IRandomAccessStream> GetPixelDataAsync(int requestedSamples) override;

    private:
        static void OnSourceChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct UrlColorSource : UrlColorSourceT<UrlColorSource, implementation::UrlColorSource>
    {
    };
}
