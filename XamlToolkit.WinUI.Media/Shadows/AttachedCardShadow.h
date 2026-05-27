#pragma once

#include "AttachedCardShadow.g.h"
#include "../XamlToolkit.WinUI/common.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::Graphics::Canvas::Geometry;
    using namespace Windows::Foundation::Numerics;
    using namespace XamlToolkit::WinUI;
    using namespace XamlToolkit::WinUI::Media;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    struct AttachedCardShadow : AttachedCardShadowT<AttachedCardShadow>
    {
        static constexpr float MaxBlurRadius = 72;

        static constexpr TypedResourceKey<CompositionGeometricClip> ClipResourceKey{ L"Clip" };

        static constexpr TypedResourceKey<CompositionPathGeometry> PathGeometryResourceKey{ L"PathGeometry" };

        static constexpr TypedResourceKey<CompositionMaskBrush> OpacityMaskBrushResourceKey{ L"OpacityMask" };

        static constexpr TypedResourceKey<ShapeVisual> OpacityMaskShapeVisualResourceKey{ L"OpacityMaskShapeVisual" };

        static constexpr TypedResourceKey<CompositionRoundedRectangleGeometry> OpacityMaskGeometryResourceKey{ L"OpacityMaskGeometry" };

        static constexpr TypedResourceKey<CompositionSpriteShape> OpacityMaskSpriteShapeResourceKey{ L"OpacityMaskSpriteShape" };

        static constexpr TypedResourceKey<CompositionVisualSurface> OpacityMaskShapeVisualSurfaceResourceKey{ L"OpacityMaskShapeVisualSurface" };

        static constexpr TypedResourceKey<CompositionSurfaceBrush> OpacityMaskShapeVisualSurfaceBrushResourceKey{ L"OpacityMaskShapeVisualSurfaceBrush" };

        static constexpr TypedResourceKey<CompositionVisualSurface> OpacityMaskVisualSurfaceResourceKey{ L"OpacityMaskVisualSurface" };

        static constexpr TypedResourceKey<CompositionSurfaceBrush> OpacityMaskSurfaceBrushResourceKey{ L"OpacityMaskSurfaceBrush" };

        static constexpr TypedResourceKey<SpriteVisual> OpacityMaskVisualResourceKey{ L"OpacityMaskVisual" };

        static constexpr TypedResourceKey<CompositionRoundedRectangleGeometry> RoundedRectangleGeometryResourceKey{ L"RoundedGeometry" };

        static constexpr TypedResourceKey<CompositionSpriteShape> ShapeResourceKey{ L"Shape" };

        static constexpr TypedResourceKey<ShapeVisual> ShapeVisualResourceKey{ L"ShapeVisual" };

        static constexpr TypedResourceKey<CompositionSurfaceBrush> SurfaceBrushResourceKey{ L"SurfaceBrush" };

        static constexpr TypedResourceKey<CompositionVisualSurface> VisualSurfaceResourceKey{ L"VisualSurface" };

        AttachedCardShadow() = default;

        static inline const wil::single_threaded_property<DependencyProperty> CornerRadiusProperty =
            DependencyProperty::Register(
                L"CornerRadius",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(4.0), AttachedShadowBase::OnDependencyPropertyChanged));


        static inline const wil::single_threaded_property<DependencyProperty> InnerContentClipModeProperty =
            DependencyProperty::Register(
                L"InnerContentClipMode",
                winrt::xaml_typename<enum InnerContentClipMode>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(InnerContentClipMode::CompositionGeometricClip), AttachedShadowBase::OnDependencyPropertyChanged));

        double CornerRadius() const { return winrt::unbox_value<double>(GetValue(CornerRadiusProperty)); }
        void CornerRadius(double value) const { SetValue(CornerRadiusProperty, winrt::box_value(value)); }

        XamlToolkit::WinUI::Media::InnerContentClipMode InnerContentClipMode() const { return winrt::unbox_value<XamlToolkit::WinUI::Media::InnerContentClipMode>(GetValue(InnerContentClipModeProperty)); }
        void InnerContentClipMode(XamlToolkit::WinUI::Media::InnerContentClipMode value) const { SetValue(InnerContentClipModeProperty, winrt::box_value(value)); }

        bool SupportsOnSizeChangedEvent() const noexcept { return true; }

        void OnElementContextInitialized(AttachedShadowElementContext const& context);

        CompositionBrush GetShadowMask(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

        CompositionClip GetShadowClip(XamlToolkit::WinUI::AttachedShadowElementContext const& context) const;

        void UpdateVisualOpacityMask(AttachedShadowElementContext const& context) const;

        void OnPropertyChanged(AttachedShadowElementContext const& context,
            DependencyProperty const& property,
            winrt::Windows::Foundation::IInspectable const& oldValue,
            winrt::Windows::Foundation::IInspectable const& newValue);

        void SetElementChildVisual(AttachedShadowElementContext const& context);

        void OnSizeChanged(AttachedShadowElementContext const& context, Windows::Foundation::Size newSize, Windows::Foundation::Size previousSize);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct AttachedCardShadow : AttachedCardShadowT<AttachedCardShadow, implementation::AttachedCardShadow>
    {
    };
}
