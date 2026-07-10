// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "ImageBlendBrush.h"
#if __has_include("ImageBlendBrush.g.cpp")
#include "ImageBlendBrush.g.cpp"
#endif
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>
#else
import winrt.Microsoft.UI.Xaml.Media.Imaging;
import winrt.Microsoft.Graphics.Canvas.Effects;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
    using namespace winrt::Microsoft::Graphics::Canvas::Effects;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ImageBlendBrush::SourceProperty =
        winrt::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<ImageSource>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ nullptr, &ImageBlendBrush::OnSourceChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> ImageBlendBrush::StretchProperty =
        winrt::DependencyProperty::Register(
            L"Stretch",
            winrt::xaml_typename<winrt::Stretch>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(Stretch::None), &ImageBlendBrush::OnStretchChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> ImageBlendBrush::ModeProperty =
        winrt::DependencyProperty::Register(
            L"Mode",
            winrt::xaml_typename<ImageBlendMode>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata{ winrt::box_value(ImageBlendMode::Multiply), &ImageBlendBrush::OnModeChanged });

    winrt::ImageSource ImageBlendBrush::Source() const
    { 
        return GetValue(SourceProperty()).try_as<winrt::ImageSource>();
    }

    void ImageBlendBrush::Source(ImageSource const& value)
    {
        SetValue(SourceProperty(), value);
    }

    winrt::Stretch ImageBlendBrush::Stretch() const 
    { 
        return winrt::unbox_value<winrt::Stretch>(GetValue(StretchProperty()));
    }

    void ImageBlendBrush::Stretch(winrt::Stretch value)
    {
        SetValue(StretchProperty(), winrt::box_value(value));
    }

	ImageBlendMode ImageBlendBrush::Mode() const
	{
		return winrt::unbox_value<ImageBlendMode>(GetValue(ModeProperty()));
	}

    void ImageBlendBrush::Mode(ImageBlendMode value)
    {
        SetValue(ModeProperty(), winrt::box_value(value));
    }

    void ImageBlendBrush::OnSourceChanged(
        winrt::DependencyObject const& d,
        winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        if (brush->_surfaceBrush)
        {
            auto bitmap = e.NewValue().try_as<winrt::BitmapImage>();
            auto uri = bitmap ? bitmap.UriSource() : winrt::Uri{ L"ms-appx:///" };
            auto newSurface = winrt::LoadedImageSurface::StartLoadFromUri(uri);

            brush->_surface = newSurface;
            brush->_surfaceBrush.Surface(newSurface);
        }
        else
        {
            // If we didn't initially have a valid surface, we need to recreate our effect now
            brush->OnDisconnected();
            brush->OnConnected();
        }
    }

    void ImageBlendBrush::OnStretchChanged(
        winrt::DependencyObject const& d,
        winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        // Unbox and update surface if CompositionBrush exists
        if (brush->_surfaceBrush)
        {
            // Modify the stretch property on our brush.
			auto stretchValue = winrt::unbox_value<winrt::Stretch>(e.NewValue());
            brush->_surfaceBrush.Stretch(CompositionStretchFromStretch(stretchValue));
        }
    }

    void ImageBlendBrush::OnModeChanged(
        winrt::DependencyObject const& d,
        [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        // We can't animate our enum properties so recreate our internal brush
        brush->OnDisconnected();
        brush->OnConnected();
    }

    winrt::CompositionStretch ImageBlendBrush::CompositionStretchFromStretch(winrt::Stretch value)
    {
        switch (value)
        {
        case winrt::Stretch::None:
            return winrt::CompositionStretch::None;
        case winrt::Stretch::Fill:
            return winrt::CompositionStretch::Fill;
        case winrt::Stretch::Uniform:
            return winrt::CompositionStretch::Uniform;
        case winrt::Stretch::UniformToFill:
            return winrt::CompositionStretch::UniformToFill;
        }
        return winrt::CompositionStretch::None;
    }

    void ImageBlendBrush::OnConnected()
    {
        auto compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

        // Delay creating composition resources until they're required.
        if (CompositionBrush() == nullptr && Source())
        {
            auto bitmap = Source().try_as<winrt::BitmapImage>();
            if (!bitmap)
            {
                return;
            }

            // Use LoadedImageSurface API to get ICompositionSurface from image uri
            _surface = winrt::LoadedImageSurface::StartLoadFromUri(bitmap.UriSource());

            // Load Surface onto SurfaceBrush
            _surfaceBrush = compositor.CreateSurfaceBrush(_surface);
            _surfaceBrush.Stretch(CompositionStretchFromStretch(Stretch()));

            // Check if effects are supported
            winrt::CompositionCapabilities compositionCapabilities;
            if (!compositionCapabilities.AreEffectsSupported())
            {
                // Just use image straight-up, if we don't support effects
                CompositionBrush(_surfaceBrush);
                return;
            }

            auto backdrop = compositor.CreateBackdropBrush();

            // Use a Win2D Blend affect applied to a CompositionBackdropBrush.
            winrt::BlendEffect graphicsEffect;
            graphicsEffect.Name(L"Blend");
            graphicsEffect.Mode(static_cast<winrt::BlendEffectMode>(Mode()));
            graphicsEffect.Background(winrt::CompositionEffectSourceParameter{ L"backdrop" });
            graphicsEffect.Foreground(winrt::CompositionEffectSourceParameter{ L"image" });

            auto effectFactory = compositor.CreateEffectFactory(graphicsEffect);
            auto effectBrush = effectFactory.CreateBrush();

            effectBrush.SetSourceParameter(L"backdrop", backdrop);
            effectBrush.SetSourceParameter(L"image", _surfaceBrush);

            CompositionBrush(effectBrush);
        }
    }

    void ImageBlendBrush::OnDisconnected()
    {
        // Dispose of composition resources when no longer in use
        if (auto brush = CompositionBrush())
        {
            brush.Close();
            CompositionBrush(nullptr);
        }

        if (_surfaceBrush)
        {
            _surfaceBrush.Close();
            _surfaceBrush = nullptr;
        }

        if (_surface)
        {
            _surface.Close();
            _surface = nullptr;
        }
    }
}