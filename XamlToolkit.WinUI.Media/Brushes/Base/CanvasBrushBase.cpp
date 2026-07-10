// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "CanvasBrushBase.h"
#if __has_include("CanvasBrushBase.g.cpp")
#include "CanvasBrushBase.g.cpp"
#endif

namespace winrt 
{
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace Microsoft::Graphics::Canvas::UI::Composition;
    using namespace Microsoft::Graphics::DirectX;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    void CanvasBrushBase::OnConnected()
    {
        // Unregister previous device events if any
        if (_device)
        {
            _device.DeviceLost(_deviceLostToken);
        }

        // Get shared Canvas device
        _device = winrt::CanvasDevice::GetSharedDevice();
        _deviceLostToken = _device.DeviceLost({ this, &CanvasBrushBase::OnDeviceLost });

        // Unregister previous graphics events if any
        if (_graphics)
        {
            _graphics.RenderingDeviceReplaced(_renderingDeviceReplacedToken);
        }

        // Create CompositionGraphicsDevice
        auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();
        _graphics = winrt::CanvasComposition::CreateCompositionGraphicsDevice(compositor, _device);
        _renderingDeviceReplacedToken = _graphics.RenderingDeviceReplaced({ this, &CanvasBrushBase::OnRenderingDeviceReplaced });

        // Delay creating composition resources until they're required
        if (CompositionBrush() == nullptr)
        {
            // Check if effects are supported
            winrt::CompositionCapabilities compositionCapabilities;
            if (!compositionCapabilities.AreEffectsSupported())
            {
                return;
            }

            winrt::float2 size{ _surfaceWidth, _surfaceHeight };
            auto surface = _graphics.CreateDrawingSurface(
                winrt::Size{ _surfaceWidth, _surfaceHeight },
                winrt::DirectXPixelFormat::B8G8R8A8UIntNormalized,
                winrt::DirectXAlphaMode::Premultiplied);

            // Create drawing session and call OnDraw
            auto session = winrt::CanvasComposition::CreateDrawingSession(surface);
            if (!OnDraw(_device, session, size))
            {
                return;
            }

            _surfaceBrush = compositor.CreateSurfaceBrush(surface);
            _surfaceBrush.Stretch(winrt::CompositionStretch::Fill);

            CompositionBrush(_surfaceBrush);
        }
    }

    void CanvasBrushBase::OnDisconnected()
    {
        // Unregister device events
        if (_device)
        {
            _device.DeviceLost(_deviceLostToken);
            _device = nullptr;
        }

        // Unregister graphics events
        if (_graphics)
        {
            _graphics.RenderingDeviceReplaced(_renderingDeviceReplacedToken);
            _graphics = nullptr;
        }

        // Dispose composition resources
        if (CompositionBrush())
        {
            CompositionBrush(nullptr);
        }

        if (_surfaceBrush)
        {
            _surfaceBrush.Close();
            _surfaceBrush = nullptr;
        }
    }

    void CanvasBrushBase::OnDeviceLost([[maybe_unused]] winrt::CanvasDevice const& sender, [[maybe_unused]] winrt::IInspectable const& args)
    {
        OnDisconnected();
        OnConnected();
    }

    void CanvasBrushBase::OnRenderingDeviceReplaced([[maybe_unused]] winrt::CompositionGraphicsDevice const& sender, [[maybe_unused]] winrt::IInspectable const& args)
    {
        OnDisconnected();
        OnConnected();
    }
}