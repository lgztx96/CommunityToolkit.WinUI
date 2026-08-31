#pragma once

#include "TransformExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace ::winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct TransformExtensions
    {
        static winrt::Matrix GetMatrix(winrt::RotateTransform const& transform) noexcept;

        static winrt::Matrix GetMatrix(winrt::ScaleTransform const& transform) noexcept;

        static winrt::Matrix GetMatrix(winrt::SkewTransform const& transform) noexcept;

        static winrt::Matrix GetMatrix(winrt::TranslateTransform const& transform) noexcept;

        static winrt::Matrix GetMatrix(winrt::Transform const& transform);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct TransformExtensions : TransformExtensionsT<TransformExtensions, implementation::TransformExtensions>
    {
    };
}
