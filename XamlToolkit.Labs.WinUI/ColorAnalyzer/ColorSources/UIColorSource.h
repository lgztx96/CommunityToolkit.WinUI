#pragma once

#include "UIColorSource.g.h"
#include "ColorSource.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace winrt::Windows::Foundation::Numerics;
	using namespace Windows::Storage::Streams;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media::Imaging;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct UIColorSource : UIColorSourceT<UIColorSource, implementation::ColorSource>
    {
        UIColorSource() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;

        winrt::UIElement Source() const;
        void Source(winrt::UIElement const& value);

        winrt::IAsyncOperation<winrt::IRandomAccessStream> GetPixelDataAsync(int requestedSamples) override;

    private:
        static void OnSourceChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct UIColorSource : UIColorSourceT<UIColorSource, implementation::UIColorSource>
    {
    };
}
