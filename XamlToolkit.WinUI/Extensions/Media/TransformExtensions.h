#pragma once

#include "TransformExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct TransformExtensions
    {
        static Matrix GetMatrix(RotateTransform const& transform) noexcept;

        static Matrix GetMatrix(ScaleTransform const& transform) noexcept;

        static Matrix GetMatrix(SkewTransform const& transform) noexcept;

        static Matrix GetMatrix(TranslateTransform const& transform) noexcept;

        static Matrix GetMatrix(Transform const& transform);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct TransformExtensions : TransformExtensionsT<TransformExtensions, implementation::TransformExtensions>
    {
    };
}
