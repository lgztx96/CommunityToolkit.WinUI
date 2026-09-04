#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#include <numbers>
#endif
#include "MatrixExtensions.h"
#if __has_include("MatrixExtensions.g.cpp")
#include "MatrixExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    bool MatrixExtensions::HasInverse(winrt::Matrix const& matrix) noexcept
    {
        return ((matrix.M11 * matrix.M22) - (matrix.M12 * matrix.M21)) != 0;
    }

    winrt::Matrix MatrixExtensions::Multiply(winrt::Matrix const& matrix1, winrt::Matrix const& matrix2) noexcept
    {
        return winrt::Matrix(
            (matrix1.M11 * matrix2.M11) + (matrix1.M12 * matrix2.M21),
            (matrix1.M11 * matrix2.M12) + (matrix1.M12 * matrix2.M22),
            (matrix1.M21 * matrix2.M11) + (matrix1.M22 * matrix2.M21),
            (matrix1.M21 * matrix2.M12) + (matrix1.M22 * matrix2.M22),
            (matrix1.OffsetX * matrix2.M11) + (matrix1.OffsetY * matrix2.M21) + matrix2.OffsetX,
            (matrix1.OffsetX * matrix2.M12) + (matrix1.OffsetY * matrix2.M22) + matrix2.OffsetY);
    }

    winrt::Matrix MatrixExtensions::Round(winrt::Matrix const& matrix, int decimalsAfterRound) noexcept
    {
        return winrt::Matrix(
            Round(matrix.M11, decimalsAfterRound),
            Round(matrix.M12, decimalsAfterRound),
            Round(matrix.M21, decimalsAfterRound),
            Round(matrix.M22, decimalsAfterRound),
            matrix.OffsetX,
            matrix.OffsetY);
    }

    winrt::Matrix MatrixExtensions::Rotate(winrt::Matrix const& matrix, double angle) noexcept
    {
        return Multiply(matrix, CreateRotationRadians(std::fmod(angle, 360) * (std::numbers::pi / 180.0)));
    }

    winrt::Matrix MatrixExtensions::RotateAt(winrt::Matrix const& matrix, double angle, double centerX, double centerY) noexcept
    {
        return Multiply(matrix, CreateRotationRadians(std::fmod(angle, 360) * (std::numbers::pi / 180.0), centerX, centerY));
    }

    winrt::Matrix MatrixExtensions::Scale(winrt::Matrix const& matrix, double scaleX, double scaleY) noexcept
    {
        return Multiply(matrix, CreateScaling(scaleX, scaleY));
    }

    winrt::Matrix MatrixExtensions::ScaleAt(winrt::Matrix const& matrix, double scaleX, double scaleY, double centerX, double centerY) noexcept
    {
        return Multiply(matrix, CreateScaling(scaleX, scaleY, centerX, centerY));
    }

    winrt::Matrix MatrixExtensions::Skew(winrt::Matrix const& matrix, double skewX, double skewY) noexcept
    {
        return Multiply(matrix, CreateSkewRadians(std::fmod(skewX, 360) * (std::numbers::pi / 180.0), std::fmod(skewY, 360) * (std::numbers::pi / 180.0)));
    }

    winrt::Matrix MatrixExtensions::Translate(winrt::Matrix const& matrix, double offsetX, double offsetY) noexcept
    {
        return winrt::Matrix(matrix.M11, matrix.M12, matrix.M21, matrix.M22, matrix.OffsetX + offsetX, matrix.OffsetY + offsetY);
    }

    winrt::Matrix MatrixExtensions::CreateRotationRadians(double angle) noexcept
    {
        return CreateRotationRadians(angle, 0, 0);
    }

    winrt::Matrix MatrixExtensions::CreateRotationRadians(double angle, double centerX, double centerY) noexcept
    {
        auto sin = std::sin(angle);
        auto cos = std::cos(angle);
        auto dx = (centerX * (1.0 - cos)) + (centerY * sin);
        auto dy = (centerY * (1.0 - cos)) - (centerX * sin);

        return winrt::Matrix(cos, sin, -sin, cos, dx, dy);
    }

    winrt::Matrix MatrixExtensions::CreateScaling(double scaleX, double scaleY) noexcept
    {
        return winrt::Matrix(scaleX, 0, 0, scaleY, 0, 0);
    }

    winrt::Matrix MatrixExtensions::CreateScaling(double scaleX, double scaleY, double centerX, double centerY) noexcept
    {
        return winrt::Matrix(scaleX, 0, 0, scaleY, centerX - (scaleX * centerX), centerY - (scaleY * centerY));
    }

    winrt::Matrix MatrixExtensions::CreateSkewRadians(double skewX, double skewY) noexcept
    {
        return winrt::Matrix(1.0, std::tan(skewY), std::tan(skewX), 1.0, 0.0, 0.0);
    }

    double MatrixExtensions::Round(double value, int decimals) noexcept
    {
        double factor = std::pow(10.0, decimals);
        return std::round(value * factor) / factor;
    }
}
