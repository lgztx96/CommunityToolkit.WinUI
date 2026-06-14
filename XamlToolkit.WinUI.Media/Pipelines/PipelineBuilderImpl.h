// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>
#include <winrt/Windows.Graphics.Effects.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <winrt/Windows.UI.h>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#endif

import winrt.XamlToolkit.WinUI.Media;
import winrt.Windows.UI;
import winrt.Windows.Graphics.Effects;
import winrt.Microsoft.UI.Composition;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Graphics::Effects;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Media::Pipelines
{
    // Win2D effect aliases (matching C# version to avoid conflicts with custom effects)
    namespace CanvasEffects = winrt::Microsoft::Graphics::Canvas::Effects;

    using CanvasGaussianBlurEffect = CanvasEffects::GaussianBlurEffect;
    using CanvasSaturationEffect = CanvasEffects::SaturationEffect;
    using CanvasSepiaEffect = CanvasEffects::SepiaEffect;
    using CanvasOpacityEffect = CanvasEffects::OpacityEffect;
    using CanvasExposureEffect = CanvasEffects::ExposureEffect;
    using CanvasHueRotationEffect = CanvasEffects::HueRotationEffect;
    using CanvasTintEffect = CanvasEffects::TintEffect;
    using CanvasTemperatureAndTintEffect = CanvasEffects::TemperatureAndTintEffect;
    using CanvasLuminanceToAlphaEffect = CanvasEffects::LuminanceToAlphaEffect;
    using CanvasInvertEffect = CanvasEffects::InvertEffect;
    using CanvasGrayscaleEffect = CanvasEffects::GrayscaleEffect;
    using CanvasBorderEffect = CanvasEffects::BorderEffect;
    using CanvasColorSourceEffect = CanvasEffects::ColorSourceEffect;
    using CanvasBlendEffect = CanvasEffects::BlendEffect;
    using CanvasCrossFadeEffect = CanvasEffects::CrossFadeEffect;
    using EffectBorderMode = CanvasEffects::EffectBorderMode;
    using EffectOptimization = CanvasEffects::EffectOptimization;
    using BlendEffectMode = CanvasEffects::BlendEffectMode;

    /// <summary>
    /// A delegate that represents a custom effect property setter that can be applied to a CompositionBrush
    /// </summary>
    /// <typeparam name="T">The type of property value to set</typeparam>
    template<typename T>
    using EffectSetter = std::function<void(CompositionBrush const& brush, T const& value)>;

    /// <summary>
    /// A delegate that represents a custom effect property animation that can be applied to a CompositionBrush
    /// </summary>
    /// <typeparam name="T">The type of property value to animate</typeparam>
    template<typename T>
    using EffectAnimation = std::function<IAsyncAction(CompositionBrush const& brush, T const& value, TimeSpan const& duration)>;

    /// <summary>
    /// A class that allows to build custom effects pipelines and create CompositionBrush instances from them
    /// This is a pure C++ implementation (no IDL) following the AnimationBuilder pattern.
    /// </summary>
    class PipelineBuilderImpl
    {
    private:
        /// <summary>
        /// Internal shared state of a PipelineBuilderImpl, managed via shared_ptr
        /// for reference semantics matching the C# GC-based ownership model.
        /// </summary>
        struct State
        {
            std::function<IAsyncOperation<IGraphicsEffectSource>()> _sourceProducer;
            std::vector<winrt::hstring> _animationProperties;
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> _lazyParameters;
        };

        /// <summary>
        /// Shared ownership of the pipeline state. Always non-null — initialized
        /// to an empty State to support default construction (required by WinRT wrappers).
        /// </summary>
        std::shared_ptr<State> _state = std::make_shared<State>();

    public:
        /// <summary>
        /// Default constructor — creates a PipelineBuilderImpl with an empty (but valid) Impl.
        /// Required for WinRT activation and transient variable initialization.
        /// </summary>
        PipelineBuilderImpl() = default;

        PipelineBuilderImpl(std::shared_ptr<State> state) noexcept : _state(state) {}

    private:
        /// <summary>
        /// Leaf constructor: starts a pipeline from a CompositionBrush factory.
        /// Used by FromBackdrop(), FromBrush(), FromImage(), FromUIElement().
        /// </summary>
        PipelineBuilderImpl(std::function<IAsyncOperation<CompositionBrush>()> factory);

        /// <summary>
        /// Leaf constructor: starts a pipeline with explicit IGraphicsEffectSource factory,
        /// animation properties, and lazy parameters.
        /// Used by FromColor(), FromHdrColor(), FromEffect().
        /// </summary>
        PipelineBuilderImpl(
            std::function<IAsyncOperation<IGraphicsEffectSource>()> factory,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> lazy = {});

        /// <summary>
        /// Single-parent constructor: attaches a new effect to an existing pipeline.
        /// The parentState shared_ptr keeps the parent pipeline alive for the lifetime
        /// of the new factory closure.
        /// </summary>
        PipelineBuilderImpl(
            PipelineBuilderImpl const& source,
            std::function<IAsyncOperation<IGraphicsEffectSource>()> factory,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> lazy = {});

        /// <summary>
        /// Dual-parent constructor: merges two pipelines (Blend, CrossFade, Merge).
        /// Both parent Impls are kept alive through shared_ptr captures in the factory closure.
        /// </summary>
        PipelineBuilderImpl(
            std::function<IAsyncOperation<IGraphicsEffectSource>()> factory,
            PipelineBuilderImpl const& a,
            PipelineBuilderImpl const& b,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> lazy = {});

        /// <summary>
        /// Generates a unique uppercase ASCII letters ID from a GUID
        /// </summary>
        static winrt::hstring GenerateId();

        /// <summary>
        /// Helper to merge animation properties
        /// </summary>
        template <typename T>
        static std::vector<T> Merge(std::vector<T> const& a, std::vector<T> const& b)
        {
            for (auto const& item : a)
            {
                if (std::find(b.begin(), b.end(), item) != b.end())
                {
                    throw std::runtime_error(
                        "The input collection has at least an item already present in the second collection");
                }
            }

            std::vector<T> result;
            result.reserve(a.size() + b.size());

            result.append_range(a);
            result.append_range(b);

            return result;
        }

        /// <summary>
        /// Helper to merge lazy parameters
        /// </summary>
        template <typename K, typename V>
        static std::unordered_map<K, V> Merge(std::unordered_map<K, V> const& a, std::unordered_map<K, V> const& b)
        {
            std::unordered_map<K, V> result{ a };

            for (auto const& [key, value] : b)
            {
                auto [_, inserted] = result.emplace(key, value);

                if (!inserted)
                {
                    throw std::runtime_error("The key already exists in the current pipeline");
                }
            }

            return result;
        }

    public:
        // ===== Initialization Methods (PipelineBuilder.Initialization.cs) =====

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the CompositionBrush returned by Compositor.CreateBackdropBrush
        /// </summary>
        static PipelineBuilderImpl FromBackdrop();

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified color
        /// </summary>
        static PipelineBuilderImpl FromColor(winrt::Windows::UI::Color color);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified color
        /// </summary>
        static PipelineBuilderImpl FromColor(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& setter);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified color
        /// </summary>
        static PipelineBuilderImpl FromColor(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& animation);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified HDR color
        /// </summary>
        static PipelineBuilderImpl FromHdrColor(winrt::Windows::Foundation::Numerics::float4 color);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified HDR color
        /// </summary>
        static PipelineBuilderImpl FromHdrColor(winrt::Windows::Foundation::Numerics::float4 color, EffectSetter<winrt::Windows::Foundation::Numerics::float4>& setter);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a solid CompositionBrush with the specified HDR color
        /// </summary>
        static PipelineBuilderImpl FromHdrColor(winrt::Windows::Foundation::Numerics::float4 color, EffectAnimation<winrt::Windows::Foundation::Numerics::float4>& animation);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input CompositionBrush instance
        /// </summary>
        static PipelineBuilderImpl FromBrush(CompositionBrush const& brush);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input CompositionBrush instance
        /// </summary>
        static PipelineBuilderImpl FromBrush(std::function<CompositionBrush()> factory);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input CompositionBrush instance
        /// </summary>
        static PipelineBuilderImpl FromBrush(std::function<IAsyncOperation<CompositionBrush>()> factory);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input IGraphicsEffectSource instance
        /// </summary>
        static PipelineBuilderImpl FromEffect(IGraphicsEffectSource const& effect);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input IGraphicsEffectSource instance
        /// </summary>
        static PipelineBuilderImpl FromEffect(std::function<IGraphicsEffectSource()> factory);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the input IGraphicsEffectSource instance
        /// </summary>
        static PipelineBuilderImpl FromEffect(std::function<IAsyncOperation<IGraphicsEffectSource>()> factory);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a Win2D image
        /// </summary>
        static PipelineBuilderImpl FromImage(winrt::hstring const& relativePath, Media::DpiMode dpiMode = Media::DpiMode::DisplayDpiWith96AsLowerBound, Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a Win2D image
        /// </summary>
        static PipelineBuilderImpl FromImage(Uri const& uri, Media::DpiMode dpiMode = Media::DpiMode::DisplayDpiWith96AsLowerBound, Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a Win2D image tiled to cover the available space
        /// </summary>
        static PipelineBuilderImpl FromTiles(winrt::hstring const& relativePath, Media::DpiMode dpiMode = Media::DpiMode::DisplayDpiWith96AsLowerBound, Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from a Win2D image tiled to cover the available space
        /// </summary>
        static PipelineBuilderImpl FromTiles(Uri const& uri, Media::DpiMode dpiMode = Media::DpiMode::DisplayDpiWith96AsLowerBound, Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Starts a new PipelineBuilder pipeline from the CompositionBrush returned by Compositor.CreateBackdropBrush on the input UIElement
        /// </summary>
        static PipelineBuilderImpl FromUIElement(UIElement const& element);

        // ===== Effects Methods (PipelineBuilder.Effects.cs) =====

        /// <summary>
        /// Adds a new GaussianBlurEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Blur(float blur, EffectBorderMode mode = EffectBorderMode::Hard, EffectOptimization optimization = EffectOptimization::Balanced) const;

        /// <summary>
        /// Adds a new GaussianBlurEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Blur(float blur, EffectSetter<float>& setter, EffectBorderMode mode = EffectBorderMode::Hard, EffectOptimization optimization = EffectOptimization::Balanced) const;

        /// <summary>
        /// Adds a new GaussianBlurEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Blur(float blur, EffectAnimation<float>& animation, EffectBorderMode mode = EffectBorderMode::Hard, EffectOptimization optimization = EffectOptimization::Balanced) const;

        /// <summary>
        /// Adds a new GaussianBlurEffect to the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Blur(float blur, winrt::hstring& target, EffectBorderMode mode = EffectBorderMode::Hard, EffectOptimization optimization = EffectOptimization::Balanced) const;

        /// <summary>
        /// Adds a new SaturationEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Saturation(float saturation) const;

        /// <summary>
        /// Adds a new SaturationEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Saturation(float saturation, EffectSetter<float>& setter) const;

        /// <summary>
        /// Adds a new SaturationEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Saturation(float saturation, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Adds a new SaturationEffect to the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Saturation(float saturation, winrt::hstring& target) const;

        /// <summary>
        /// Adds a new SepiaEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Sepia(float intensity) const;

        /// <summary>
        /// Adds a new SepiaEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Sepia(float intensity, EffectSetter<float>& stetter) const;

        /// <summary>
        /// Adds a new SepiaEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Sepia(float intensity, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Adds a new SepiaEffect to the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Sepia(float intensity, winrt::hstring& target) const;

        /// <summary>
        /// Adds a new OpacityEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Opacity(float opacity) const;

        /// <summary>
        /// Adds a new OpacityEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Opacity(float opacity, EffectSetter<float>& setter) const;

        /// <summary>
        /// Adds a new OpacityEffect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Opacity(float opacity, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Adds a new OpacityEffect to the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Opacity(float opacity, winrt::hstring& target) const;

        /// <summary>
        /// Applies an exposure effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Exposure(float amount) const;

        /// <summary>
        /// Applies an exposure effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Exposure(float amount, EffectSetter<float>& setter) const;

        /// <summary>
        /// Applies an exposure effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Exposure(float amount, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Applies an exposure effect on the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Exposure(float amount, winrt::hstring& target) const;

        /// <summary>
        /// Applies a hue rotation effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl HueRotation(float angle) const;

        /// <summary>
        /// Applies a hue rotation effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl HueRotation(float angle, EffectSetter<float>& setter) const;

        /// <summary>
        /// Applies a hue rotation effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl HueRotation(float angle, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Applies a hue rotation effect on the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl HueRotation(float angle, winrt::hstring& target) const;

        /// <summary>
        /// Applies a tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Tint(winrt::Windows::UI::Color color) const;

        /// <summary>
        /// Applies a tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Tint(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& setter) const;

        /// <summary>
        /// Applies a tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Tint(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& animation) const;

        /// <summary>
        /// Applies a tint effect on the current pipeline (internal with target output)
        /// </summary>
        PipelineBuilderImpl Tint(winrt::Windows::UI::Color color, winrt::hstring& target) const;

        /// <summary>
        /// Applies a temperature and tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl TemperatureAndTint(float temperature, float tint) const;

        /// <summary>
        /// Applies a temperature and tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl TemperatureAndTint(float temperature, EffectSetter<float>& temperatureSetter, float tint, EffectSetter<float>& tintSetter) const;

        /// <summary>
        /// Applies a temperature and tint effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl TemperatureAndTint(float temperature, EffectAnimation<float>& temperatureAnimation, float tint, EffectAnimation<float>& tintAnimation) const;

        /// <summary>
        /// Applies a shade effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Shade(winrt::Windows::UI::Color color, float mix) const;

        /// <summary>
        /// Applies a shade effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Shade(winrt::Windows::UI::Color color, EffectSetter<winrt::Windows::UI::Color>& colorSetter, float mix, EffectSetter<float>& mixSetter) const;

        /// <summary>
        /// Applies a shade effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Shade(winrt::Windows::UI::Color color, EffectAnimation<winrt::Windows::UI::Color>& colorAnimation, float mix, EffectAnimation<float>& mixAnimation) const;

        /// <summary>
        /// Applies a luminance to alpha effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl LuminanceToAlpha() const;

        /// <summary>
        /// Applies an invert effect on the current pipeline
        /// </summary>
        PipelineBuilderImpl Invert() const;

        /// <summary>
        /// Applies a grayscale on the current pipeline
        /// </summary>
        PipelineBuilderImpl Grayscale() const;

        /// <summary>
        /// Applies a custom effect to the current pipeline
        /// </summary>
        PipelineBuilderImpl Effect(
            std::function<IGraphicsEffectSource(IGraphicsEffectSource const&)> factory,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers = {}) const;

        /// <summary>
        /// Applies a custom effect to the current pipeline (async)
        /// </summary>
        PipelineBuilderImpl Effect(
            std::function<IAsyncOperation<IGraphicsEffectSource>(IGraphicsEffectSource const&)> factory,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers = {}) const;

        // ===== Merge Methods (PipelineBuilder.Merge.cs) =====

        /// <summary>
        /// Blends two pipelines using a BlendEffect instance with the specified mode
        /// </summary>
        PipelineBuilderImpl Blend(PipelineBuilderImpl const& pipeline, BlendEffectMode mode, Media::Placement placement = Media::Placement::Foreground) const;

        /// <summary>
        /// Cross fades two pipelines using a CrossFadeEffect instance
        /// </summary>
        PipelineBuilderImpl CrossFade(PipelineBuilderImpl const& pipeline, float factor = 0.5f) const;

        /// <summary>
        /// Cross fades two pipelines using a CrossFadeEffect instance
        /// </summary>
        PipelineBuilderImpl CrossFade(PipelineBuilderImpl const& pipeline, float factor, EffectSetter<float>& setter) const;

        /// <summary>
        /// Cross fades two pipelines using a CrossFadeEffect instance
        /// </summary>
        PipelineBuilderImpl CrossFade(PipelineBuilderImpl const& pipeline, float factor, EffectAnimation<float>& animation) const;

        /// <summary>
        /// Cross fades two pipelines using a CrossFadeEffect instance (internal with target output)
        /// </summary>
        PipelineBuilderImpl CrossFade(PipelineBuilderImpl const& pipeline, float factor, winrt::hstring& target) const;

        /// <summary>
        /// Blends two pipelines using the provided function to do so
        /// </summary>
        PipelineBuilderImpl Merge(
            std::function<IGraphicsEffectSource(IGraphicsEffectSource const&, IGraphicsEffectSource const&)> factory,
            PipelineBuilderImpl const& background,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers = {}) const;

        /// <summary>
        /// Blends two pipelines using the provided asynchronous function to do so
        /// </summary>
        PipelineBuilderImpl Merge(
            std::function<IAsyncOperation<IGraphicsEffectSource>(IGraphicsEffectSource const&, IGraphicsEffectSource const&)> factory,
            PipelineBuilderImpl const& background,
            std::vector<winrt::hstring> animations = {},
            std::unordered_map<winrt::hstring, std::function<IAsyncOperation<CompositionBrush>()>> initializers = {}) const;

        // ===== Prebuilt Methods (PipelineBuilder.Prebuilt.cs) =====

        /// <summary>
        /// Returns a new PipelineBuilder instance that implements the in-app backdrop acrylic effect
        /// </summary>
        static PipelineBuilderImpl FromBackdropAcrylic(
            winrt::Windows::UI::Color tintColor,
            float tintOpacity,
            float blurAmount,
            Uri const& noiseUri,
            Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Returns a new PipelineBuilder instance that implements the in-app backdrop acrylic effect
        /// </summary>
        static PipelineBuilderImpl FromBackdropAcrylic(
            winrt::Windows::UI::Color tintColor,
            EffectSetter<winrt::Windows::UI::Color>& tintColorSetter,
            float tintOpacity,
            EffectSetter<float>& tintOpacitySetter,
            float blurAmount,
            EffectSetter<float>& blurAmountSetter,
            Uri const& noiseUri,
            Media::CacheMode cacheMode = Media::CacheMode::Default);

        /// <summary>
        /// Returns a new PipelineBuilder instance that implements the in-app backdrop acrylic effect
        /// </summary>
        static PipelineBuilderImpl FromBackdropAcrylic(
            winrt::Windows::UI::Color tintColor,
            EffectAnimation<winrt::Windows::UI::Color>& tintAnimation,
            float tintOpacity,
            EffectAnimation<float>& tintOpacityAnimation,
            float blurAmount,
            EffectAnimation<float>& blurAmountAnimation,
            Uri const& noiseUri,
            Media::CacheMode cacheMode = Media::CacheMode::Default);

        // ===== Build Methods =====

        /// <summary>
        /// Builds a CompositionBrush instance from the current effects pipeline
        /// </summary>
        IAsyncOperation<CompositionBrush> BuildAsync();

        /// <summary>
        /// Builds the current pipeline and creates a SpriteVisual that is applied to the input UIElement
        /// </summary>
        IAsyncOperation<SpriteVisual> AttachAsync(UIElement const& target, UIElement const& reference = nullptr);

        winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush AsBrush() const;
    };
}