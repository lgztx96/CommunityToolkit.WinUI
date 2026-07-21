// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "SurfaceLoader.h"
#include "../Extensions/System/UriExtensions.h"

namespace winrt::XamlToolkit::WinUI::Media::Helpers
{
	std::shared_ptr<SurfaceLoader> SurfaceLoader::GetInstance()
	{
		auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();
		return GetInstance(compositor);
	}

	std::shared_ptr<SurfaceLoader> SurfaceLoader::GetInstance(winrt::Compositor const& compositor)
	{
		std::lock_guard<std::mutex> lock(_instanceMutex);

		auto& instances = Instances;
		if (auto it = instances.find(compositor); it != instances.end())
		{
			return it->second;
		}

		auto instance = std::shared_ptr<SurfaceLoader>(new SurfaceLoader(compositor));
		return instances.emplace(compositor, instance).first->second;
	}

	void SurfaceLoader::InitializeDevices()
	{
		if (_canvasDevice)
		{
			_deviceLostRevoker.revoke();
		}

		if (_compositionDevice)
		{
			_deviceReplacedRevoker.revoke();
		}

		_canvasDevice = winrt::CanvasDevice();
		_compositionDevice = winrt::CanvasComposition::CreateCompositionGraphicsDevice(_compositor, _canvasDevice);

		_deviceLostRevoker = _canvasDevice.DeviceLost(winrt::auto_revoke, { this, &SurfaceLoader::CanvasDevice_DeviceLost });
		_deviceReplacedRevoker = _compositionDevice.RenderingDeviceReplaced(winrt::auto_revoke, { this, &SurfaceLoader::CompositionDevice_RenderingDeviceReplaced });
	}

	void SurfaceLoader::CanvasDevice_DeviceLost(
		[[maybe_unused]] winrt::CanvasDevice const& sender,
		[[maybe_unused]] winrt::IInspectable const& args)
	{
		InitializeDevices();
	}

	void SurfaceLoader::CompositionDevice_RenderingDeviceReplaced(
		[[maybe_unused]] winrt::CompositionGraphicsDevice const& sender, 
		[[maybe_unused]] winrt::RenderingDeviceReplacedEventArgs const& args)
	{
		InitializeDevices();
	}

	winrt::IAsyncOperation<winrt::CompositionBrush> SurfaceLoader::LoadImageAsync(
		winrt::Uri uri,
		Media::DpiMode dpiMode,
		Media::CacheMode cacheMode)
	{
		auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

		// Lock and check the cache first
		auto lock = co_await _win2dMutex.lock_async();
		uri = winrt::XamlToolkit::WinUI::Media::UriExtensions::ToAppxUri(uri);

		if (cacheMode == Media::CacheMode::Default)
		{
			winrt::CompositionBrush cachedBrush{ nullptr };
			if (Cache.TryGetValue(compositor, uri, cachedBrush))
			{
				co_return cachedBrush;
			}
		}

		// Load the image
		winrt::CompositionBrush brush{ nullptr };
		try
		{
			// This will throw and the canvas will re-initialize the Win2D device if needed
			auto sharedDevice = winrt::CanvasDevice::GetSharedDevice();
			brush = co_await LoadSurfaceBrushAsync(sharedDevice, compositor, uri, dpiMode);
		}
		catch (...)
		{
			// Device error
		}

		// Cache when needed and return the result
		/*if (brush && cacheMode != Media::CacheMode::Disabled)
		{
			Cache.AddOrUpdate(compositor, uri, brush);
		}*/

		co_return brush;
	}

	winrt::IAsyncOperation<winrt::CompositionDrawingSurface> SurfaceLoader::LoadFromUri(
		winrt::Uri const& uri,
		winrt::Size sizeTarget)
	{
		auto bitmap = co_await winrt::CanvasBitmap::LoadAsync(_canvasDevice, uri);
		auto sizeSource = bitmap.Size();

		if (sizeTarget.Width == 0 || sizeTarget.Height == 0)
		{
			sizeTarget = sizeSource;
		}

		auto surface = _compositionDevice.CreateDrawingSurface(
			sizeTarget,
			winrt::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::DirectXAlphaMode::Premultiplied);

		{
			auto ds = winrt::CanvasComposition::CreateDrawingSession(surface);
			ds.Clear(winrt::Windows::UI::Color{ 0, 0, 0, 0 });
			ds.DrawImage(bitmap,
				winrt::Rect{ 0, 0, sizeTarget.Width, sizeTarget.Height },
				winrt::Rect{ 0, 0, sizeSource.Width, sizeSource.Height });
		}

		co_return surface;
	}

