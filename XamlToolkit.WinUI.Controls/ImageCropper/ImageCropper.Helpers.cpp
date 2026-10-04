#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <algorithm>
#include <cmath>
#else
import winrt.Microsoft.Graphics.Canvas;
import winrt.Microsoft.Graphics.Canvas.Effects;
import winrt.Microsoft.Graphics.Canvas.Geometry;
#endif
#include "ImageCropper.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::Foundation::Numerics;
	using namespace Microsoft::Graphics::Canvas;
	using namespace Microsoft::Graphics::Canvas::Effects;
	using namespace Microsoft::Graphics::Canvas::Geometry;
	using namespace Windows::Graphics::Imaging;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	static constexpr double ThresholdValue = 0.001;

	static winrt::BitmapBounds GetBitmapBounds(winrt::Rect const& croppedRect, uint32_t pixelWidth, uint32_t pixelHeight)
	{
		const auto left = static_cast<uint32_t>(std::floor(std::max<float>(croppedRect.X, 0)));
		const auto top = static_cast<uint32_t>(std::floor(std::max<float>(croppedRect.Y, 0)));
		const auto right = std::min<uint32_t>(static_cast<uint32_t>(std::ceil(croppedRect.X + croppedRect.Width)), pixelWidth);
		const auto bottom = std::min<uint32_t>(static_cast<uint32_t>(std::ceil(croppedRect.Y + croppedRect.Height)), pixelHeight);

		return winrt::BitmapBounds
		{
			.X = left,
			.Y = top,
			.Width = right > left ? right - left : 0,
			.Height = bottom > top ? bottom - top : 0
		};
	}

	winrt::IAsyncAction ImageCropper::CropImageAsync(winrt::IRandomAccessStream sourceStream, winrt::IRandomAccessStream stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat)
	{
		sourceStream.Seek(0);

		const auto decoder = co_await winrt::BitmapDecoder::CreateAsync(sourceStream);
		const auto frame = co_await decoder.GetFrameAsync(0);

		const auto bounds = GetBitmapBounds(croppedRect, frame.OrientedPixelWidth(), frame.OrientedPixelHeight());

		winrt::BitmapTransform transform;
		transform.Bounds(bounds);

		const auto pixelData = co_await frame.GetPixelDataAsync(
			winrt::BitmapPixelFormat::Bgra8,
			winrt::BitmapAlphaMode::Premultiplied,
			transform,
			winrt::ExifOrientationMode::RespectExifOrientation,
			winrt::ColorManagementMode::ColorManageToSRgb);

		const auto bytes = pixelData.DetachPixelData();

		const auto& bitmapEncoder = co_await winrt::BitmapEncoder::CreateAsync(GetEncoderId(bitmapFileFormat), stream);
		bitmapEncoder.SetPixelData(winrt::BitmapPixelFormat::Bgra8, winrt::BitmapAlphaMode::Premultiplied, bounds.Width, bounds.Height, 96.0, 96.0, bytes);
		co_await bitmapEncoder.FlushAsync();
	}

	winrt::IAsyncAction ImageCropper::CropImageWithShapeAsync(winrt::IRandomAccessStream croppedRegion, winrt::IRandomAccessStream stream, BitmapFileFormat bitmapFileFormat, Controls::CropShape cropShape)
	{
		auto device = winrt::CanvasDevice::GetSharedDevice();

		// WinUI3/Win2D bug: switch back to CanvasBitmap once it works.
		auto sourceBitmap = co_await winrt::CanvasVirtualBitmap::LoadAsync(device, croppedRegion);
		if (sourceBitmap == nullptr)
		{
			co_return;
		}

		const auto croppedSize = sourceBitmap.SizeInPixels();
		const auto croppedWidth = static_cast<float>(croppedSize.Width);
		const auto croppedHeight = static_cast<float>(croppedSize.Height);

		auto clipGeometry = CreateClipGeometry(device, cropShape, winrt::Size(croppedWidth, croppedHeight));
		if (clipGeometry == nullptr)
		{
			co_return;
		}

		winrt::CanvasRenderTarget offScreen(device, croppedWidth, croppedHeight, 96.0f);
		auto drawingSession = offScreen.CreateDrawingSession();
		winrt::CanvasCommandList markCommandList(device);

		auto markDrawingSession = markCommandList.CreateDrawingSession();
		markDrawingSession.FillGeometry(clipGeometry, winrt::Microsoft::UI::Colors::Black());

		winrt::AlphaMaskEffect alphaMaskEffect;
		alphaMaskEffect.Source(sourceBitmap);
		alphaMaskEffect.AlphaMask(markCommandList);

		drawingSession.DrawImage(alphaMaskEffect);
		drawingSession.Close();

		auto pixelBytes = offScreen.GetPixelBytes();
		auto bitmapEncoder = co_await winrt::BitmapEncoder::CreateAsync(GetEncoderId(bitmapFileFormat), stream);
		bitmapEncoder.SetPixelData(winrt::BitmapPixelFormat::Bgra8, winrt::BitmapAlphaMode::Premultiplied, offScreen.SizeInPixels().Width, offScreen.SizeInPixels().Height, 96.0, 96.0, pixelBytes);
		co_await bitmapEncoder.FlushAsync();
	}

	winrt::CanvasGeometry ImageCropper::CreateClipGeometry(winrt::ICanvasResourceCreator resourceCreator, Controls::CropShape cropShape, winrt::Size croppedSize)
	{
		switch (cropShape)
		{
		case CropShape::Rectangular:
			break;
		case CropShape::Circular:
			float radiusX = croppedSize.Width / 2;
			float radiusY = croppedSize.Height / 2;
			return winrt::CanvasGeometry::CreateEllipse(resourceCreator, winrt::float2(radiusX, radiusY), radiusX, radiusY);
		}

		return nullptr;
	}

	winrt::guid ImageCropper::GetEncoderId(winrt::BitmapFileFormat bitmapFileFormat)
	{
		switch (bitmapFileFormat)
		{
		case winrt::BitmapFileFormat::Bmp:
			return BitmapEncoder::BmpEncoderId();
		case winrt::BitmapFileFormat::Png:
			return BitmapEncoder::PngEncoderId();
		case winrt::BitmapFileFormat::Jpeg:
			return BitmapEncoder::JpegEncoderId();
		case winrt::BitmapFileFormat::Tiff:
			return BitmapEncoder::TiffEncoderId();
		case winrt::BitmapFileFormat::Gif:
			return BitmapEncoder::GifEncoderId();
		case winrt::BitmapFileFormat::JpegXR:
			return BitmapEncoder::JpegXREncoderId();
		}

		return winrt::BitmapEncoder::PngEncoderId();
	}

	winrt::Point ImageCropper::GetSafePoint(winrt::Rect targetRect, winrt::Point point)
	{
		winrt::Point safePoint(point.X, point.Y);
		if (safePoint.X < targetRect.X)
		{
			safePoint.X = targetRect.X;
		}

		if (safePoint.X > targetRect.X + targetRect.Width)
		{
			safePoint.X = targetRect.X + targetRect.Width;
		}

		if (safePoint.Y < targetRect.Y)
		{
			safePoint.Y = targetRect.Y;
		}

		if (safePoint.Y > targetRect.Y + targetRect.Height)
		{
			safePoint.Y = targetRect.Y + targetRect.Height;
		}

		return safePoint;
	}

	bool ImageCropper::IsSafePoint(winrt::Rect targetRect, winrt::Point point)
	{
		if (point.X - targetRect.X < -ThresholdValue)
		{
			return false;
		}

		if (point.X - (targetRect.X + targetRect.Width) > ThresholdValue)
		{
			return false;
		}

		if (point.Y - targetRect.Y < -ThresholdValue)
		{
			return false;
		}

		if (point.Y - (targetRect.Y + targetRect.Height) > ThresholdValue)
		{
			return false;
		}

		return true;
	}

	bool ImageCropper::IsSafeRect(winrt::Point startPoint, winrt::Point endPoint, winrt::Size minSize)
	{
		winrt::Point checkPoint(startPoint.X + minSize.Width, startPoint.Y + minSize.Height);
		return checkPoint.X - endPoint.X < ThresholdValue
			&& checkPoint.Y - endPoint.Y < ThresholdValue;
	}

	winrt::Rect ImageCropper::GetSafeRect(winrt::Point startPoint, winrt::Point endPoint, winrt::Size minSize, ThumbPosition position)
	{
		winrt::Point checkPoint(startPoint.X + minSize.Width, startPoint.Y + minSize.Height);
		switch (position)
		{
		case ThumbPosition::Top:
			if (checkPoint.Y > endPoint.Y)
			{
				startPoint.Y = endPoint.Y - minSize.Height;
			}

			break;
		case ThumbPosition::Bottom:
			if (checkPoint.Y > endPoint.Y)
			{
				endPoint.Y = startPoint.Y + minSize.Height;
			}

			break;
		case ThumbPosition::Left:
			if (checkPoint.X > endPoint.X)
			{
				startPoint.X = endPoint.X - minSize.Width;
			}

			break;
		case ThumbPosition::Right:
			if (checkPoint.X > endPoint.X)
			{
				endPoint.X = startPoint.X + minSize.Width;
			}

			break;
		case ThumbPosition::UpperLeft:
			if (checkPoint.X > endPoint.X)
			{
				startPoint.X = endPoint.X - minSize.Width;
			}

			if (checkPoint.Y > endPoint.Y)
			{
				startPoint.Y = endPoint.Y - minSize.Height;
			}

			break;
		case ThumbPosition::UpperRight:
			if (checkPoint.X > endPoint.X)
			{
				endPoint.X = startPoint.X + minSize.Width;
			}

			if (checkPoint.Y > endPoint.Y)
			{
				startPoint.Y = endPoint.Y - minSize.Height;
			}

			break;
		case ThumbPosition::LowerLeft:
			if (checkPoint.X > endPoint.X)
			{
				startPoint.X = endPoint.X - minSize.Width;
			}

			if (checkPoint.Y > endPoint.Y)
			{
				endPoint.Y = startPoint.Y + minSize.Height;
			}

			break;
		case ThumbPosition::LowerRight:
			if (checkPoint.X > endPoint.X)
			{
				endPoint.X = startPoint.X + minSize.Width;
			}

			if (checkPoint.Y > endPoint.Y)
			{
				endPoint.Y = startPoint.Y + minSize.Height;
			}

			break;
		}

		return ToRect(startPoint, endPoint);
	}

	winrt::Rect ImageCropper::GetUniformRect(winrt::Rect targetRect, double aspectRatio)
	{
		auto ratio = targetRect.Width / targetRect.Height;
		auto cx = targetRect.X + (targetRect.Width / 2);
		auto cy = targetRect.Y + (targetRect.Height / 2);
		double width, height;
		if (aspectRatio > ratio)
		{
			width = targetRect.Width;
			height = width / aspectRatio;
		}
		else
		{
			height = targetRect.Height;
			width = height * aspectRatio;
		}
		auto x = cx - (width / 2.0f);
		auto y = cy - (height / 2.0f);
		return winrt::Rect(
			static_cast<float>(x),
			static_cast<float>(y),
			static_cast<float>(width),
			static_cast<float>(height));
	}

	bool ImageCropper::IsValidRect(winrt::Rect targetRect)
	{
		return !winrt::RectHelper::GetIsEmpty(targetRect) && targetRect.Width > 0 && targetRect.Height > 0;
	}

	winrt::Point ImageCropper::GetSafeSizeChangeWhenKeepAspectRatio(winrt::Rect targetRect, ThumbPosition thumbPosition, winrt::Rect selectedRect, winrt::Point originSizeChange, double aspectRatio)
	{
		auto safeWidthChange = originSizeChange.X;
		auto safeHeightChange = originSizeChange.Y;
		auto maxWidthChange = 0.0f;
		auto maxHeightChange = 0.0f;
		switch (thumbPosition)
		{
		case ThumbPosition::Top:
			maxWidthChange = targetRect.Width - selectedRect.Width;
			maxHeightChange = winrt::RectHelper::GetTop(selectedRect) - winrt::RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::Bottom:
			maxWidthChange = targetRect.Width - selectedRect.Width;
			maxHeightChange = winrt::RectHelper::GetBottom(targetRect) - winrt::RectHelper::GetBottom(selectedRect);
			break;
		case ThumbPosition::Left:
			maxWidthChange = winrt::RectHelper::GetLeft(selectedRect) - winrt::RectHelper::GetLeft(targetRect);
			maxHeightChange = targetRect.Height - selectedRect.Height;
			break;
		case ThumbPosition::Right:
			maxWidthChange = winrt::RectHelper::GetRight(targetRect) - winrt::RectHelper::GetRight(selectedRect);
			maxHeightChange = targetRect.Height - selectedRect.Height;
			break;
		case ThumbPosition::UpperLeft:
			maxWidthChange = winrt::RectHelper::GetLeft(selectedRect) - winrt::RectHelper::GetLeft(targetRect);
			maxHeightChange = winrt::RectHelper::GetTop(selectedRect) - winrt::RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::UpperRight:
			maxWidthChange = winrt::RectHelper::GetRight(targetRect) - winrt::RectHelper::GetRight(selectedRect);
			maxHeightChange = winrt::RectHelper::GetTop(selectedRect) - winrt::RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::LowerLeft:
			maxWidthChange = winrt::RectHelper::GetLeft(selectedRect) - winrt::RectHelper::GetLeft(targetRect);
			maxHeightChange = winrt::RectHelper::GetBottom(targetRect) - winrt::RectHelper::GetBottom(selectedRect);
			break;
		case ThumbPosition::LowerRight:
			maxWidthChange = winrt::RectHelper::GetRight(targetRect) - winrt::RectHelper::GetRight(selectedRect);
			maxHeightChange = winrt::RectHelper::GetBottom(targetRect) - winrt::RectHelper::GetBottom(selectedRect);
			break;
		}

		if (originSizeChange.X > maxWidthChange)
		{
			safeWidthChange = maxWidthChange;
			safeHeightChange = static_cast<float>(safeWidthChange / aspectRatio);
		}

		if (originSizeChange.Y > maxHeightChange)
		{
			safeHeightChange = maxHeightChange;
			safeWidthChange = static_cast<float>(safeHeightChange * aspectRatio);
		}

		return winrt::Point(safeWidthChange, safeHeightChange);
	}

	bool ImageCropper::CanContains(winrt::Rect targetRect, winrt::Rect testRect)
	{
		return (targetRect.Width - testRect.Width > -ThresholdValue) && (targetRect.Height - testRect.Height > -ThresholdValue);
	}

	bool ImageCropper::TryGetContainedRect(winrt::Rect targetRect, winrt::Rect& testRect)
	{
		if (!CanContains(targetRect, testRect))
		{
			return false;
		}

		if (winrt::RectHelper::GetLeft(targetRect) > winrt::RectHelper::GetLeft(testRect))
		{
			testRect.X += RectHelper::GetLeft(targetRect) - winrt::RectHelper::GetLeft(testRect);
		}

		if (winrt::RectHelper::GetTop(targetRect) > winrt::RectHelper::GetTop(testRect))
		{
			testRect.Y += RectHelper::GetTop(targetRect) - winrt::RectHelper::GetTop(testRect);
		}

		if (winrt::RectHelper::GetRight(targetRect) < winrt::RectHelper::GetRight(testRect))
		{
			testRect.X += RectHelper::GetRight(targetRect) - winrt::RectHelper::GetRight(testRect);
		}

		if (winrt::RectHelper::GetBottom(targetRect) < winrt::RectHelper::GetBottom(testRect))
		{
			testRect.Y += winrt::RectHelper::GetBottom(targetRect) - winrt::RectHelper::GetBottom(testRect);
		}

		return true;
	}

	bool ImageCropper::IsCornerThumb(ThumbPosition thumbPosition)
	{
		switch (thumbPosition)
		{
		case ThumbPosition::Top:
		case ThumbPosition::Bottom:
		case ThumbPosition::Left:
		case ThumbPosition::Right:
			return false;
		case ThumbPosition::UpperLeft:
		case ThumbPosition::UpperRight:
		case ThumbPosition::LowerLeft:
		case ThumbPosition::LowerRight:
			return true;
		}

		return false;
	}
}
