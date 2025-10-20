#pragma once

#include "TransformExtensions.g.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct TransformExtensions : TransformExtensionsT<TransformExtensions>
    {
        static Matrix GetMatrix(RotateTransform const& transform);

        static Matrix GetMatrix(ScaleTransform const& transform);

        static Matrix GetMatrix(SkewTransform const& transform);

        static Matrix GetMatrix(TranslateTransform const& transform);

        static Matrix GetMatrix(Transform const& transform);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct TransformExtensions : TransformExtensionsT<TransformExtensions, implementation::TransformExtensions>
    {
    };
}
