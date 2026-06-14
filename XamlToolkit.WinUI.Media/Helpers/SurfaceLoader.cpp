// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "SurfaceLoader.h"
#include "../Extensions/System/UriExtensions.h"

import winrt.Microsoft.Graphics.Canvas.UI.Composition;

namespace winrt::XamlToolkit::WinUI::Media::Helpers
{
	std::shared_ptr<SurfaceLoader> SurfaceLoader::GetInstance()
	{
		auto compositor = winrt::Microsoft::UI::Xaml::Media::CompositionTarget::GetCompositorForCurrentThread();
		return GetInstance(compositor);
	}

	std::shared_ptr<SurfaceLoader> SurfaceLoader::GetInstance(winrt::Microsoft::UI::Composition::Compositor const& compositor)
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

		_canvasDevice = winrt::Microsoft::Graphics::Canvas::CanvasDevice();
		_compositionDevice = winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionGraphicsDevice(_compositor, _canvasDevice);

		_deviceLostRevoker = _canvasDevice.DeviceLost(winrt::auto_revoke, { this, &SurfaceLoader::CanvasDevice_DeviceLost });
		_deviceReplacedRevoker = _compositionDevice.RenderingDeviceReplaced(winrt::auto_revoke, { this, &SurfaceLoader::CompositionDevice_RenderingDeviceReplaced });
	}

	void SurfaceLoader::CanvasDevice_DeviceLost(
		[[maybe_unused]] winrt::Microsoft::Graphics::Canvas::CanvasDevice const& sender,
		[[maybe_unused]] winrt::Windows::Foundation::IInspectable const& args)
	{
		InitializeDevices();
	}

	void SurfaceLoader::CompositionDevice_RenderingDeviceReplaced(
		[[maybe_unused]] winrt::Microsoft::UI::Composition::CompositionGraphicsDevice const& sender, 
		[[maybe_unused]] winrt::Microsoft::UI::Composition::RenderingDeviceReplacedEventArgs const& args)
	{
		InitializeDevices();
	}

	winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionBrush> SurfaceLoader::LoadImageAsync(
		winrt::Windows::Foundation::Uri uri,
		Media::DpiMode dpiMode,
		Media::CacheMode cacheMode)
	{
		auto compositor = winrt::Microsoft::UI::Xaml::Media::CompositionTarget::GetCompositorForCurrentThread();

		// Lock and check the cache first
		auto lock = co_await _win2dMutex.lock_async();
		uri = winrt::XamlToolkit::WinUI::Media::UriExtensions::ToAppxUri(uri);

		if (cacheMode == Media::CacheMode::Default)
		{
			winrt::Microsoft::UI::Composition::CompositionBrush cachedBrush{ nullptr };
			if (Cache.TryGetValue(compositor, uri, cachedBrush))
			{
				co_return cachedBrush;
			}
		}

		// Load the image
		winrt::Microsoft::UI::Composition::CompositionBrush brush{ nullptr };
		try
		{
			// This will throw and the canvas will re-initialize the Win2D device if needed
			auto sharedDevice = winrt::Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();
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

	winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionDrawingSurface> SurfaceLoader::LoadFromUri(
		winrt::Windows::Foundation::Uri const& uri,
		winrt::Windows::Foundation::Size sizeTarget)
	{
		auto bitmap = co_await winrt::Microsoft::Graphics::Canvas::CanvasBitmap::LoadAsync(_canvasDevice, uri);
		auto sizeSource = bitmap.Size();

		if (sizeTarget.Width == 0 || sizeTarget.Height == 0)
		{
			sizeTarget = sizeSource;
		}

		auto surface = _compositionDevice.CreateDrawingSurface(
			sizeTarget,
			winrt::Microsoft::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Microsoft::Graphics::DirectX::DirectXAlphaMode::Premultiplied);

		{
			auto ds = winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateDrawingSession(surface);
			ds.Clear(winrt::Windows::UI::Color{ 0, 0, 0, 0 });
			ds.DrawImage(bitmap,
				winrt::Windows::Foundation::Rect{ 0, 0, sizeTarget.Width, sizeTarget.Height },
				winrt::Windows::Foundation::Rect{ 0, 0, sizeSource.Width, sizeSource.Height });
		}

		co_return surface;
	}

	winrt::Microsoft::UI::Composition::CompositionDrawingSurface SurfaceLoader::LoadText(
		winrt::hstring const& text,
		winrt::Windows::Foundation::Size sizeTarget,
		winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat const& textFormat,
		winrt::Windows::UI::Color const& textColor,
		winrt::Windows::UI::Color const& bgColor)
	{
		auto surface = _compositionDevice.CreateDrawingSurface(
			sizeTarget,
			winrt::Microsoft::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Microsoft::Graphics::DirectX::DirectXAlphaMode::Premultiplied);

		{
			auto ds = winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateDrawingSession(surface);
			ds.Clear(bgColor);
			ds.DrawText(text,
				winrt::Windows::Foundation::Rect{ 0, 0, sizeTarget.Width, sizeTarget.Height },
				textColor,
				textFormat);
		}

		return surface;
	}

	winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionBrush> SurfaceLoader::LoadSurfaceBrushAsync(
		winrt::Microsoft::Graphics::Canvas::CanvasDevice const& canvasDevice,
		winrt::Microsoft::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::Uri const& uri,
		Media::DpiMode dpiMode)
	{
		float dpi = static_cast<float>(GetSystemDpiForProcess(GetCurrentProcess())); 

		// Load the bitmap with the appropriate settings
		winrt::Microsoft::Graphics::Canvas::CanvasBitmap bitmap{ nullptr };

		auto loadBitmap = [&](std::optional<float> targetDpi) -> 
			winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::Graphics::Canvas::CanvasBitmap>
		{
			if (uri.SchemeName() == L"file")
			{
				auto filePath = uri.RawUri();

				if (targetDpi)
				{
					co_return co_await winrt::Microsoft::Graphics::Canvas::CanvasBitmap::LoadAsync(canvasDevice, filePath, *targetDpi);
				}

				co_return co_await winrt::Microsoft::Graphics::Canvas::CanvasBitmap::LoadAsync(canvasDevice, filePath);
			}

			if (targetDpi)
			{
				co_return co_await winrt::Microsoft::Graphics::Canvas::CanvasBitmap::LoadAsync(canvasDevice, uri, *targetDpi);
			}

			co_return co_await winrt::Microsoft::Graphics::Canvas::CanvasBitmap::LoadAsync(canvasDevice, uri);
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
		auto sizeInPixels = winrt::Windows::Foundation::Size{
			static_cast<float>(bitmap.SizeInPixels().Width),
			static_cast<float>(bitmap.SizeInPixels().Height)
		};

		// Get the device and the target surface
		auto graphicsDevice = winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionGraphicsDevice(compositor, canvasDevice);

		// Create the drawing surface
		auto drawingSurface = graphicsDevice.CreateDrawingSurface(
			sizeInPixels,
			winrt::Microsoft::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Microsoft::Graphics::DirectX::DirectXAlphaMode::Premultiplied);

		{
			// Create a drawing session for the target surface
			auto drawingSession = winrt::Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateDrawingSession(
				drawingSurface,
				winrt::Windows::Foundation::Rect{ 0, 0, sizeInPixels.Width, sizeInPixels.Height },
				dpi);

			drawingSession.Clear(winrt::Windows::UI::Color{ 0, 0, 0, 0 });
			drawingSession.DrawImage(bitmap,
				winrt::Windows::Foundation::Rect{ 0, 0, size.Width, size.Height },
				winrt::Windows::Foundation::Rect{ 0, 0, size.Width, size.Height });
		}

		// Setup the effect brush to use
		auto surfaceBrush = compositor.CreateSurfaceBrush(drawingSurface);
		surfaceBrush.Stretch(winrt::Microsoft::UI::Composition::CompositionStretch::None);

		// Adjust the scale if the DPI scaling is greater than 100%
		double pixels = dpi / 96.0f;
		if (pixels > 1.0)
		{
			surfaceBrush.Scale(winrt::Windows::Foundation::Numerics::float2{ static_cast<float>(1.0 / pixels), static_cast<float>(1.0 / pixels) });
			surfaceBrush.BitmapInterpolationMode(winrt::Microsoft::UI::Composition::CompositionBitmapInterpolationMode::NearestNeighbor);
		}

		co_return surfaceBrush;
	}
}