#include "pch.h"
#include "ImageCropper.h"
#include "ImageCropper.h"
#if __has_include("ImageCropper.g.cpp")
#include "ImageCropper.g.cpp"
#endif
#include <winrt/Windows.Storage.Streams.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	ImageCropper::ImageCropper()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		_maskAreaGeometryGroup.FillRule(FillRule::EvenOdd);
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
	Size ImageCropper::MinCropSize()
	{
		auto aspectRatio = KeepAspectRatio() ? ActualAspectRatio() : 1;
		auto size = Size(static_cast<float>(MinCroppedPixelLength), static_cast<float>(MinCroppedPixelLength));
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
	Size ImageCropper::MinSelectSize()
	{
		auto realMinSelectSize = _imageTransform.TransformBounds(ToRect(MinCropSize()));
		auto minLength = std::min<float>(realMinSelectSize.Width, realMinSelectSize.Height);
		if (minLength < MinSelectedLength)
		{
			auto aspectRatio = KeepAspectRatio() ? ActualAspectRatio() : 1;
			auto minSelectSize = Size(static_cast<float>(MinSelectedLength), static_cast<float>(MinSelectedLength));
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

		return Size(realMinSelectSize.Width, realMinSelectSize.Height);
	}

	void ImageCropper::OnApplyTemplate()
	{
		UnhookEvents();
		_layoutGrid = GetTemplateChild(LayoutGridName).try_as<Grid>();
		_imageCanvas = GetTemplateChild(ImageCanvasPartName).try_as<Canvas>();
		_sourceImage = GetTemplateChild(SourceImagePartName).try_as<Image>();
		_maskAreaPath = GetTemplateChild(MaskAreaPathPartName).try_as<Path>();
		_overlayAreaPath = GetTemplateChild(OverlayAreaPathPartName).try_as<Path>();
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
		if (_imageCanvas != nullptr)
		{
			_imageCanvasSizeChangedToken = _imageCanvas.SizeChanged({ get_weak(), &ImageCropper::ImageCanvas_SizeChanged });
		}

		if (_sourceImage != nullptr)
		{
			_sourceImage.ManipulationMode(ManipulationModes::TranslateX | ManipulationModes::TranslateY);
			_sourceImageManipulationDeltaToken = _sourceImage.ManipulationDelta({ get_weak(), &ImageCropper::SourceImage_ManipulationDelta });
		}

		if (_maskAreaPath != nullptr)
		{
			_maskAreaPath.Data(_maskAreaGeometryGroup);
		}

		if (_overlayAreaPath != nullptr)
		{
			_overlayAreaPath.Data(_overlayGeometry);
		}

		if (_topThumb != nullptr)
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

		if (_bottomThumb != nullptr)
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

		if (_leftThumb != nullptr)
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

		if (_rightThumb != nullptr)
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

		if (_upperLeftThumb != nullptr)
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

		if (_upperRightThumb != nullptr)
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

		if (_lowerLeftThumb != nullptr)
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

		if (_lowerRightThumb != nullptr)
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
		if (_imageCanvas != nullptr)
		{
			_imageCanvas.SizeChanged(_imageCanvasSizeChangedToken);
		}

		if (_sourceImage != nullptr)
		{
			_sourceImage.ManipulationDelta(_sourceImageManipulationDeltaToken);
		}

		if (_maskAreaPath != nullptr)
		{
			_maskAreaPath.Data(nullptr);
		}

		if (_topThumb != nullptr)
		{
			_topThumb.ManipulationDelta(_topThumbManipulationDeltaToken);
			_topThumb.ManipulationCompleted(_topThumbManipulationCompletedToken);
			_topThumb.KeyDown(_topThumbKeyDownToken);
			_topThumb.KeyUp(_topThumbKeyUpToken);
		}

		if (_bottomThumb != nullptr)
		{
			_bottomThumb.ManipulationDelta(_bottomThumbManipulationDeltaToken);
			_bottomThumb.ManipulationCompleted(_bottomThumbManipulationCompletedToken);
			_bottomThumb.KeyDown(_bottomThumbKeyDownToken);
			_bottomThumb.KeyUp(_bottomThumbKeyUpToken);
		}

		if (_leftThumb != nullptr)
		{
			_leftThumb.ManipulationDelta(_leftThumbManipulationDeltaToken);
			_leftThumb.ManipulationCompleted(_leftThumbManipulationCompletedToken);
			_leftThumb.KeyDown(_leftThumbKeyDownToken);
			_leftThumb.KeyUp(_leftThumbKeyUpToken);
		}

		if (_rightThumb != nullptr)
		{
			_rightThumb.ManipulationDelta(_rightThumbManipulationDeltaToken);
			_rightThumb.ManipulationCompleted(_rightThumbManipulationCompletedToken);
			_rightThumb.KeyDown(_rightThumbKeyDownToken);
			_rightThumb.KeyUp(_rightThumbKeyUpToken);
		}

		if (_upperLeftThumb != nullptr)
		{
			_upperLeftThumb.ManipulationDelta(_upperLeftThumbManipulationDeltaToken);
			_upperLeftThumb.ManipulationCompleted(_upperLeftThumbManipulationCompletedToken);
			_upperLeftThumb.KeyDown(_upperLeftThumbKeyDownToken);
			_upperLeftThumb.KeyUp(_upperLeftThumbKeyUpToken);
		}

		if (_upperRightThumb != nullptr)
		{
			_upperRightThumb.ManipulationDelta(_upperRightThumbManipulationDeltaToken);
			_upperRightThumb.ManipulationCompleted(_upperRightThumbManipulationCompletedToken);
			_upperRightThumb.KeyDown(_upperRightThumbKeyDownToken);
			_upperRightThumb.KeyUp(_upperRightThumbKeyUpToken);
		}

		if (_lowerLeftThumb != nullptr)
		{
			_lowerLeftThumb.ManipulationDelta(_lowerLeftThumbManipulationDeltaToken);
			_lowerLeftThumb.ManipulationCompleted(_lowerLeftThumbManipulationCompletedToken);
			_lowerLeftThumb.KeyDown(_lowerLeftThumbKeyDownToken);
			_lowerLeftThumb.KeyUp(_lowerLeftThumbKeyUpToken);
		}

		if (_lowerRightThumb != nullptr)
		{
			_lowerRightThumb.ManipulationDelta(_lowerRightThumbManipulationDeltaToken);
			_lowerRightThumb.ManipulationCompleted(_lowerRightThumbManipulationCompletedToken);
			_lowerRightThumb.KeyDown(_lowerRightThumbKeyDownToken);
			_lowerRightThumb.KeyUp(_lowerRightThumbKeyUpToken);
		}
	}

	Size ImageCropper::MeasureOverride(Size availableSize)
	{
		if (Source() == nullptr || Source().PixelWidth() == 0 || Source().PixelHeight() == 0)
		{
			return base_type::MeasureOverride(availableSize);
		}

		if (std::isinf(availableSize.Width) || std::isinf(availableSize.Height))
		{
			if (!std::isinf(availableSize.Width))
			{
				availableSize.Height = availableSize.Width / Source().PixelWidth() * Source().PixelHeight();
			}
			else if (!std::isinf(availableSize.Height))
			{
				availableSize.Width = availableSize.Height / Source().PixelHeight() * Source().PixelWidth();
			}
			else
			{
				availableSize.Width = static_cast<float>(Source().PixelWidth());
				availableSize.Height = static_cast<float>(Source().PixelHeight());
			}

			base_type::MeasureOverride(availableSize);
			return availableSize;
		}

		return base_type::MeasureOverride(availableSize);
	}

	winrt::Windows::Foundation::IAsyncAction ImageCropper::LoadImageFromFile(winrt::Windows::Storage::StorageFile const& imageFile)
	{
		const auto writeableBitmap = WriteableBitmap(1, 1);
		const auto& stream = co_await imageFile.OpenReadAsync();
		co_await writeableBitmap.SetSourceAsync(stream);

		Source(writeableBitmap);
	}

	winrt::Windows::Foundation::IAsyncAction ImageCropper::SaveAsync(IRandomAccessStream const& stream, BitmapFileFormat bitmapFileFormat, bool keepRectangularOutput)
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

	bool ImageCropper::TrySetCroppedRegion(Rect rect)
	{
		// Reject regions smaller than the minimum size
		if (rect.Width < MinCropSize().Width || rect.Height < MinCropSize().Height)
		{
			return false;
		}

		// Reject regions that are not contained in the original picture
		if (RectHelper::GetLeft(rect) < RectHelper::GetLeft(_restrictedCropRect)
			|| RectHelper::GetTop(rect) < RectHelper::GetTop(_restrictedCropRect)
			|| RectHelper::GetRight(rect) > RectHelper::GetRight(_restrictedCropRect)
			|| RectHelper::GetBottom(rect) > RectHelper::GetBottom(_restrictedCropRect))
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
