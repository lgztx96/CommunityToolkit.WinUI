#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <optional>
#endif
#include "ImageCropper.h"
#if __has_include("ImageCropper.g.cpp")
#include "ImageCropper.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	ImageCropper::ImageCropper()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		_maskAreaGeometryGroup.FillRule(winrt::FillRule::EvenOdd);
	}

	/// <summary>
	/// Gets the internally used aspect ratio, rather than the user-provided value. Adjusted to handle crop shape and invalid values.
	/// </summary>
	double ImageCropper::ActualAspectRatio()
	{
		std::optional<double> aspectRatio;
		switch (CropShape())
		{
		case CropShape::Rectangular:
			aspectRatio = AspectRatio();
			break;
		case CropShape::Circular:
			aspectRatio = 1.0;
			break;
		default:
			aspectRatio = AspectRatio();
			break;
		}

		if (aspectRatio && aspectRatio > 0)
		{
			// When not null or 0.
			return aspectRatio.value();
		}
		else
		{
			// Fallback to sentinal value.
			// Used to indicate aspect ratio should be discarded and reset during manipulation of the image cropper.
			return -1;
		}
	}

	/// <summary>
	/// Gets the minimum cropped size.
	/// </summary>
	winrt::Size ImageCropper::MinCropSize()
	{
		auto aspectRatio = KeepAspectRatio() ? ActualAspectRatio() : 1;
		winrt::Size size(static_cast<float>(MinCroppedPixelLength), static_cast<float>(MinCroppedPixelLength));
		if (aspectRatio >= 1)
		{
			size.Width = static_cast<float>(size.Height * aspectRatio);
		}
		else
		{
			size.Height = static_cast<float>(size.Width / aspectRatio);
		}

		return size;
	}

	/// <summary>
	/// Gets the minimum selectable size.
	/// </summary>
	winrt::Size ImageCropper::MinSelectSize()
	{
		auto realMinSelectSize = _imageTransform.TransformBounds(ToRect(MinCropSize()));
		auto minLength = std::min<float>(realMinSelectSize.Width, realMinSelectSize.Height);
		if (minLength < MinSelectedLength)
		{
			auto aspectRatio = KeepAspectRatio() ? ActualAspectRatio() : 1;
			winrt::Size minSelectSize(static_cast<float>(MinSelectedLength), static_cast<float>(MinSelectedLength));
			if (aspectRatio >= 1)
			{
				minSelectSize.Width = static_cast<float>(minSelectSize.Height * aspectRatio);
			}
			else
			{
				minSelectSize.Height = static_cast<float>(minSelectSize.Width / aspectRatio);
			}

			return minSelectSize;
		}

		return winrt::Size(realMinSelectSize.Width, realMinSelectSize.Height);
	}

	void ImageCropper::OnApplyTemplate()
	{
		UnhookEvents();
		_layoutGrid = GetTemplateChild(LayoutGridName).try_as<winrt::Grid>();
		_imageCanvas = GetTemplateChild(ImageCanvasPartName).try_as<winrt::Canvas>();
		_sourceImage = GetTemplateChild(SourceImagePartName).try_as<winrt::Image>();
		_maskAreaPath = GetTemplateChild(MaskAreaPathPartName).try_as<winrt::Path>();
		_overlayAreaPath = GetTemplateChild(OverlayAreaPathPartName).try_as<winrt::Path>();
		_topThumb = GetTemplateChild(TopThumbPartName).try_as<Controls::ImageCropperThumb>();
		_bottomThumb = GetTemplateChild(BottomThumbPartName).try_as<Controls::ImageCropperThumb>();
		_leftThumb = GetTemplateChild(LeftThumbPartName).try_as<Controls::ImageCropperThumb>();
		_rightThumb = GetTemplateChild(RightThumbPartName).try_as<Controls::ImageCropperThumb>();
		_upperLeftThumb = GetTemplateChild(UpperLeftThumbPartName).try_as<Controls::ImageCropperThumb>();
		_upperRightThumb = GetTemplateChild(UpperRightThumbPartName).try_as<Controls::ImageCropperThumb>();
		_lowerLeftThumb = GetTemplateChild(LowerLeftThumbPartName).try_as<Controls::ImageCropperThumb>();
		_lowerRightThumb = GetTemplateChild(LowerRightThumbPartName).try_as<Controls::ImageCropperThumb>();
		HookUpEvents();
		UpdateThumbsVisibility();
	}

	void ImageCropper::HookUpEvents()
	{
		if (_imageCanvas)
		{
			_imageCanvasSizeChangedToken = _imageCanvas.SizeChanged({ get_weak(), &ImageCropper::ImageCanvas_SizeChanged });
		}

		if (_sourceImage)
		{
			_sourceImage.ManipulationMode(winrt::ManipulationModes::TranslateX | winrt::ManipulationModes::TranslateY);
			_sourceImageManipulationDeltaToken = _sourceImage.ManipulationDelta({ get_weak(), &ImageCropper::SourceImage_ManipulationDelta });
		}

		if (_maskAreaPath)
		{
			_maskAreaPath.Data(_maskAreaGeometryGroup);
		}

		if (_overlayAreaPath)
		{
			_overlayAreaPath.Data(_overlayGeometry);
		}

		if (_topThumb)
		{
			_topThumb.Position(ThumbPosition::Top);
			_topThumbManipulationDeltaToken = _topThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_topThumbManipulationCompletedToken = _topThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_topThumbKeyDownToken = _topThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_topThumbKeyUpToken = _topThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_bottomThumb)
		{
			_bottomThumb.Position(ThumbPosition::Bottom);
			_bottomThumbManipulationDeltaToken = _bottomThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_bottomThumbManipulationCompletedToken = _bottomThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_bottomThumbKeyDownToken = _bottomThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_bottomThumbKeyUpToken = _bottomThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_leftThumb)
		{
			_leftThumb.Position(ThumbPosition::Left);
			_leftThumbManipulationDeltaToken = _leftThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_leftThumbManipulationCompletedToken = _leftThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_leftThumbKeyDownToken = _leftThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_leftThumbKeyUpToken = _leftThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_rightThumb)
		{
			_rightThumb.Position(ThumbPosition::Right);
			_rightThumbManipulationDeltaToken = _rightThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_rightThumbManipulationCompletedToken = _rightThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_rightThumbKeyDownToken = _rightThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_rightThumbKeyUpToken = _rightThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_upperLeftThumb)
		{
			_upperLeftThumb.Position(ThumbPosition::UpperLeft);
			_upperLeftThumbManipulationDeltaToken = _upperLeftThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_upperLeftThumbManipulationCompletedToken = _upperLeftThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_upperLeftThumbKeyDownToken = _upperLeftThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_upperLeftThumbKeyUpToken = _upperLeftThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_upperRightThumb)
		{
			_upperRightThumb.Position(ThumbPosition::UpperRight);
			_upperRightThumbManipulationDeltaToken = _upperRightThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_upperRightThumbManipulationCompletedToken = _upperRightThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_upperRightThumbKeyDownToken = _upperRightThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_upperRightThumbKeyUpToken = _upperRightThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_lowerLeftThumb)
		{
			_lowerLeftThumb.Position(ThumbPosition::LowerLeft);
			_lowerLeftThumbManipulationDeltaToken = _lowerLeftThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_lowerLeftThumbManipulationCompletedToken = _lowerLeftThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_lowerLeftThumbKeyDownToken = _lowerLeftThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_lowerLeftThumbKeyUpToken = _lowerLeftThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}

		if (_lowerRightThumb)
		{
			_lowerRightThumb.Position(ThumbPosition::LowerRight);
			_lowerRightThumbManipulationDeltaToken = _lowerRightThumb.ManipulationDelta(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationDelta });
			_lowerRightThumbManipulationCompletedToken = _lowerRightThumb.ManipulationCompleted(
				{ get_weak(), &ImageCropper::ImageCropperThumb_ManipulationCompleted });
			_lowerRightThumbKeyDownToken = _lowerRightThumb.KeyDown(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyDown });
			_lowerRightThumbKeyUpToken = _lowerRightThumb.KeyUp(
				{ get_weak(), &ImageCropper::ImageCropperThumb_KeyUp });
		}
	}

	void ImageCropper::UnhookEvents()
	{
		if (_imageCanvas)
		{
			_imageCanvas.SizeChanged(_imageCanvasSizeChangedToken);
		}

		if (_sourceImage)
		{
			_sourceImage.ManipulationDelta(_sourceImageManipulationDeltaToken);
		}

		if (_maskAreaPath)
		{
			_maskAreaPath.Data(nullptr);
		}

		if (_topThumb)
		{
			_topThumb.ManipulationDelta(_topThumbManipulationDeltaToken);
			_topThumb.ManipulationCompleted(_topThumbManipulationCompletedToken);
			_topThumb.KeyDown(_topThumbKeyDownToken);
			_topThumb.KeyUp(_topThumbKeyUpToken);
		}

		if (_bottomThumb)
		{
			_bottomThumb.ManipulationDelta(_bottomThumbManipulationDeltaToken);
			_bottomThumb.ManipulationCompleted(_bottomThumbManipulationCompletedToken);
			_bottomThumb.KeyDown(_bottomThumbKeyDownToken);
			_bottomThumb.KeyUp(_bottomThumbKeyUpToken);
		}

		if (_leftThumb)
		{
			_leftThumb.ManipulationDelta(_leftThumbManipulationDeltaToken);
			_leftThumb.ManipulationCompleted(_leftThumbManipulationCompletedToken);
			_leftThumb.KeyDown(_leftThumbKeyDownToken);
			_leftThumb.KeyUp(_leftThumbKeyUpToken);
		}

		if (_rightThumb)
		{
			_rightThumb.ManipulationDelta(_rightThumbManipulationDeltaToken);
			_rightThumb.ManipulationCompleted(_rightThumbManipulationCompletedToken);
			_rightThumb.KeyDown(_rightThumbKeyDownToken);
			_rightThumb.KeyUp(_rightThumbKeyUpToken);
		}

		if (_upperLeftThumb)
		{
			_upperLeftThumb.ManipulationDelta(_upperLeftThumbManipulationDeltaToken);
			_upperLeftThumb.ManipulationCompleted(_upperLeftThumbManipulationCompletedToken);
			_upperLeftThumb.KeyDown(_upperLeftThumbKeyDownToken);
			_upperLeftThumb.KeyUp(_upperLeftThumbKeyUpToken);
		}

		if (_upperRightThumb)
		{
			_upperRightThumb.ManipulationDelta(_upperRightThumbManipulationDeltaToken);
			_upperRightThumb.ManipulationCompleted(_upperRightThumbManipulationCompletedToken);
			_upperRightThumb.KeyDown(_upperRightThumbKeyDownToken);
			_upperRightThumb.KeyUp(_upperRightThumbKeyUpToken);
		}

		if (_lowerLeftThumb)
		{
			_lowerLeftThumb.ManipulationDelta(_lowerLeftThumbManipulationDeltaToken);
			_lowerLeftThumb.ManipulationCompleted(_lowerLeftThumbManipulationCompletedToken);
			_lowerLeftThumb.KeyDown(_lowerLeftThumbKeyDownToken);
			_lowerLeftThumb.KeyUp(_lowerLeftThumbKeyUpToken);
		}

		if (_lowerRightThumb)
		{
			_lowerRightThumb.ManipulationDelta(_lowerRightThumbManipulationDeltaToken);
			_lowerRightThumb.ManipulationCompleted(_lowerRightThumbManipulationCompletedToken);
			_lowerRightThumb.KeyDown(_lowerRightThumbKeyDownToken);
			_lowerRightThumb.KeyUp(_lowerRightThumbKeyUpToken);
		}
	}

	winrt::Size ImageCropper::MeasureOverride(winrt::Size availableSize)
	{
		auto source = Source();
		if (source == nullptr || source.PixelWidth() == 0 || source.PixelHeight() == 0)
		{
			return base_type::MeasureOverride(availableSize);
		}

		if (std::isinf(availableSize.Width) || std::isinf(availableSize.Height))
		{
			if (!std::isinf(availableSize.Width))
			{
				availableSize.Height = availableSize.Width / source.PixelWidth() * source.PixelHeight();
			}
			else if (!std::isinf(availableSize.Height))
			{
				availableSize.Width = availableSize.Height / source.PixelHeight() * source.PixelWidth();
			}
			else
			{
				availableSize.Width = static_cast<float>(source.PixelWidth());
				availableSize.Height = static_cast<float>(source.PixelHeight());
			}

			base_type::MeasureOverride(availableSize);
			return availableSize;
		}

		return base_type::MeasureOverride(availableSize);
	}

	winrt::IAsyncAction ImageCropper::LoadImageFromFile(winrt::StorageFile const& imageFile)
	{
		auto strongThis = get_strong();

		const auto writeableBitmap = winrt::WriteableBitmap(1, 1);
		const auto& stream = co_await imageFile.OpenReadAsync();
		co_await writeableBitmap.SetSourceAsync(stream);

		Source(writeableBitmap);
	}

	winrt::IAsyncAction ImageCropper::SaveAsync(winrt::IRandomAccessStream const& stream, winrt::BitmapFileFormat bitmapFileFormat, bool keepRectangularOutput)
	{
		if (Source() == nullptr)
		{
			co_return;
		}

		if (keepRectangularOutput || CropShape() == CropShape::Rectangular)
		{
			co_await CropImageAsync(Source(), stream, _currentCroppedRect, bitmapFileFormat);
			co_return;
		}

		co_await CropImageWithShapeAsync(Source(), stream, _currentCroppedRect, bitmapFileFormat, CropShape());
	}

	void ImageCropper::Reset()
	{
		InitImageLayout(true);
	}

	bool ImageCropper::TrySetCroppedRegion(winrt::Rect rect)
	{
		// Reject regions smaller than the minimum size
		if (rect.Width < MinCropSize().Width || rect.Height < MinCropSize().Height)
		{
			return false;
		}

		// Reject regions that are not contained in the original picture
		if (winrt::RectHelper::GetLeft(rect) < winrt::RectHelper::GetLeft(_restrictedCropRect)
			|| winrt::RectHelper::GetTop(rect) < winrt::RectHelper::GetTop(_restrictedCropRect)
			|| winrt::RectHelper::GetRight(rect) > winrt::RectHelper::GetRight(_restrictedCropRect)
			|| winrt::RectHelper::GetBottom(rect) > winrt::RectHelper::GetBottom(_restrictedCropRect))
		{
			return false;
		}

		// If an aspect ratio is set, reject regions that don't respect it
		// If cropping a circle, reject regions where the aspect ratio is not 1
		if (KeepAspectRatio() && ActualAspectRatio() != rect.Width / rect.Height)
		{
			return false;
		}

		_currentCroppedRect = rect;
		if (TryUpdateImageLayout(true))
		{
			UpdateSelectionThumbs(true);
			UpdateMaskArea(true);
		}

		return true;
	}
}
