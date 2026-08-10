#pragma once

#include "MatrixExtensions.g.h"

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
    struct MatrixExtensions
    {
        static bool HasInverse(Matrix const& matrix) noexcept;

        static Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) noexcept;

        static Matrix Round(Matrix const& matrix, int decimalsAfterRound) noexcept;

        static Matrix Rotate(Matrix const& matrix, double angle) noexcept;

        static Matrix RotateAt(Matrix const& matrix, double angle, double centerX, double centerY) noexcept;

        static Matrix Scale(Matrix const& matrix, double scaleX, double scaleY) noexcept;

        static Matrix ScaleAt(Matrix const& matrix, double scaleX, double scaleY, double centerX, double centerY) noexcept;

        static Matrix Skew(Matrix const& matrix, double skewX, double skewY) noexcept;

        static Matrix Translate(Matrix const& matrix, double offsetX, double offsetY) noexcept;

        static Matrix CreateRotationRadians(double angle) noexcept;

        static Matrix CreateRotationRadians(double angle, double centerX, double centerY) noexcept;

        static Matrix CreateScaling(double scaleX, double scaleY) noexcept;

        static Matrix CreateScaling(double scaleX, double scaleY, double centerX, double centerY) noexcept;

        static Matrix CreateSkewRadians(double skewX, double skewY) noexcept;

        static double Round(double value, int decimals) noexcept;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct MatrixExtensions : MatrixExtensionsT<MatrixExtensions, implementation::MatrixExtensions>
    {
    };
}
