#pragma once

#include "MatrixExtensions.g.h"
#include <numbers>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct MatrixExtensions : MatrixExtensionsT<MatrixExtensions>
    {
        static bool HasInverse(Matrix const& matrix);

        static Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2);

        static Matrix Round(Matrix const& matrix, int decimalsAfterRound);

        static Matrix Rotate(Matrix const& matrix, double angle);

        static Matrix RotateAt(Matrix const& matrix, double angle, double centerX, double centerY);

        static Matrix Scale(Matrix const& matrix, double scaleX, double scaleY);

        static Matrix ScaleAt(Matrix const& matrix, double scaleX, double scaleY, double centerX, double centerY);

        static Matrix Skew(Matrix const& matrix, double skewX, double skewY);

        static Matrix Translate(Matrix const& matrix, double offsetX, double offsetY);

        static Matrix CreateRotationRadians(double angle);

        static Matrix CreateRotationRadians(double angle, double centerX, double centerY);

        static Matrix CreateScaling(double scaleX, double scaleY);

        static Matrix CreateScaling(double scaleX, double scaleY, double centerX, double centerY);

        static Matrix CreateSkewRadians(double skewX, double skewY);

        static double Round(double value, int decimals);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct MatrixExtensions : MatrixExtensionsT<MatrixExtensions, implementation::MatrixExtensions>
    {
    };
}
