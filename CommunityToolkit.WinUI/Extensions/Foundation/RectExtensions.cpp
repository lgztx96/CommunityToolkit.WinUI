#include "pch.h"
#include "RectExtensions.h"
#if __has_include("RectExtensions.g.cpp")
#include "RectExtensions.g.cpp"
#endif
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::implementation
{
	bool RectExtensions::IntersectsWith(Rect rect1, Rect rect2)
	{
		if (RectHelper::GetIsEmpty(rect1) || RectHelper::GetIsEmpty(rect2))
		{
			return false;
		}

		auto left1 = RectHelper::GetLeft(rect1);
		auto top1 = RectHelper::GetTop(rect1);
		auto right1 = RectHelper::GetRight(rect1);
		auto bottom1 = RectHelper::GetBottom(rect1);

		auto left2 = RectHelper::GetLeft(rect2);
		auto top2 = RectHelper::GetTop(rect2);
		auto right2 = RectHelper::GetRight(rect2);
		auto bottom2 = RectHelper::GetBottom(rect2);

		return (left1 <= right2) && (right1 >= left2) && (top1 <= bottom2) && (bottom1 >= top2);
	}

	Size RectExtensions::ToSize(Rect rect)
	{
		return Size(rect.Width, rect.Height);
	}

	Rect RectExtensions::Transform(Rect rectangle, Matrix const& matrix)
	{
		auto left = RectHelper::GetLeft(rectangle);
		auto top = RectHelper::GetTop(rectangle);
		auto right = RectHelper::GetRight(rectangle);
		auto bottom = RectHelper::GetBottom(rectangle);

		Point leftTop = Media::MatrixHelper::Transform(matrix, Point(left, top));
		Point rightTop = Media::MatrixHelper::Transform(matrix, Point(right, top));
		Point leftBottom = Media::MatrixHelper::Transform(matrix, Point(left, bottom));
		Point rightBottom = Media::MatrixHelper::Transform(matrix, Point(right, bottom));

		left = std::min<float>(std::min<float>(leftTop.X, rightTop.X), std::min<float>(leftBottom.X, rightBottom.X));
		top = std::min<float>(std::min<float>(leftTop.Y, rightTop.Y), std::min<float>(leftBottom.Y, rightBottom.Y));
		right = std::max<float>(std::max<float>(leftTop.X, rightTop.X), std::max<float>(leftBottom.X, rightBottom.X));
		bottom = std::max<float>(std::max<float>(leftTop.Y, rightTop.Y), std::max<float>(leftBottom.Y, rightBottom.Y));

		auto width = right - left;
		auto height = bottom - top;
		return Rect(left, top, width, height);
	}
}
