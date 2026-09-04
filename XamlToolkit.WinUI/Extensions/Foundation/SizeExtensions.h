#pragma once

#include "SizeExtensions.g.h"

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
    struct SizeExtensions
    {
        static winrt::Rect ToRect(winrt::Size size) noexcept;

        static winrt::Rect ToRect(winrt::Size size, double x, double y) noexcept;

        static winrt::Rect ToRect(winrt::Size size, winrt::Point point) noexcept;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct SizeExtensions : SizeExtensionsT<SizeExtensions, implementation::SizeExtensions>
    {
    };
}