	winrt::CompositionDrawingSurface SurfaceLoader::LoadText(
		winrt::hstring const& text,
		winrt::Size sizeTarget,
		winrt::CanvasTextFormat const& textFormat,
		winrt::Windows::UI::Color const& textColor,
		winrt::Windows::UI::Color const& bgColor)
	{
		auto surface = _compositionDevice.CreateDrawingSurface(
			sizeTarget,
			winrt::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::DirectXAlphaMode::Premultiplied);

		{
			auto ds = winrt::CanvasComposition::CreateDrawingSession(surface);
			ds.Clear(bgColor);
			ds.DrawText(text,
				winrt::Rect{ 0, 0, sizeTarget.Width, sizeTarget.Height },
				textColor,
				textFormat);
		}

		return surface;
	}

	winrt::IAsyncOperation<winrt::CompositionBrush> SurfaceLoader::LoadSurfaceBrushAsync(
		winrt::CanvasDevice const& canvasDevice,
		winrt::Compositor const& compositor,
		winrt::Uri const& uri,
		Media::DpiMode dpiMode)
	{
		float dpi = static_cast<float>(GetSystemDpiForProcess(GetCurrentProcess())); 

		// Load the bitmap with the appropriate settings
		winrt::CanvasBitmap bitmap{ nullptr };

		auto loadBitmap = [&](std::optional<float> targetDpi) -> 
			winrt::IAsyncOperation<winrt::CanvasBitmap>
		{
			if (uri.SchemeName() == L"file")
			{
				auto filePath = uri.RawUri();

				if (targetDpi)
				{
					co_return co_await winrt::CanvasBitmap::LoadAsync(canvasDevice, filePath, *targetDpi);
				}

				co_return co_await winrt::CanvasBitmap::LoadAsync(canvasDevice, filePath);
			}

			if (targetDpi)
			{
				co_return co_await winrt::CanvasBitmap::LoadAsync(canvasDevice, uri, *targetDpi);
			}

			co_return co_await winrt::CanvasBitmap::LoadAsync(canvasDevice, uri);
		};

		switch (dpiMode)
		{
		case Media::DpiMode::UseSourceDpi:
			bitmap = co_await loadBitmap(std::nullopt);
			break;

		case Media::DpiMode::Default96Dpi:
			bitmap = co_await loadBitmap(96.0f);
			break;

		case Media::DpiMode::DisplayDpi:
			bitmap = co_await loadBitmap(dpi);
			break;

		case Media::DpiMode::DisplayDpiWith96AsLowerBound:
			bitmap = co_await loadBitmap(std::max<float>(dpi, 96.0f));
			break;

		default:
			throw winrt::hresult_invalid_argument(L"Invalid DPI mode");
		}

		// Calculate the surface size
		auto size = bitmap.Size();
		auto sizeInPixels = winrt::Size{
			static_cast<float>(bitmap.SizeInPixels().Width),
			static_cast<float>(bitmap.SizeInPixels().Height)
		};

		// Get the device and the target surface
		auto graphicsDevice = winrt::CanvasComposition::CreateCompositionGraphicsDevice(compositor, canvasDevice);

		// Create the drawing surface
		auto drawingSurface = graphicsDevice.CreateDrawingSurface(
			sizeInPixels,
			winrt::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::DirectXAlphaMode::Premultiplied);

		{
			// Create a drawing session for the target surface
			auto drawingSession = winrt::CanvasComposition::CreateDrawingSession(
				drawingSurface,
				winrt::Rect{ 0, 0, sizeInPixels.Width, sizeInPixels.Height },
				dpi);

			drawingSession.Clear(winrt::Windows::UI::Color{ 0, 0, 0, 0 });
			drawingSession.DrawImage(bitmap,
				winrt::Rect{ 0, 0, size.Width, size.Height },
				winrt::Rect{ 0, 0, size.Width, size.Height });
		}

		// Setup the effect brush to use
		auto surfaceBrush = compositor.CreateSurfaceBrush(drawingSurface);
		surfaceBrush.Stretch(winrt::CompositionStretch::None);

		// Adjust the scale if the DPI scaling is greater than 100%
		double pixels = dpi / 96.0f;
		if (pixels > 1.0)
		{
			surfaceBrush.Scale(winrt::float2{ static_cast<float>(1.0 / pixels), static_cast<float>(1.0 / pixels) });
			surfaceBrush.BitmapInterpolationMode(winrt::CompositionBitmapInterpolationMode::NearestNeighbor);
		}

		co_return surfaceBrush;
	}
}