#pragma once

#include "MatrixExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace ::winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct MatrixExtensions
    {
        static bool HasInverse(winrt::Matrix const& matrix) noexcept;

        static winrt::Matrix Multiply(winrt::Matrix const& matrix1, winrt::Matrix const& matrix2) noexcept;

        static winrt::Matrix Round(winrt::Matrix const& matrix, int decimalsAfterRound) noexcept;

        static winrt::Matrix Rotate(winrt::Matrix const& matrix, double angle) noexcept;

        static winrt::Matrix RotateAt(winrt::Matrix const& matrix, double angle, double centerX, double centerY) noexcept;

        static winrt::Matrix Scale(winrt::Matrix const& matrix, double scaleX, double scaleY) noexcept;

        static winrt::Matrix ScaleAt(winrt::Matrix const& matrix, double scaleX, double scaleY, double centerX, double centerY) noexcept;

        static winrt::Matrix Skew(winrt::Matrix const& matrix, double skewX, double skewY) noexcept;

        static winrt::Matrix Translate(winrt::Matrix const& matrix, double offsetX, double offsetY) noexcept;

        static winrt::Matrix CreateRotationRadians(double angle) noexcept;

        static winrt::Matrix CreateRotationRadians(double angle, double centerX, double centerY) noexcept;

        static winrt::Matrix CreateScaling(double scaleX, double scaleY) noexcept;

        static winrt::Matrix CreateScaling(double scaleX, double scaleY, double centerX, double centerY) noexcept;

        static winrt::Matrix CreateSkewRadians(double skewX, double skewY) noexcept;

        static double Round(double value, int decimals) noexcept;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct MatrixExtensions : MatrixExtensionsT<MatrixExtensions, implementation::MatrixExtensions>
    {
    };
}
