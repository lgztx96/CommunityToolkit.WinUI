#pragma once

#include "RectExtensions.g.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct RectExtensions : RectExtensionsT<RectExtensions>
    {
        static bool IntersectsWith(Rect rect1, Rect rect2);

        static Size ToSize(Rect rect);

        static Rect Transform(Rect rectangle, Matrix const& matrix);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct RectExtensions : RectExtensionsT<RectExtensions, implementation::RectExtensions>
    {
    };
}
