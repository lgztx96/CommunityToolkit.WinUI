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
    struct RectExtensions : RectExtensionsT<RectExtensions>
    {
        static bool IntersectsWith(Rect rect1, Rect rect2);

        static Size ToSize(Rect rect);

        static Rect Transform(Rect rectangle, Matrix const& matrix);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct RectExtensions : RectExtensionsT<RectExtensions, implementation::RectExtensions>
    {
    };
}
