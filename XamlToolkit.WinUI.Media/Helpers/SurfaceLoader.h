// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "CompositionObjectCache.h"
#include "../XamlToolkit.WinUI/common.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.DirectX.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <mutex>
#include <memory>
#include <functional>
#else
import winrt.Windows.UI;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.Graphics.Canvas;
import winrt.Microsoft.Graphics.DirectX;
import winrt.Microsoft.Graphics.Canvas.Text;
import winrt.Microsoft.Graphics.Canvas.UI.Composition;
import winrt.XamlToolkit.WinUI.Media;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Numerics;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
    using namespace winrt::Microsoft::Graphics::Canvas;
    using namespace winrt::Microsoft::Graphics::DirectX;
    using namespace winrt::Microsoft::Graphics::Canvas::Text;
    using namespace winrt::Microsoft::Graphics::Canvas::UI::Composition;
}

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
        static std::shared_ptr<SurfaceLoader> GetInstance(winrt::Compositor const& compositor);

        /// <summary>
        /// Loads a CompositionBrush instance with the target image.
        /// </summary>
        static winrt::IAsyncOperation<winrt::CompositionBrush> LoadImageAsync(
            winrt::Uri uri,
            Media::DpiMode dpiMode,
            Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Loads a CompositionDrawingSurface from URI.
        /// </summary>
        winrt::IAsyncOperation<winrt::CompositionDrawingSurface> LoadFromUri(
            winrt::Uri const& uri,
            winrt::Size sizeTarget = {});

        /// <summary>
        /// Loads the text on to a <see cref="CompositionDrawingSurface"/>.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="sizeTarget">The size target.</param>
        /// <param name="textFormat">The text format.</param>
        /// <param name="textColor">Color of the text.</param>
        /// <param name="bgColor">Color of the bg.</param>
        /// <returns><see cref="CompositionDrawingSurface"/></returns>
        winrt::CompositionDrawingSurface LoadText(
            winrt::hstring const& text,
            winrt::Size sizeTarget,
            winrt::CanvasTextFormat const& textFormat,
            winrt::Windows::UI::Color const& textColor,
            winrt::Windows::UI::Color const& bgColor);

        /// <summary>
        /// Gets the canvas device for this instance.
        /// </summary>
        winrt::CanvasDevice CanvasDevice() const { return _canvasDevice; }

        /// <summary>
        /// Gets the composition graphics device for this instance.
        /// </summary>
        winrt::CompositionGraphicsDevice CompositionDevice() const { return _compositionDevice; }

    private:
        SurfaceLoader() = default;

		SurfaceLoader(winrt::Compositor const& compositor)
		{
			_compositor = compositor;
            InitializeDevices();
		}

        void InitializeDevices();

        void CanvasDevice_DeviceLost(winrt::CanvasDevice const& sender, winrt::IInspectable const& args);

        void CompositionDevice_RenderingDeviceReplaced(winrt::CompositionGraphicsDevice const& sender, winrt::RenderingDeviceReplacedEventArgs const& args);

        winrt::Compositor _compositor{ nullptr };
        winrt::CanvasDevice _canvasDevice{ nullptr };
        winrt::CompositionGraphicsDevice _compositionDevice{ nullptr };

		winrt::CanvasDevice::DeviceLost_revoker _deviceLostRevoker;
		winrt::CompositionGraphicsDevice::RenderingDeviceReplaced_revoker _deviceReplacedRevoker;

        static inline std::mutex _instanceMutex;
        static inline std::map<winrt::Compositor, std::shared_ptr<SurfaceLoader>> Instances;

        static inline winrt::async_mutex _win2dMutex;
        static inline CompositionObjectCacheWithKey<winrt::Uri, winrt::CompositionBrush> Cache;

        static winrt::IAsyncOperation<winrt::CompositionBrush> LoadSurfaceBrushAsync(
            winrt::CanvasDevice const& canvasDevice,
            winrt::Compositor const& compositor,
            winrt::Uri const& uri,
            Media::DpiMode dpiMode);
    };
}