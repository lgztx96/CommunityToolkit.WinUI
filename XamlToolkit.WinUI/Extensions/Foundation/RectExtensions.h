#pragma once

#include "RectExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct RectExtensions
    {
        static bool IntersectsWith(winrt::Rect rect1, winrt::Rect rect2);

        static winrt::Size ToSize(winrt::Rect rect);

        static winrt::Rect Transform(winrt::Rect rectangle, winrt::Matrix const& matrix);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct RectExtensions : RectExtensionsT<RectExtensions, implementation::RectExtensions>
    {
    };
}
