#include "pch.h"
#include "ImageCropper.h"
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation::Numerics;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace Microsoft::UI::Xaml::Hosting;

	void ImageCropper::InitImageLayout(bool animate)
	{
		if (Source() != nullptr)
		{
			_restrictedCropRect = Rect(0, 0, static_cast<float>(Source().PixelWidth()), static_cast<float>(Source().PixelHeight()));
			if (IsValidRect(_restrictedCropRect))
			{
				_currentCroppedRect = KeepAspectRatio() ? GetUniformRect(_restrictedCropRect, ActualAspectRatio()) : _restrictedCropRect;

				if (TryUpdateImageLayout(animate))
				{
					UpdateSelectionThumbs(animate);
					UpdateMaskArea(animate);
				}
			}
		}

		UpdateThumbsVisibility();
	}

	bool ImageCropper::TryUpdateImageLayout(bool animate)
	{
		if (Source() != nullptr && IsValidRect(CanvasRect()))
		{
			auto uniformSelectedRect = GetUniformRect(CanvasRect(), _currentCroppedRect.Width / _currentCroppedRect.Height);

			return TryUpdateImageLayoutWithViewport(uniformSelectedRect, _currentCroppedRect, animate);
		}

		return false;
	}

	bool ImageCropper::TryUpdateImageLayoutWithViewport(Rect viewport, Rect viewportImageRect, bool animate)
	{
		if (!IsValidRect(viewport) || !IsValidRect(viewportImageRect))
		{
			return false;
		}

		auto imageScale = viewport.Width / viewportImageRect.Width;
		_imageTransform.ScaleY(imageScale);
		_imageTransform.ScaleX(imageScale);
		_imageTransform.TranslateX(viewport.X - (viewportImageRect.X * imageScale));
		_imageTransform.TranslateY(viewport.Y - (viewportImageRect.Y * imageScale));
		_inverseImageTransform.ScaleY(1 / imageScale);
		_inverseImageTransform.ScaleX(_inverseImageTransform.ScaleY());
		_inverseImageTransform.TranslateX(-_imageTransform.TranslateX() / imageScale);
		_inverseImageTransform.TranslateY(-_imageTransform.TranslateY() / imageScale);
		_restrictedSelectRect = _imageTransform.TransformBounds(_restrictedCropRect);

		if (animate)
		{
			AnimateUIElementOffset(Point(static_cast<float>(_imageTransform.TranslateX()), static_cast<float>(_imageTransform.TranslateY())), _animationDuration, _sourceImage);
			AnimateUIElementScale(imageScale, _animationDuration, _sourceImage);
		}
		else
		{
			auto targetVisual = ElementCompositionPreview::GetElementVisual(_sourceImage);
			targetVisual.Offset(float3(static_cast<float>(_imageTransform.TranslateX()), static_cast<float>(_imageTransform.TranslateY()), 0));
			targetVisual.Scale(float3(imageScale));
		}

		return true;
	}

	void ImageCropper::UpdateCroppedRect(ThumbPosition position, Point diffPos)
	{
		if ((diffPos.X == 0 && diffPos.X == 0) || !IsValidRect(CanvasRect()))
		{
			return;
		}

		double radian = 0, diffPointRadian = 0;
		if (KeepAspectRatio())
		{
			radian = std::atan(ActualAspectRatio());
			diffPointRadian = std::atan(diffPos.X / diffPos.Y);
		}

		auto startPoint = Point(_startX, _startY);
		auto endPoint = Point(_endX, _endY);
		auto currentSelectedRect = ToRect(startPoint, endPoint);
		switch (position)
		{
		case ThumbPosition::Top:
			if (KeepAspectRatio())
			{
				auto x = -diffPos.Y * ActualAspectRatio();
				auto originSizeChange = Point(static_cast<float>(x), -diffPos.Y);
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				startPoint.X += -safeChange.X / 2;
				endPoint.X += safeChange.X / 2;
				startPoint.Y += -safeChange.Y;
			}
			else
			{
				startPoint.Y += diffPos.Y;
			}

			break;
		case ThumbPosition::Bottom:
			if (KeepAspectRatio())
			{
				auto x = diffPos.Y * ActualAspectRatio();
				auto originSizeChange = Point(static_cast<float>(x), diffPos.Y);
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				startPoint.X += -safeChange.X / 2;
				endPoint.X += safeChange.X / 2;
				endPoint.Y += safeChange.Y;
			}
			else
			{
				endPoint.Y += diffPos.Y;
			}

			break;
		case ThumbPosition::Left:
			if (KeepAspectRatio())
			{
				auto y = -diffPos.X / ActualAspectRatio();
				auto originSizeChange = Point(-diffPos.X, static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				startPoint.Y += -safeChange.Y / 2;
				endPoint.Y += safeChange.Y / 2;
				startPoint.X += -safeChange.X;
			}
			else
			{
				startPoint.X += diffPos.X;
			}

			break;
		case ThumbPosition::Right:
			if (KeepAspectRatio())
			{
				auto y = diffPos.X / ActualAspectRatio();
				auto originSizeChange = Point(diffPos.X, static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				startPoint.Y += -safeChange.Y / 2;
				endPoint.Y += safeChange.Y / 2;
				endPoint.X += safeChange.X;
			}
			else
			{
				endPoint.X += diffPos.X;
			}

			break;
		case ThumbPosition::UpperLeft:
			if (KeepAspectRatio())
			{
				auto effectiveLength = diffPos.Y / std::cos(diffPointRadian) * std::cos(diffPointRadian - radian);
				auto x = -effectiveLength * std::sin(radian);
				auto y = -effectiveLength * std::cos(radian);
				auto originSizeChange = Point(static_cast<float>(x), static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				diffPos.X = -safeChange.X;
				diffPos.Y = -safeChange.Y;
			}

			startPoint.X += diffPos.X;
			startPoint.Y += diffPos.Y;
			break;
		case ThumbPosition::UpperRight:
			if (KeepAspectRatio())
			{
				diffPointRadian = -diffPointRadian;
				auto effectiveLength = diffPos.Y / std::cos(diffPointRadian) * std::cos(diffPointRadian - radian);
				auto x = -effectiveLength * std::sin(radian);
				auto y = -effectiveLength * std::cos(radian);
				auto originSizeChange = Point(static_cast<float>(x), static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				diffPos.X = safeChange.X;
				diffPos.Y = -safeChange.Y;
			}

			endPoint.X += diffPos.X;
			startPoint.Y += diffPos.Y;
			break;
		case ThumbPosition::LowerLeft:
			if (KeepAspectRatio())
			{
				diffPointRadian = -diffPointRadian;
				auto effectiveLength = diffPos.Y / std::cos(diffPointRadian) * std::cos(diffPointRadian - radian);
				auto x = effectiveLength * std::sin(radian);
				auto y = effectiveLength * std::cos(radian);
				auto originSizeChange = Point(static_cast<float>(x), static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				diffPos.X = -safeChange.X;
				diffPos.Y = safeChange.Y;
			}

			startPoint.X += diffPos.X;
			endPoint.Y += diffPos.Y;
			break;
		case ThumbPosition::LowerRight:
			if (KeepAspectRatio())
			{
				auto effectiveLength = diffPos.Y / std::cos(diffPointRadian) * std::cos(diffPointRadian - radian);
				auto x = effectiveLength * std::sin(radian);
				auto y = effectiveLength * std::cos(radian);
				auto originSizeChange = Point(static_cast<float>(x), static_cast<float>(y));
				auto safeChange = GetSafeSizeChangeWhenKeepAspectRatio(_restrictedSelectRect, position, currentSelectedRect, originSizeChange, ActualAspectRatio());
				diffPos.X = safeChange.X;
				diffPos.Y = safeChange.Y;
			}

			endPoint.X += diffPos.X;
			endPoint.Y += diffPos.Y;
			break;
		}

		if (!IsSafeRect(startPoint, endPoint, MinSelectSize()))
		{
			if (KeepAspectRatio())
			{
				if ((endPoint.Y - startPoint.Y) < (_endY - _startY) ||
					(endPoint.X - startPoint.X) < (_endX - _startX))
				{
					return;
				}
			}
			else
			{
				auto safeRect = GetSafeRect(startPoint, endPoint, MinSelectSize(), position);
				safeRect = RectHelper::Intersect(safeRect, _restrictedSelectRect);
				startPoint = Point(safeRect.X, safeRect.Y);
				endPoint = Point(safeRect.X + safeRect.Width, safeRect.Y + safeRect.Height);
			}
		}

		auto isEffectiveRegion = IsSafePoint(_restrictedSelectRect, startPoint) &&
			IsSafePoint(_restrictedSelectRect, endPoint);
		auto selectedRect = ToRect(startPoint, endPoint);
		if (!isEffectiveRegion)
		{
			if (!IsCornerThumb(position) && TryGetContainedRect(_restrictedSelectRect, selectedRect))
			{
				startPoint = Point(
					RectHelper::GetLeft(selectedRect),
					RectHelper::GetTop(selectedRect)
				);

				endPoint = Point(
					RectHelper::GetRight(selectedRect),
					RectHelper::GetBottom(selectedRect)
				);
			}
			else
			{
				return;
			}
		}

		selectedRect = RectHelper::Union(selectedRect, CanvasRect());
		if (selectedRect != CanvasRect())
		{
			auto croppedRect = _inverseImageTransform.TransformBounds(ToRect(startPoint, endPoint));
			croppedRect = RectHelper::Intersect(croppedRect, _restrictedCropRect);
			_currentCroppedRect = croppedRect;
			auto viewportRect = GetUniformRect(CanvasRect(), selectedRect.Width / selectedRect.Height);
			auto viewportImgRect = _inverseImageTransform.TransformBounds(selectedRect);

			if (TryUpdateImageLayoutWithViewport(viewportRect, viewportImgRect))
			{
				UpdateSelectionThumbs();
				UpdateMaskArea();
			}
		}
		else
		{
			UpdateSelectionThumbs(startPoint, endPoint);
			UpdateMaskArea();
		}
	}

	void ImageCropper::UpdateSelectionThumbs(bool animate)
	{
		auto selectedRect = _imageTransform.TransformBounds(_currentCroppedRect);
		auto startPoint = GetSafePoint(_restrictedSelectRect, Point(selectedRect.X, selectedRect.Y));
		auto endPoint = GetSafePoint(_restrictedSelectRect, Point(selectedRect.X + selectedRect.Width, selectedRect.Y + selectedRect.Height));

		UpdateSelectionThumbs(startPoint, endPoint, animate);
	}

	void ImageCropper::UpdateSelectionThumbs(Point startPoint, Point endPoint, bool animate)
	{
		_startX = startPoint.X;
		_startY = startPoint.Y;
		_endX = endPoint.X;
		_endY = endPoint.Y;
		auto center = SelectionAreaCenter();

		Storyboard storyboard{ nullptr };
		if (animate)
		{
			storyboard = Storyboard();
		}

		if (_topThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(center.X, _animationDuration, _topThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startY, _animationDuration, _topThumb, L"Y", true));
			}
			else
			{
				_topThumb.X(center.X);
				_topThumb.Y(_startY);
			}
		}

		if (_bottomThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(center.X, _animationDuration, _bottomThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endY, _animationDuration, _bottomThumb, L"Y", true));
			}
			else
			{
				_bottomThumb.X(center.X);
				_bottomThumb.Y(_endY);
			}
		}

		if (_leftThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startX, _animationDuration, _leftThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(center.Y, _animationDuration, _leftThumb, L"Y", true));
			}
			else
			{
				_leftThumb.X(_startX);
				_leftThumb.Y(center.Y);
			}
		}

		if (_rightThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endX, _animationDuration, _rightThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(center.Y, _animationDuration, _rightThumb, L"Y", true));
			}
			else
			{
				_rightThumb.X(_endX);
				_rightThumb.Y(center.Y);
			}
		}

		if (_upperLeftThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startX, _animationDuration, _upperLeftThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startY, _animationDuration, _upperLeftThumb, L"Y", true));
			}
			else
			{
				_upperLeftThumb.X(_startX);
				_upperLeftThumb.Y(_startY);
			}
		}

		if (_upperRightThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endX, _animationDuration, _upperRightThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startY, _animationDuration, _upperRightThumb, L"Y", true));
			}
			else
			{
				_upperRightThumb.X(_endX);
				_upperRightThumb.Y(_startY);
			}
		}

		if (_lowerLeftThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_startX, _animationDuration, _lowerLeftThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endY, _animationDuration, _lowerLeftThumb, L"Y", true));
			}
			else
			{
				_lowerLeftThumb.X(_startX);
				_lowerLeftThumb.Y(_endY);
			}
		}

		if (_lowerRightThumb != nullptr)
		{
			if (animate)
			{
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endX, _animationDuration, _lowerRightThumb, L"X", true));
				if (storyboard) storyboard.Children().Append(CreateDoubleAnimation(_endY, _animationDuration, _lowerRightThumb, L"Y", true));
			}
			else
			{
				_lowerRightThumb.X(_endX);
				_lowerRightThumb.Y(_endY);
			}
		}

		if (animate)
		{
			if (storyboard) storyboard.Begin();
		}
	}

	void ImageCropper::UpdateCropShape()
	{
		_maskAreaGeometryGroup.Children().Clear();
		_outerGeometry = RectangleGeometry();
		switch (CropShape())
		{
		case CropShape::Rectangular:
			_innerGeometry = RectangleGeometry();
			_overlayGeometry = RectangleGeometry();
			break;
		case CropShape::Circular:
			_innerGeometry = EllipseGeometry();
			_overlayGeometry = EllipseGeometry();
			break;
		}

		_maskAreaGeometryGroup.Children().Append(_outerGeometry);
		_maskAreaGeometryGroup.Children().Append(_innerGeometry);
		if (_overlayAreaPath != nullptr)
		{
			_overlayAreaPath.Data(_overlayGeometry);
		}
	}

	void ImageCropper::UpdateMaskArea(bool animate)
	{
		if (_layoutGrid == nullptr || _maskAreaGeometryGroup.Children().Size() < 2)
		{
			return;
		}

		_outerGeometry.Rect({
			static_cast<float>(-_layoutGrid.Padding().Left),
			static_cast<float>(-_layoutGrid.Padding().Top),
			static_cast<float>(_layoutGrid.ActualWidth()),
			static_cast<float>(_layoutGrid.ActualHeight())
			});

		switch (CropShape())
		{
		case CropShape::Rectangular:
		{
			auto updateRectangleGeometry = [&](RectangleGeometry rectangleGeometry)
				{
					auto to = ToRect(Point(_startX, _startY), Point(_endX, _endY));
					if (animate)
					{
						auto storyboard = Storyboard();
						storyboard.Children().Append(CreateRectangleAnimation(to, _animationDuration, rectangleGeometry, true));
						storyboard.Begin();
					}
					else
					{
						rectangleGeometry.Rect(to);
					}
				};
			if (auto innerRectangleGeometry = _innerGeometry.try_as<RectangleGeometry>())
			{
				updateRectangleGeometry(innerRectangleGeometry);
			}
			if (auto overlayRectangleGeometry = _overlayGeometry.try_as<RectangleGeometry>())
			{
				updateRectangleGeometry(overlayRectangleGeometry);
			}

			break;
		}
		case CropShape::Circular:
		{
			auto updateEllipseGeometry = [&](EllipseGeometry ellipseGeometry)
				{
					auto center = Point(((_endX - _startX) / 2) + _startX, ((_endY - _startY) / 2) + _startY);
					auto radiusX = (_endX - _startX) / 2;
					auto radiusY = (_endY - _startY) / 2;
					if (animate)
					{
						auto storyboard = Storyboard();
						storyboard.Children().Append(CreatePointAnimation(center, _animationDuration, ellipseGeometry, L"Center", true));
						storyboard.Children().Append(CreateDoubleAnimation(radiusX, _animationDuration, ellipseGeometry, L"RadiusX", true));
						storyboard.Children().Append(CreateDoubleAnimation(radiusY, _animationDuration, ellipseGeometry, L"RadiusY", true));
						storyboard.Begin();
					}
					else
					{
						ellipseGeometry.Center(center);
						ellipseGeometry.RadiusX(radiusX);
						ellipseGeometry.RadiusY(radiusY);
					}
				};
			if (auto innerEllipseGeometry = _innerGeometry.try_as<EllipseGeometry>())
			{
				updateEllipseGeometry(innerEllipseGeometry);
			}
			if (auto overlayEllipseGeometry = _overlayGeometry.try_as<EllipseGeometry>())
			{
				updateEllipseGeometry(overlayEllipseGeometry);
			}

			break;
		}
		}

		RectangleGeometry geometry;
		geometry.Rect({ 0, 0, static_cast<float>(_layoutGrid.ActualWidth()), static_cast<float>(_layoutGrid.ActualHeight()) });
		_layoutGrid.Clip(geometry);
	}

	bool ImageCropper::TryUpdateAspectRatio()
	{
		if (KeepAspectRatio() == false || Source() == nullptr || IsValidRect(_restrictedSelectRect) == false)
		{
			return false;
		}

		auto center = SelectionAreaCenter();
		auto restrictedMinLength = MinCroppedPixelLength * _imageTransform.ScaleX();
		auto maxSelectedLength = std::max<float>(_endX - _startX, _endY - _startY);
		auto viewRect = Rect(center.X - (maxSelectedLength / 2), center.Y - (maxSelectedLength / 2), maxSelectedLength, maxSelectedLength);

		auto uniformSelectedRect = GetUniformRect(viewRect, ActualAspectRatio());
		if (uniformSelectedRect.Width > _restrictedSelectRect.Width || uniformSelectedRect.Height > _restrictedSelectRect.Height)
		{
			uniformSelectedRect = GetUniformRect(_restrictedSelectRect, ActualAspectRatio());
		}

		// If selection area is smaller than allowed.
		if (uniformSelectedRect.Width < restrictedMinLength || uniformSelectedRect.Height < restrictedMinLength)
		{
			// Scale selection area to fit.
			auto scale = restrictedMinLength / std::min<float>(uniformSelectedRect.Width, uniformSelectedRect.Height);
			uniformSelectedRect.Width *= static_cast<float>(scale);
			uniformSelectedRect.Height *= static_cast<float>(scale);

			// If selection area is larger than allowed.
			if (uniformSelectedRect.Width > _restrictedSelectRect.Width || uniformSelectedRect.Height > _restrictedSelectRect.Height)
			{
				// Sentinal value. Equivelant to setting KeepAspectRatio to false. Causes AspectRatio to be recalculated.
				AspectRatio(-1);
				return false;
			}
		}

		// Fix positioning
		if (_restrictedSelectRect.X > uniformSelectedRect.X)
		{
			uniformSelectedRect.X += _restrictedSelectRect.X - uniformSelectedRect.X;
		}

		if (_restrictedSelectRect.Y > uniformSelectedRect.Y)
		{
			uniformSelectedRect.Y += _restrictedSelectRect.Y - uniformSelectedRect.Y;
		}

		// Fix size
		if ((_restrictedSelectRect.X + _restrictedSelectRect.Width) < (uniformSelectedRect.X + uniformSelectedRect.Width))
		{
			uniformSelectedRect.X += (_restrictedSelectRect.X + _restrictedSelectRect.Width) - (uniformSelectedRect.X + uniformSelectedRect.Width);
		}

		if ((_restrictedSelectRect.Y + _restrictedSelectRect.Height) < (uniformSelectedRect.Y + uniformSelectedRect.Height))
		{
			uniformSelectedRect.Y += (_restrictedSelectRect.Y + _restrictedSelectRect.Height) - (uniformSelectedRect.Y + uniformSelectedRect.Height);
		}

		// Apply transformation
		auto croppedRect = _inverseImageTransform.TransformBounds(uniformSelectedRect);
		croppedRect = RectHelper::Intersect(croppedRect, _restrictedCropRect);
		_currentCroppedRect = croppedRect;

		return true;
	}

	void ImageCropper::UpdateThumbsVisibility()
	{
		auto cornerThumbsVisibility = Visibility::Visible;
		auto otherThumbsVisibility = Visibility::Visible;
		switch (ThumbPlacement())
		{
		case ThumbPlacement::All:
			break;
		case ThumbPlacement::Corners:
			otherThumbsVisibility = Visibility::Collapsed;
			break;
		}

		switch (CropShape())
		{
		case CropShape::Rectangular:
			break;
		case CropShape::Circular:
			cornerThumbsVisibility = Visibility::Collapsed;
			otherThumbsVisibility = Visibility::Visible;
			break;
		}

		if (Source() == nullptr)
		{
			cornerThumbsVisibility = otherThumbsVisibility = Visibility::Collapsed;
		}

		if (_topThumb != nullptr)
		{
			_topThumb.Visibility(otherThumbsVisibility);
		}

		if (_bottomThumb != nullptr)
		{
			_bottomThumb.Visibility(otherThumbsVisibility);
		}

		if (_leftThumb != nullptr)
		{
			_leftThumb.Visibility(otherThumbsVisibility);
		}

		if (_rightThumb != nullptr)
		{
			_rightThumb.Visibility(otherThumbsVisibility);
		}

		if (_upperLeftThumb != nullptr)
		{
			_upperLeftThumb.Visibility(cornerThumbsVisibility);
		}

		if (_upperRightThumb != nullptr)
		{
			_upperRightThumb.Visibility(cornerThumbsVisibility);
		}

		if (_lowerLeftThumb != nullptr)
		{
			_lowerLeftThumb.Visibility(cornerThumbsVisibility);
		}

		if (_lowerRightThumb != nullptr)
		{
			_lowerRightThumb.Visibility(cornerThumbsVisibility);
		}
	}

	Point ImageCropper::SelectionAreaCenter() { return Point(((_endX - _startX) / 2) + _startX, ((_endY - _startY) / 2) + _startY); }
}