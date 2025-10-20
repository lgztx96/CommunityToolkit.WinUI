#include "pch.h"
#include "ImageCropper.h"
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation::Numerics;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::Effects;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	static const constexpr double ThresholdValue = 0.001;

	winrt::Windows::Foundation::IAsyncAction ImageCropper::CropImageAsync(WriteableBitmap const& writeableBitmap, IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat)
	{
		croppedRect.X = std::max<float>(croppedRect.X, 0);
		croppedRect.Y = std::max<float>(croppedRect.Y, 0);
		auto x = (uint32_t)std::floor(croppedRect.X);
		auto y = (uint32_t)std::floor(croppedRect.Y);
		auto width = (uint32_t)std::floor(croppedRect.Width);
		auto height = (uint32_t)std::floor(croppedRect.Height);
		auto buffer = writeableBitmap.PixelBuffer();

		const auto& bitmapEncoder = co_await BitmapEncoder::CreateAsync(GetEncoderId(bitmapFileFormat), stream);
		bitmapEncoder.SetPixelData(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Premultiplied, (uint32_t)writeableBitmap.PixelWidth(), (uint32_t)writeableBitmap.PixelHeight(), 96.0, 96.0, { buffer.data(), buffer.Length() });
		bitmapEncoder.BitmapTransform().Bounds(BitmapBounds
			{
				.X = x,
				.Y = y,
				.Width = width,
				.Height = height
			});
		co_await bitmapEncoder.FlushAsync();
	}

	winrt::Windows::Foundation::IAsyncAction ImageCropper::CropImageWithShapeAsync(WriteableBitmap const& writeableBitmap, IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat, Controls::CropShape cropShape)
	{
		auto device = CanvasDevice::GetSharedDevice();
		auto clipGeometry = CreateClipGeometry(device, cropShape, Size(croppedRect.Width, croppedRect.Height));
		if (clipGeometry == nullptr)
		{
			co_return;
		}

		// WinUI3/Win2D bug: switch back to CanvasBitmap once it works.
		CanvasVirtualBitmap sourceBitmap{ nullptr };
		auto randomAccessStream = InMemoryRandomAccessStream();
		co_await CropImageAsync(writeableBitmap, randomAccessStream, croppedRect, bitmapFileFormat);
		sourceBitmap = co_await CanvasVirtualBitmap::LoadAsync(device, randomAccessStream);

		auto offScreen = CanvasRenderTarget(device, croppedRect.Width, croppedRect.Height, 96.0f);
		auto drawingSession = offScreen.CreateDrawingSession();
		auto markCommandList = CanvasCommandList(device);

		auto markDrawingSession = markCommandList.CreateDrawingSession();
		markDrawingSession.FillGeometry(clipGeometry, Windows::UI::Colors::Black());

		AlphaMaskEffect alphaMaskEffect;
		alphaMaskEffect.Source(sourceBitmap);
		alphaMaskEffect.AlphaMask(markCommandList);

		drawingSession.DrawImage(alphaMaskEffect);

		auto pixelBytes = offScreen.GetPixelBytes();
		auto bitmapEncoder = co_await BitmapEncoder::CreateAsync(GetEncoderId(bitmapFileFormat), stream);
		bitmapEncoder.SetPixelData(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Premultiplied, offScreen.SizeInPixels().Width, offScreen.SizeInPixels().Height, 96.0, 96.0, pixelBytes);
		co_await bitmapEncoder.FlushAsync();
	}

	CanvasGeometry ImageCropper::CreateClipGeometry(ICanvasResourceCreator resourceCreator, Controls::CropShape cropShape, Size croppedSize)
	{
		switch (cropShape)
		{
		case CropShape::Rectangular:
			break;
		case CropShape::Circular:
			auto radiusX = croppedSize.Width / 2;
			auto radiusY = croppedSize.Height / 2;
			auto center = Point(radiusX, radiusY);
			return CanvasGeometry::CreateEllipse(resourceCreator, float2(center.X, center.Y), static_cast<float>(radiusX), static_cast<float>(radiusY));
		}

		return nullptr;
	}

	winrt::guid ImageCropper::GetEncoderId(BitmapFileFormat bitmapFileFormat)
	{
		switch (bitmapFileFormat)
		{
		case BitmapFileFormat::Bmp:
			return BitmapEncoder::BmpEncoderId();
		case BitmapFileFormat::Png:
			return BitmapEncoder::PngEncoderId();
		case BitmapFileFormat::Jpeg:
			return BitmapEncoder::JpegEncoderId();
		case BitmapFileFormat::Tiff:
			return BitmapEncoder::TiffEncoderId();
		case BitmapFileFormat::Gif:
			return BitmapEncoder::GifEncoderId();
		case BitmapFileFormat::JpegXR:
			return BitmapEncoder::JpegXREncoderId();
		}

		return BitmapEncoder::PngEncoderId();
	}

	Point ImageCropper::GetSafePoint(Rect targetRect, Point point)
	{
		auto safePoint = Point(point.X, point.Y);
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

	bool ImageCropper::IsSafePoint(Rect targetRect, Point point)
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

	bool ImageCropper::IsSafeRect(Point startPoint, Point endPoint, Size minSize)
	{
		auto checkPoint = Point(startPoint.X + minSize.Width, startPoint.Y + minSize.Height);
		return checkPoint.X - endPoint.X < ThresholdValue
			&& checkPoint.Y - endPoint.Y < ThresholdValue;
	}

	Rect ImageCropper::GetSafeRect(Point startPoint, Point endPoint, Size minSize, ThumbPosition position)
	{
		auto checkPoint = Point(startPoint.X + minSize.Width, startPoint.Y + minSize.Height);
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

	Rect ImageCropper::GetUniformRect(Rect targetRect, double aspectRatio)
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
		return Rect(
			static_cast<float>(x),
			static_cast<float>(y),
			static_cast<float>(width),
			static_cast<float>(height));
	}

	bool ImageCropper::IsValidRect(Rect targetRect)
	{
		return !RectHelper::GetIsEmpty(targetRect) && targetRect.Width > 0 && targetRect.Height > 0;
	}

	Point ImageCropper::GetSafeSizeChangeWhenKeepAspectRatio(Rect targetRect, ThumbPosition thumbPosition, Rect selectedRect, Point originSizeChange, double aspectRatio)
	{
		auto safeWidthChange = originSizeChange.X;
		auto safeHeightChange = originSizeChange.Y;
		auto maxWidthChange = 0.0f;
		auto maxHeightChange = 0.0f;
		switch (thumbPosition)
		{
		case ThumbPosition::Top:
			maxWidthChange = targetRect.Width - selectedRect.Width;
			maxHeightChange = RectHelper::GetTop(selectedRect) - RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::Bottom:
			maxWidthChange = targetRect.Width - selectedRect.Width;
			maxHeightChange = RectHelper::GetBottom(targetRect) - RectHelper::GetBottom(selectedRect);
			break;
		case ThumbPosition::Left:
			maxWidthChange = RectHelper::GetLeft(selectedRect) - RectHelper::GetLeft(targetRect);
			maxHeightChange = targetRect.Height - selectedRect.Height;
			break;
		case ThumbPosition::Right:
			maxWidthChange = RectHelper::GetRight(targetRect) - RectHelper::GetRight(selectedRect);
			maxHeightChange = targetRect.Height - selectedRect.Height;
			break;
		case ThumbPosition::UpperLeft:
			maxWidthChange = RectHelper::GetLeft(selectedRect) - RectHelper::GetLeft(targetRect);
			maxHeightChange = RectHelper::GetTop(selectedRect) - RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::UpperRight:
			maxWidthChange = RectHelper::GetRight(targetRect) - RectHelper::GetRight(selectedRect);
			maxHeightChange = RectHelper::GetTop(selectedRect) - RectHelper::GetTop(targetRect);
			break;
		case ThumbPosition::LowerLeft:
			maxWidthChange = RectHelper::GetLeft(selectedRect) - RectHelper::GetLeft(targetRect);
			maxHeightChange = RectHelper::GetBottom(targetRect) - RectHelper::GetBottom(selectedRect);
			break;
		case ThumbPosition::LowerRight:
			maxWidthChange = RectHelper::GetRight(targetRect) - RectHelper::GetRight(selectedRect);
			maxHeightChange = RectHelper::GetBottom(targetRect) - RectHelper::GetBottom(selectedRect);
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

		return Point(safeWidthChange, safeHeightChange);
	}

	bool ImageCropper::CanContains(Rect targetRect, Rect testRect)
	{
		return (targetRect.Width - testRect.Width > -ThresholdValue) && (targetRect.Height - testRect.Height > -ThresholdValue);
	}

	bool ImageCropper::TryGetContainedRect(Rect targetRect, Rect& testRect)
	{
		if (!CanContains(targetRect, testRect))
		{
			return false;
		}

		if (RectHelper::GetLeft(targetRect) > RectHelper::GetLeft(testRect))
		{
			testRect.X += RectHelper::GetLeft(targetRect) - RectHelper::GetLeft(testRect);
		}

		if (RectHelper::GetTop(targetRect) > RectHelper::GetTop(testRect))
		{
			testRect.Y += RectHelper::GetTop(targetRect) - RectHelper::GetTop(testRect);
		}

		if (RectHelper::GetRight(targetRect) < RectHelper::GetRight(testRect))
		{
			testRect.X += RectHelper::GetRight(targetRect) - RectHelper::GetRight(testRect);
		}

		if (RectHelper::GetBottom(targetRect) < RectHelper::GetBottom(testRect))
		{
			testRect.Y += RectHelper::GetBottom(targetRect) - RectHelper::GetBottom(testRect);
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