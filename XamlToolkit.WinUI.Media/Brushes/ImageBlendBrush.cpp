// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "ImageBlendBrush.h"
#if __has_include("ImageBlendBrush.g.cpp")
#include "ImageBlendBrush.g.cpp"
#endif

import winrt.Microsoft.UI.Xaml.Media.Imaging;
import winrt.Microsoft.Graphics.Canvas.Effects;

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Media::Imaging;
    using namespace Microsoft::Graphics::Canvas::Effects;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ImageBlendBrush::SourceProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<ImageSource>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &ImageBlendBrush::OnSourceChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ImageBlendBrush::StretchProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Stretch",
            winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::Stretch>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(Stretch::None), &ImageBlendBrush::OnStretchChanged });

    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ImageBlendBrush::ModeProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Mode",
            winrt::xaml_typename<ImageBlendMode>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(ImageBlendMode::Multiply), &ImageBlendBrush::OnModeChanged });

    winrt::Microsoft::UI::Xaml::Media::ImageSource ImageBlendBrush::Source() const
    { 
        return GetValue(SourceProperty()).try_as<winrt::Microsoft::UI::Xaml::Media::ImageSource>();
    }

    void ImageBlendBrush::Source(ImageSource const& value)
    {
        SetValue(SourceProperty(), value);
    }

    winrt::Microsoft::UI::Xaml::Media::Stretch ImageBlendBrush::Stretch() const 
    { 
        return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Stretch>(GetValue(StretchProperty()));
    }

    void ImageBlendBrush::Stretch(winrt::Microsoft::UI::Xaml::Media::Stretch value)
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
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        if (brush->_surfaceBrush)
        {
            auto bitmap = e.NewValue().try_as<BitmapImage>();
            auto uri = bitmap ? bitmap.UriSource() : winrt::Windows::Foundation::Uri{ L"ms-appx:///" };
            auto newSurface = LoadedImageSurface::StartLoadFromUri(uri);

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
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        // Unbox and update surface if CompositionBrush exists
        if (brush->_surfaceBrush)
        {
            // Modify the stretch property on our brush.
			auto stretchValue = winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Stretch>(e.NewValue());
            brush->_surfaceBrush.Stretch(CompositionStretchFromStretch(stretchValue));
        }
    }

    void ImageBlendBrush::OnModeChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<ImageBlendBrush>();

        // We can't animate our enum properties so recreate our internal brush
        brush->OnDisconnected();
        brush->OnConnected();
    }

    CompositionStretch ImageBlendBrush::CompositionStretchFromStretch(winrt::Microsoft::UI::Xaml::Media::Stretch value)
    {
        switch (value)
        {
        case Stretch::None:
            return CompositionStretch::None;
        case Stretch::Fill:
            return CompositionStretch::Fill;
        case Stretch::Uniform:
            return CompositionStretch::Uniform;
        case Stretch::UniformToFill:
            return CompositionStretch::UniformToFill;
        }
        return CompositionStretch::None;
    }

    void ImageBlendBrush::OnConnected()
    {
        auto compositor = CompositionTarget::GetCompositorForCurrentThread();

        // Delay creating composition resources until they're required.
        if (CompositionBrush() == nullptr && Source())
        {
            auto bitmap = Source().try_as<BitmapImage>();
            if (!bitmap)
            {
                return;
            }

            // Use LoadedImageSurface API to get ICompositionSurface from image uri
            _surface = LoadedImageSurface::StartLoadFromUri(bitmap.UriSource());

            // Load Surface onto SurfaceBrush
            _surfaceBrush = compositor.CreateSurfaceBrush(_surface);
            _surfaceBrush.Stretch(CompositionStretchFromStretch(Stretch()));

            // Check if effects are supported
            CompositionCapabilities compositionCapabilities;
            if (!compositionCapabilities.AreEffectsSupported())
            {
                // Just use image straight-up, if we don't support effects
                CompositionBrush(_surfaceBrush);
                return;
            }

            auto backdrop = compositor.CreateBackdropBrush();

            // Use a Win2D Blend affect applied to a CompositionBackdropBrush.
            winrt::Microsoft::Graphics::Canvas::Effects::BlendEffect graphicsEffect;
            graphicsEffect.Name(L"Blend");
            graphicsEffect.Mode(static_cast<BlendEffectMode>(Mode()));
            graphicsEffect.Background(winrt::Microsoft::UI::Composition::CompositionEffectSourceParameter{ L"backdrop" });
            graphicsEffect.Foreground(winrt::Microsoft::UI::Composition::CompositionEffectSourceParameter{ L"image" });

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