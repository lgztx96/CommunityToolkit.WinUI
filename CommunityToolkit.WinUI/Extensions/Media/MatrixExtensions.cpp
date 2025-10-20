#include "pch.h"
#include "MatrixExtensions.h"
#if __has_include("MatrixExtensions.g.cpp")
#include "MatrixExtensions.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
    bool MatrixExtensions::HasInverse(Matrix const& matrix)
    {
        return ((matrix.M11 * matrix.M22) - (matrix.M12 * matrix.M21)) != 0;
    }

    Matrix MatrixExtensions::Multiply(Matrix const& matrix1, Matrix const& matrix2)
    {
        return Matrix(
            (matrix1.M11 * matrix2.M11) + (matrix1.M12 * matrix2.M21),
            (matrix1.M11 * matrix2.M12) + (matrix1.M12 * matrix2.M22),
            (matrix1.M21 * matrix2.M11) + (matrix1.M22 * matrix2.M21),
            (matrix1.M21 * matrix2.M12) + (matrix1.M22 * matrix2.M22),
            (matrix1.OffsetX * matrix2.M11) + (matrix1.OffsetY * matrix2.M21) + matrix2.OffsetX,
            (matrix1.OffsetX * matrix2.M12) + (matrix1.OffsetY * matrix2.M22) + matrix2.OffsetY);
    }

    Matrix MatrixExtensions::Round(Matrix const& matrix, int decimalsAfterRound)
    {
        return Matrix(
            Round(matrix.M11, decimalsAfterRound),
            Round(matrix.M12, decimalsAfterRound),
            Round(matrix.M21, decimalsAfterRound),
            Round(matrix.M22, decimalsAfterRound),
            matrix.OffsetX,
            matrix.OffsetY);
    }

    Matrix MatrixExtensions::Rotate(Matrix const& matrix, double angle)
    {
        return Multiply(matrix, CreateRotationRadians(std::fmod(angle, 360) * (std::numbers::pi / 180.0)));
    }

    Matrix MatrixExtensions::RotateAt(Matrix const& matrix, double angle, double centerX, double centerY)
    {
        return Multiply(matrix, CreateRotationRadians(std::fmod(angle, 360) * (std::numbers::pi / 180.0), centerX, centerY));
    }

    Matrix MatrixExtensions::Scale(Matrix const& matrix, double scaleX, double scaleY)
    {
        return Multiply(matrix, CreateScaling(scaleX, scaleY));
    }

    Matrix MatrixExtensions::ScaleAt(Matrix const& matrix, double scaleX, double scaleY, double centerX, double centerY)
    {
        return Multiply(matrix, CreateScaling(scaleX, scaleY, centerX, centerY));
    }

    Matrix MatrixExtensions::Skew(Matrix const& matrix, double skewX, double skewY)
    {
        return Multiply(matrix, CreateSkewRadians(std::fmod(skewX, 360) * (std::numbers::pi / 180.0), std::fmod(skewY, 360) * (std::numbers::pi / 180.0)));
    }

    Matrix MatrixExtensions::Translate(Matrix const& matrix, double offsetX, double offsetY)
    {
        return Matrix(matrix.M11, matrix.M12, matrix.M21, matrix.M22, matrix.OffsetX + offsetX, matrix.OffsetY + offsetY);
    }

    Matrix MatrixExtensions::CreateRotationRadians(double angle)
    {
        return CreateRotationRadians(angle, 0, 0);
    }

    Matrix MatrixExtensions::CreateRotationRadians(double angle, double centerX, double centerY)
    {
        auto sin = std::sin(angle);
        auto cos = std::cos(angle);
        auto dx = (centerX * (1.0 - cos)) + (centerY * sin);
        auto dy = (centerY * (1.0 - cos)) - (centerX * sin);

        return Matrix(cos, sin, -sin, cos, dx, dy);
    }

    Matrix MatrixExtensions::CreateScaling(double scaleX, double scaleY)
    {
        return Matrix(scaleX, 0, 0, scaleY, 0, 0);
    }

    Matrix MatrixExtensions::CreateScaling(double scaleX, double scaleY, double centerX, double centerY)
    {
        return Matrix(scaleX, 0, 0, scaleY, centerX - (scaleX * centerX), centerY - (scaleY * centerY));
    }

    Matrix MatrixExtensions::CreateSkewRadians(double skewX, double skewY)
    {
        return Matrix(1.0, std::tan(skewY), std::tan(skewX), 1.0, 0.0, 0.0);
    }

    double MatrixExtensions::Round(double value, int decimals)
    {
        double factor = std::pow(10.0, decimals);
        return std::round(value * factor) / factor;
    }
}
