#pragma once

#include "StreamColorSource.g.h"
#include "ColorSource.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
    using namespace Windows::Graphics::Imaging;
    using namespace Windows::Storage::Streams;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct StreamColorSource : StreamColorSourceT<StreamColorSource, implementation::ColorSource>
    {
        StreamColorSource() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;

        winrt::IRandomAccessStream Source() const;
        void Source(winrt::IRandomAccessStream const& value);

        winrt::IAsyncOperation<winrt::IRandomAccessStream> GetPixelDataAsync(int requestedSamples) override;

    private:
        static void OnSourceChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct StreamColorSource : StreamColorSourceT<StreamColorSource, implementation::StreamColorSource>
    {
    };
}
