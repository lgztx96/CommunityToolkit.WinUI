// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "CompositionObjectCache.h"
#include "../../Extensions/async_mutex.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <mutex>
#include <memory>
#include <functional>
#endif

import winrt.XamlToolkit.WinUI.Media;
import winrt.Windows.Foundation;
import winrt.Windows.UI;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.Graphics.Canvas;
import winrt.Microsoft.Graphics.Canvas.Text;

namespace winrt::XamlToolkit::WinUI::Media::Helpers
{
    /// <summary>
    /// A class that can load and draw images and other objects to Win2D surfaces and brushes.
    /// Pure C++ class - no IDL exposure.
    /// </summary>
    class SurfaceLoader
    {
    public:
        /// <summary>
        /// Gets a SurfaceLoader instance for the compositor of the current thread.
        /// </summary>
        static std::shared_ptr<SurfaceLoader> GetInstance();

        /// <summary>
        /// Gets a SurfaceLoader instance for a given compositor.
        /// </summary>
        static std::shared_ptr<SurfaceLoader> GetInstance(winrt::Microsoft::UI::Composition::Compositor const& compositor);

        /// <summary>
        /// Loads a CompositionBrush instance with the target image.
        /// </summary>
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionBrush> LoadImageAsync(
            winrt::Windows::Foundation::Uri uri,
            Media::DpiMode dpiMode,
            Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Loads a CompositionDrawingSurface from URI.
        /// </summary>
        winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionDrawingSurface> LoadFromUri(
            winrt::Windows::Foundation::Uri const& uri,
            winrt::Windows::Foundation::Size sizeTarget = {});

        /// <summary>
        /// Loads the text on to a <see cref="CompositionDrawingSurface"/>.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="sizeTarget">The size target.</param>
        /// <param name="textFormat">The text format.</param>
        /// <param name="textColor">Color of the text.</param>
        /// <param name="bgColor">Color of the bg.</param>
        /// <returns><see cref="CompositionDrawingSurface"/></returns>
        winrt::Microsoft::UI::Composition::CompositionDrawingSurface LoadText(
            winrt::hstring const& text,
            winrt::Windows::Foundation::Size sizeTarget,
            winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat const& textFormat,
            winrt::Windows::UI::Color const& textColor,
            winrt::Windows::UI::Color const& bgColor);

        /// <summary>
        /// Gets the canvas device for this instance.
        /// </summary>
        winrt::Microsoft::Graphics::Canvas::CanvasDevice CanvasDevice() const { return _canvasDevice; }

        /// <summary>
        /// Gets the composition graphics device for this instance.
        /// </summary>
        winrt::Microsoft::UI::Composition::CompositionGraphicsDevice CompositionDevice() const { return _compositionDevice; }

    private:
        SurfaceLoader() = default;

		SurfaceLoader(winrt::Microsoft::UI::Composition::Compositor const& compositor)
		{
			_compositor = compositor;
            InitializeDevices();
		}

        void InitializeDevices();

        void CanvasDevice_DeviceLost(winrt::Microsoft::Graphics::Canvas::CanvasDevice const& sender, winrt::Windows::Foundation::IInspectable const& args);

        void CompositionDevice_RenderingDeviceReplaced(winrt::Microsoft::UI::Composition::CompositionGraphicsDevice const& sender, winrt::Microsoft::UI::Composition::RenderingDeviceReplacedEventArgs const& args);

        winrt::Microsoft::UI::Composition::Compositor _compositor{ nullptr };
        winrt::Microsoft::Graphics::Canvas::CanvasDevice _canvasDevice{ nullptr };
        winrt::Microsoft::UI::Composition::CompositionGraphicsDevice _compositionDevice{ nullptr };

		winrt::Microsoft::Graphics::Canvas::CanvasDevice::DeviceLost_revoker _deviceLostRevoker;
		winrt::Microsoft::UI::Composition::CompositionGraphicsDevice::RenderingDeviceReplaced_revoker _deviceReplacedRevoker;

        static inline std::mutex _instanceMutex;
        static inline std::map<winrt::Microsoft::UI::Composition::Compositor, std::shared_ptr<SurfaceLoader>> Instances;

        static inline winrt::async_mutex _win2dMutex;
        static inline CompositionObjectCacheWithKey<winrt::Windows::Foundation::Uri, winrt::Microsoft::UI::Composition::CompositionBrush> Cache;

        static winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::CompositionBrush> LoadSurfaceBrushAsync(
            winrt::Microsoft::Graphics::Canvas::CanvasDevice const& canvasDevice,
            winrt::Microsoft::UI::Composition::Compositor const& compositor,
            winrt::Windows::Foundation::Uri const& uri,
            Media::DpiMode dpiMode);
    };
}