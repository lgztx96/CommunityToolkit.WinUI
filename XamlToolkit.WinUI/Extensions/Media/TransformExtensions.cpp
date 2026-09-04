#include "pch.h"
#include "winrt_module_imports.h"
#include "TransformExtensions.h"
#if __has_include("TransformExtensions.g.cpp")
#include "TransformExtensions.g.cpp"
#endif
#include "MatrixExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::Matrix TransformExtensions::GetMatrix(winrt::RotateTransform const& transform) noexcept
	{
		return MatrixExtensions::RotateAt(winrt::MatrixHelper::Identity(), transform.Angle(), transform.CenterX(), transform.CenterY());
	}

	winrt::Matrix TransformExtensions::GetMatrix(winrt::ScaleTransform const& transform) noexcept
	{
		return MatrixExtensions::ScaleAt(winrt::MatrixHelper::Identity(), transform.ScaleX(), transform.ScaleY(), transform.CenterX(), transform.CenterY());
	}

	winrt::Matrix TransformExtensions::GetMatrix(winrt::SkewTransform const& transform) noexcept
	{
		winrt::Matrix matrix = winrt::MatrixHelper::Identity();

		auto angleX = transform.AngleX();
		auto angleY = transform.AngleY();
		auto centerX = transform.CenterX();
		auto centerY = transform.CenterY();

		bool hasCenter = centerX != 0 || centerY != 0;

		if (hasCenter)
		{
			// If we have a center, translate matrix before/after skewing.
			matrix = MatrixExtensions::Translate(matrix, -centerX, -centerY);
			matrix = MatrixExtensions::Skew(matrix, angleX, angleY);
			matrix = MatrixExtensions::Translate(matrix, centerX, centerY);
		}
		else
		{
			matrix = MatrixExtensions::Skew(matrix, angleX, angleY);
		}

		return matrix;
	}

	winrt::Matrix TransformExtensions::GetMatrix(winrt::TranslateTransform const& transform) noexcept
	{
		return MatrixExtensions::Translate(winrt::MatrixHelper::Identity(), transform.X(), transform.Y());
	}

	winrt::Matrix TransformExtensions::GetMatrix(winrt::Transform const& transform)
	{
		if (const auto rotate = transform.try_as<winrt::RotateTransform>()) 
		{
			return GetMatrix(rotate);
		}
		else if (const auto scale = transform.try_as<winrt::ScaleTransform>())
		{
			return GetMatrix(scale);
		}
		else if (const auto skew = transform.try_as<winrt::SkewTransform>())
		{
			return GetMatrix(skew);
		}
		else if (const auto translate = transform.try_as<winrt::TranslateTransform>())
		{
			return GetMatrix(translate);
		}
		else 
		{
			throw winrt::hresult_invalid_argument(L"unsupported transform type");
		}
	}
}
