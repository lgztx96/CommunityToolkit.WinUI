#pragma once

#include "ColorSource.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Storage::Streams;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ColorSource : ColorSourceT<ColorSource>
    {
        ColorSource() = default;

        wil::untyped_event<winrt::IInspectable> SourceUpdated;

        virtual winrt::IAsyncOperation<winrt::IRandomAccessStream> GetPixelDataAsync([[maybe_unused]] int32_t requestedSamples) { return nullptr;  };

        void InvokeSourceUpdated() { SourceUpdated.invoke(*this, nullptr); }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ColorSource : ColorSourceT<ColorSource, implementation::ColorSource>
    {
    };
}
