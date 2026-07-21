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
	static const constexpr double ThresholdValue = 0.001;

	winrt::IAsyncAction ImageCropper::CropImageAsync(winrt::WriteableBitmap const& writeableBitmap, winrt::IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat)
	{
		croppedRect.X = std::max<float>(croppedRect.X, 0);
		croppedRect.Y = std::max<float>(croppedRect.Y, 0);

		auto left = static_cast<uint32_t>(std::floor(croppedRect.X));
		auto top = static_cast<uint32_t>(std::floor(croppedRect.Y));
		auto right = static_cast<uint32_t>(std::ceil(croppedRect.X + croppedRect.Width));
		auto bottom = static_cast<uint32_t>(std::ceil(croppedRect.Y + croppedRect.Height));

		auto imgWidth = static_cast<uint32_t>(writeableBitmap.PixelWidth());
		auto imgHeight = static_cast<uint32_t>(writeableBitmap.PixelHeight());

		right = std::min<uint32_t>(right, imgWidth);
		bottom = std::min<uint32_t>(bottom, imgHeight);

		auto x = left;
		auto y = top;
		auto width = right - left;
		auto height = bottom - top;

		auto buffer = writeableBitmap.PixelBuffer();

		const auto& bitmapEncoder = co_await winrt::BitmapEncoder::CreateAsync(GetEncoderId(bitmapFileFormat), stream);
		bitmapEncoder.SetPixelData(winrt::BitmapPixelFormat::Bgra8, winrt::BitmapAlphaMode::Premultiplied, imgWidth, imgHeight, 96.0, 96.0, { buffer.data(), buffer.Length() });
		bitmapEncoder.BitmapTransform().Bounds(winrt::BitmapBounds
		{
			.X = x,
			.Y = y,
			.Width = width,
			.Height = height
		});
		co_await bitmapEncoder.FlushAsync();
	}

	winrt::IAsyncAction ImageCropper::CropImageWithShapeAsync(winrt::WriteableBitmap const& writeableBitmap, winrt::IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat, Controls::CropShape cropShape)
	{
		auto device = winrt::CanvasDevice::GetSharedDevice();
		auto clipGeometry = CreateClipGeometry(device, cropShape, winrt::Size(croppedRect.Width, croppedRect.Height));
		if (clipGeometry == nullptr)
		{
			co_return;
		}

		// WinUI3/Win2D bug: switch back to CanvasBitmap once it works.
		winrt::CanvasVirtualBitmap sourceBitmap{ nullptr };
		winrt::InMemoryRandomAccessStream randomAccessStream;
		co_await CropImageAsync(writeableBitmap, randomAccessStream, croppedRect, bitmapFileFormat);
		sourceBitmap = co_await winrt::CanvasVirtualBitmap::LoadAsync(device, randomAccessStream);

		auto offScreen = winrt::CanvasRenderTarget(device, croppedRect.Width, croppedRect.Height, 96.0f);
		auto drawingSession = offScreen.CreateDrawingSession();
		auto markCommandList = winrt::CanvasCommandList(device);

		auto markDrawingSession = markCommandList.CreateDrawingSession();
		markDrawingSession.FillGeometry(clipGeometry, winrt::Windows::UI::Colors::Black());

		winrt::AlphaMaskEffect alphaMaskEffect;
		alphaMaskEffect.Source(sourceBitmap);
		alphaMaskEffect.AlphaMask(markCommandList);

		drawingSession.DrawImage(alphaMaskEffect);

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
			auto radiusX = croppedSize.Width / 2;
			auto radiusY = croppedSize.Height / 2;
			auto center = winrt::Point(radiusX, radiusY);
			return winrt::CanvasGeometry::CreateEllipse(resourceCreator, winrt::float2(center.X, center.Y), static_cast<float>(radiusX), static_cast<float>(radiusY));
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
