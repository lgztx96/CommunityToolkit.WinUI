#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RectExtensions.h"
#if __has_include("RectExtensions.g.cpp")
#include "RectExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	bool RectExtensions::IntersectsWith(winrt::Rect rect1, winrt::Rect rect2)
	{
		if (winrt::RectHelper::GetIsEmpty(rect1) || winrt::RectHelper::GetIsEmpty(rect2))
		{
			return false;
		}

		auto left1 = winrt::RectHelper::GetLeft(rect1);
		auto top1 = winrt::RectHelper::GetTop(rect1);
		auto right1 = winrt::RectHelper::GetRight(rect1);
		auto bottom1 = winrt::RectHelper::GetBottom(rect1);

		auto left2 = winrt::RectHelper::GetLeft(rect2);
		auto top2 = winrt::RectHelper::GetTop(rect2);
		auto right2 = winrt::RectHelper::GetRight(rect2);
		auto bottom2 = winrt::RectHelper::GetBottom(rect2);

		return (left1 <= right2) && (right1 >= left2) && (top1 <= bottom2) && (bottom1 >= top2);
	}

	winrt::Size RectExtensions::ToSize(winrt::Rect rect) noexcept
	{
		return winrt::Size(rect.Width, rect.Height);
	}

	winrt::Rect RectExtensions::Transform(winrt::Rect rectangle, winrt::Matrix const& matrix)
	{
		auto left = winrt::RectHelper::GetLeft(rectangle);
		auto top = winrt::RectHelper::GetTop(rectangle);
		auto right = winrt::RectHelper::GetRight(rectangle);
		auto bottom = winrt::RectHelper::GetBottom(rectangle);

		winrt::Point leftTop = winrt::MatrixHelper::Transform(matrix, Point(left, top));
		winrt::Point rightTop = winrt::MatrixHelper::Transform(matrix, Point(right, top));
		winrt::Point leftBottom = winrt::MatrixHelper::Transform(matrix, Point(left, bottom));
		winrt::Point rightBottom = winrt::MatrixHelper::Transform(matrix, Point(right, bottom));

		left = std::min<float>(std::min<float>(leftTop.X, rightTop.X), std::min<float>(leftBottom.X, rightBottom.X));
		top = std::min<float>(std::min<float>(leftTop.Y, rightTop.Y), std::min<float>(leftBottom.Y, rightBottom.Y));
		right = std::max<float>(std::max<float>(leftTop.X, rightTop.X), std::max<float>(leftBottom.X, rightBottom.X));
		bottom = std::max<float>(std::max<float>(leftTop.Y, rightTop.Y), std::max<float>(leftBottom.Y, rightBottom.Y));

		auto width = right - left;
		auto height = bottom - top;
		return winrt::Rect(left, top, width, height);
	}
}
