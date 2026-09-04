#pragma once

#include "PointExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace ::winrt::Windows::Foundation;
    using namespace ::winrt::Windows::Foundation::Numerics;
    using namespace ::winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct PointExtensions
    {
        static winrt::Rect ToRect(winrt::Point point, double width, double height) noexcept;

        static winrt::Rect ToRect(winrt::Point point, winrt::Point end) noexcept;

        static winrt::Rect ToRect(winrt::Point point, winrt::Size size) noexcept;

        static winrt::float3 ToVector3(winrt::Point point) noexcept;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct PointExtensions : PointExtensionsT<PointExtensions, implementation::PointExtensions>
    {
    };
}
