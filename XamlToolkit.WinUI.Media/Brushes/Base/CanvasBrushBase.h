// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "CanvasBrushBase.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Windows.Graphics.DirectX.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.Graphics.Canvas;
import winrt.Microsoft.Graphics.DirectX;
import winrt.Microsoft.Graphics.Canvas.UI.Composition;
import winrt.Microsoft.UI.Composition;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Numerics;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::Graphics::Canvas;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// Helper Brush class to interop with Win2D Canvas calls.
    /// </summary>
    struct CanvasBrushBase : CanvasBrushBaseT<CanvasBrushBase>
    {
        CanvasBrushBase() = default;

        /// <summary>
        /// Gets or sets the internal surface render width.  Modify during construction.
        /// </summary>
        float SurfaceWidth() const { return _surfaceWidth; }
        void SurfaceWidth(float value) { _surfaceWidth = value; }

        /// <summary>
        /// Gets or sets the internal surface render height.  Modify during construction.
        /// </summary>
        float SurfaceHeight() const { return _surfaceHeight; }
        void SurfaceHeight(float value) { _surfaceHeight = value; }

        /// <summary>
        /// Implemented by parent class and called when canvas is being constructed for brush.
        /// </summary>
        /// <param name="device">Canvas device.</param>
        /// <param name="session">Canvas drawing session.</param>
        /// <param name="size">Size of surface to draw on.</param>
        /// <returns>True if drawing was completed and the brush is ready, otherwise return False to not create brush yet.</returns>
        virtual bool OnDraw(
            [[maybe_unused]] winrt::CanvasDevice const& device,
            [[maybe_unused]] winrt::CanvasDrawingSession const& session,
            [[maybe_unused]] winrt::float2 const& size)
        {
            return true;
        }

        void OnConnected();
        void OnDisconnected();

    private:
        float _surfaceWidth{ 100.0f };
        float _surfaceHeight{ 100.0f };

        winrt::CanvasDevice _device{ nullptr };
        winrt::CompositionGraphicsDevice _graphics{ nullptr };
        winrt::CompositionSurfaceBrush _surfaceBrush{ nullptr };

        winrt::event_token _deviceLostToken;
        winrt::event_token _renderingDeviceReplacedToken;

        void OnDeviceLost(
            winrt::CanvasDevice const& sender,
            winrt::IInspectable const& args);

        void OnRenderingDeviceReplaced(
            winrt::CompositionGraphicsDevice const& sender,
            winrt::IInspectable const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct CanvasBrushBase : CanvasBrushBaseT<CanvasBrushBase, implementation::CanvasBrushBase>
    {
    };
}