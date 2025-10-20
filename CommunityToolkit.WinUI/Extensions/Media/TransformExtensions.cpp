#include "pch.h"
#include "TransformExtensions.h"
#if __has_include("TransformExtensions.g.cpp")
#include "TransformExtensions.g.cpp"
#endif
#include "MatrixExtensions.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	Matrix TransformExtensions::GetMatrix(RotateTransform const& transform)
	{
		return MatrixExtensions::RotateAt(winrt::Microsoft::UI::Xaml::Media::MatrixHelper::Identity(), transform.Angle(), transform.CenterX(), transform.CenterY());
	}

	Matrix TransformExtensions::GetMatrix(ScaleTransform const& transform)
	{
		return MatrixExtensions::ScaleAt(winrt::Microsoft::UI::Xaml::Media::MatrixHelper::Identity(), transform.ScaleX(), transform.ScaleY(), transform.CenterX(), transform.CenterY());
	}

	Matrix TransformExtensions::GetMatrix(SkewTransform const& transform)
	{
		Matrix matrix = winrt::Microsoft::UI::Xaml::Media::MatrixHelper::Identity();

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

	Matrix TransformExtensions::GetMatrix(TranslateTransform const& transform)
	{
		return MatrixExtensions::Translate(MatrixHelper::Identity(), transform.X(), transform.Y());
	}

	Matrix TransformExtensions::GetMatrix(Transform const& transform)
	{
		if (auto rotate = transform.try_as<RotateTransform>()) {
			return GetMatrix(rotate);
		}
		else if (auto scale = transform.try_as<ScaleTransform>()) {
			return GetMatrix(scale);
		}
		else if (auto skew = transform.try_as<SkewTransform>()) {
			return GetMatrix(skew);
		}
		else if (auto translate = transform.try_as<TranslateTransform>()) {
			return GetMatrix(translate);
		}
		else {
			throw winrt::hresult_invalid_argument(L"unsupported transform type");
		}
	}
}
