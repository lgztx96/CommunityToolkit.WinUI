#include "pch.h"
#include "winrt_module_imports.h"
#include "AttachedCardShadow.h"
#if __has_include("AttachedCardShadow.g.cpp")
#include "AttachedCardShadow.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    void AttachedCardShadow::OnElementContextInitialized(AttachedShadowElementContext const& context)
    {
        UpdateVisualOpacityMask(context);
        base_type::OnElementContextInitialized(context);
    }

    winrt::CompositionBrush AttachedCardShadow::GetShadowMask(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
    {
        auto compositor = context.Compositor();
        if (compositor == nullptr)
        {
            return nullptr;
        }

        // Create rounded rectangle geometry and add it to a shape
        auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<winrt::CompositionRoundedRectangleGeometry>();
        if (!geometry) geometry = context.AddResource(
            RoundedRectangleGeometryResourceKey,
            compositor.CreateRoundedRectangleGeometry()).as<winrt::CompositionRoundedRectangleGeometry>();
        geometry.CornerRadius(float2(static_cast<float>(CornerRadius())));

        auto shape = context.GetResource(ShapeResourceKey).try_as<winrt::CompositionSpriteShape>();
        if (!shape) shape = context.AddResource(ShapeResourceKey, compositor.CreateSpriteShape(geometry)).as<winrt::CompositionSpriteShape>();
        shape.FillBrush(compositor.CreateColorBrush(winrt::Microsoft::UI::Colors::Black()));

        // Create a ShapeVisual so that our geometry can be rendered to a visual
        auto shapeVisual = context.GetResource(ShapeVisualResourceKey).try_as<winrt::ShapeVisual>();
        if (!shapeVisual) shapeVisual = context.AddResource(ShapeVisualResourceKey, compositor.CreateShapeVisual()).as<winrt::ShapeVisual>();
        shapeVisual.Shapes().Append(shape);

        // Create a CompositionVisualSurface, which renders our ShapeVisual to a texture
        auto visualSurface = context.GetResource(VisualSurfaceResourceKey).try_as<winrt::CompositionVisualSurface>();
        if (!visualSurface) visualSurface = context.AddResource(VisualSurfaceResourceKey, compositor.CreateVisualSurface()).as<winrt::CompositionVisualSurface>();
        visualSurface.SourceVisual(shapeVisual);

        // Create a CompositionSurfaceBrush to render our CompositionVisualSurface to a brush.
        // Now we have a rounded rectangle brush that can be used on as the mask for our shadow.
        auto surfaceBrush = context.GetResource(SurfaceBrushResourceKey).try_as<winrt::CompositionSurfaceBrush>();
        if (!surfaceBrush) surfaceBrush =
            context.AddResource(
                SurfaceBrushResourceKey,
                compositor.CreateSurfaceBrush(visualSurface)).as<winrt::CompositionSurfaceBrush>();
        shapeVisual.Size({ context.Element().RenderSize().Width,context.Element().RenderSize().Height });
        visualSurface.SourceSize(shapeVisual.Size());
        geometry.Size(shapeVisual.Size());

        return surfaceBrush;
    }

    winrt::CompositionClip AttachedCardShadow::GetShadowClip(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context) const
    {
        if (InnerContentClipMode() != InnerContentClipMode::CompositionGeometricClip || context.Compositor() == nullptr)
        {
            context.RemoveAndDisposeResource(PathGeometryResourceKey);
            context.RemoveAndDisposeResource(ClipResourceKey);
            return nullptr;
        }

        // The way this shadow works without the need to project on another element is because
        // we're clipping the inner part of the shadow which would be cast on the element
        // itself away. This method is creating an outline so that we are only showing the
        // parts of the shadow that are outside the element's context.
        // Note: This does cause an issue if the element does clip itself to its bounds, as then
        // the shadowed area is clipped as well.
        auto pathGeom = context.GetResource(PathGeometryResourceKey).try_as<winrt::CompositionPathGeometry>();
        if (pathGeom == nullptr) pathGeom = context.AddResource(PathGeometryResourceKey, context.Compositor().CreatePathGeometry()).as<winrt::CompositionPathGeometry>();
        auto clip = context.GetResource(ClipResourceKey).try_as<winrt::CompositionGeometricClip>();
        if (clip == nullptr) clip = context.AddResource(ClipResourceKey, context.Compositor().CreateGeometricClip(pathGeom)).as<winrt::CompositionGeometricClip>();

        // Create rounded rectangle geometry at a larger size that compensates for the size of the stroke,
        // as we want the inside edge of the stroke to match the edges of the element.
        // Additionally, the inside edge of the stroke will have a smaller radius than the radius we specified.
        // Using "(StrokeThickness / 2) + Radius" as our rectangle's radius will give us an inside stroke radius that matches the radius we want.
        auto canvasRectangle = winrt::CanvasGeometry::CreateRoundedRectangle(
            nullptr,
            -MaxBlurRadius / 2,
            -MaxBlurRadius / 2,
            static_cast<float>(context.Element().ActualWidth()) + MaxBlurRadius,
            static_cast<float>(context.Element().ActualHeight()) + MaxBlurRadius,
            (MaxBlurRadius / 2) + static_cast<float>(CornerRadius()),
            (MaxBlurRadius / 2) + static_cast<float>(CornerRadius()));

        auto canvasStroke = canvasRectangle.Stroke(MaxBlurRadius);

        pathGeom.Path(CompositionPath(canvasStroke));

        return clip;
    }

    void AttachedCardShadow::UpdateVisualOpacityMask(AttachedShadowElementContext const& context) const
    {
        if (InnerContentClipMode() != InnerContentClipMode::CompositionMaskBrush
            || context.Compositor() == nullptr)
        {
            context.RemoveAndDisposeResource(OpacityMaskShapeVisualResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskGeometryResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskSpriteShapeResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskShapeVisualSurfaceResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskShapeVisualSurfaceBrushResourceKey);

            return;
        }

        // Create ShapeVisual, and CompositionSpriteShape with geometry, these will provide the visuals for the opacity mask.
        winrt::ShapeVisual shapeVisual = context.GetResource(OpacityMaskShapeVisualResourceKey).try_as<winrt::ShapeVisual>();
        if (!shapeVisual) shapeVisual = context.AddResource(OpacityMaskShapeVisualResourceKey, context.Compositor().CreateShapeVisual()).as<ShapeVisual>();

        winrt::CompositionRoundedRectangleGeometry geometry = context.GetResource(OpacityMaskGeometryResourceKey).try_as<winrt::CompositionRoundedRectangleGeometry>();
        if (!geometry) geometry = context.AddResource(OpacityMaskGeometryResourceKey, context.Compositor().CreateRoundedRectangleGeometry()).as<CompositionRoundedRectangleGeometry>();
        winrt::CompositionSpriteShape shape = context.GetResource(OpacityMaskSpriteShapeResourceKey).try_as<winrt::CompositionSpriteShape>();
        if (!shape) shape = context.AddResource(OpacityMaskSpriteShapeResourceKey, context.Compositor().CreateSpriteShape(geometry)).as<CompositionSpriteShape>();

        // Set the attributes of the geometry, and add the CompositionSpriteShape to the ShapeVisual.
        // The geometry will have a thick outline and no fill, meaning that when used as a mask,
        // the shadow will only be rendered on the outer area covered by the outline, clipping out its inner portion.
        geometry.Offset(winrt::float2(MaxBlurRadius / 2));
        geometry.CornerRadius(winrt::float2((MaxBlurRadius / 2) + static_cast<float>(CornerRadius())));
        shape.StrokeThickness(MaxBlurRadius);
        if (!shape.StrokeBrush()) shape.StrokeBrush(context.Compositor().CreateColorBrush(winrt::Microsoft::UI::Colors::Black()));

        if (uint32_t index; !shapeVisual.Shapes().IndexOf(shape, index))
        {
            shapeVisual.Shapes().Append(shape);
        }

        // Create CompositionVisualSurface using the ShapeVisual as the source visual.
        winrt::CompositionVisualSurface visualSurface = context.GetResource(OpacityMaskShapeVisualSurfaceResourceKey).try_as<winrt::CompositionVisualSurface>();
        if (!visualSurface) visualSurface = context.AddResource(OpacityMaskShapeVisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
        visualSurface.SourceVisual(shapeVisual);

        geometry.Size(winrt::float2(static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight())) + winrt::float2(MaxBlurRadius));

        visualSurface.SourceSize(winrt::float2{ static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight()) } + winrt::float2(MaxBlurRadius * 2));
        shapeVisual.Size(visualSurface.SourceSize());

        // Create a CompositionSurfaceBrush using the CompositionVisualSurface as the source, this essentially converts the ShapeVisual into a brush.
        // This brush can then be used as a mask.
        winrt::CompositionSurfaceBrush opacityMask = context.GetResource(OpacityMaskShapeVisualSurfaceBrushResourceKey).try_as<winrt::CompositionSurfaceBrush>();
        if (!opacityMask) opacityMask = context.AddResource(OpacityMaskShapeVisualSurfaceBrushResourceKey, context.Compositor().CreateSurfaceBrush()).as<CompositionSurfaceBrush>();
        opacityMask.Surface(visualSurface);
    }

    void AttachedCardShadow::OnPropertyChanged(AttachedShadowElementContext const& context, winrt::DependencyProperty const& property, winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue)
    {
        if (property == CornerRadiusProperty)
        {
            UpdateShadowClip(context);
            UpdateVisualOpacityMask(context);

            if (auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<winrt::CompositionRoundedRectangleGeometry>())
            {
                geometry.CornerRadius(winrt::float2(static_cast<float>(winrt::unbox_value<double>(newValue))));
            }
        }
        else if (property == InnerContentClipModeProperty)
        {
            UpdateShadowClip(context);
            UpdateVisualOpacityMask(context);
            SetElementChildVisual(context);
        }
        else
        {
            base_type::OnPropertyChanged(context, property, oldValue, newValue);
        }
    }

    void AttachedCardShadow::SetElementChildVisual(AttachedShadowElementContext const& context)
    {
        if (auto opacityMask = context.GetResource(OpacityMaskShapeVisualSurfaceBrushResourceKey).try_as<winrt::CompositionSurfaceBrush>(); opacityMask && context.Compositor())
        {
            // If the resource for OpacityMaskShapeVisualSurfaceBrushResourceKey exists it means this.InnerContentClipMode == CompositionVisualSurface,
            // which means we need to take some steps to set up an opacity mask.

            // Create a CompositionVisualSurface, and use the SpriteVisual containing the shadow as the source.
            winrt::CompositionVisualSurface shadowVisualSurface = context.GetResource(OpacityMaskVisualSurfaceResourceKey).try_as<winrt::CompositionVisualSurface>();
            if (!shadowVisualSurface) shadowVisualSurface = context.AddResource(OpacityMaskVisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
            shadowVisualSurface.SourceVisual(context.SpriteVisual());
            if (auto spriteVisual = context.SpriteVisual())
            {
                spriteVisual.RelativeSizeAdjustment(winrt::float2::zero());
                spriteVisual.Size({ static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight()) });
            }

            // Adjust the offset and size of the CompositionVisualSurface to accommodate the thick outline of the shape created in UpdateVisualOpacityMask().
            shadowVisualSurface.SourceOffset(winrt::float2(-MaxBlurRadius));
            shadowVisualSurface.SourceSize(winrt::float2(static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight())) + winrt::float2(144.0f));

            // Create a CompositionSurfaceBrush from the CompositionVisualSurface. This allows us to render the shadow in a brush.
            winrt::CompositionSurfaceBrush shadowSurfaceBrush = context.GetResource(OpacityMaskSurfaceBrushResourceKey).try_as<winrt::CompositionSurfaceBrush>();
            if (!shadowSurfaceBrush) shadowSurfaceBrush = context.AddResource(OpacityMaskSurfaceBrushResourceKey, context.Compositor().CreateSurfaceBrush()).as<winrt::CompositionSurfaceBrush>();
            shadowSurfaceBrush.Surface(shadowVisualSurface);
            shadowSurfaceBrush.Stretch(winrt::CompositionStretch::None);

            // Create a CompositionMaskBrush, using the CompositionSurfaceBrush of the shadow as the source,
            // and the CompositionSurfaceBrush created in UpdateVisualOpacityMask() as the mask.
            // This creates a brush that renders the shadow with its inner portion clipped out.
            winrt::CompositionMaskBrush maskBrush = context.GetResource(OpacityMaskBrushResourceKey).try_as<winrt::CompositionMaskBrush>();
            if (!maskBrush) maskBrush = context.AddResource(OpacityMaskBrushResourceKey, context.Compositor().CreateMaskBrush()).as<CompositionMaskBrush>();
            maskBrush.Source(shadowSurfaceBrush);
            maskBrush.Mask(opacityMask);

            // Create a SpriteVisual and set its brush to the CompositionMaskBrush created in the previous step,
            // then set it as the child of the element in the context.
            winrt::SpriteVisual visual = context.GetResource(OpacityMaskVisualResourceKey).try_as<winrt::SpriteVisual>();
            if (!visual) visual = context.AddResource(OpacityMaskVisualResourceKey, context.Compositor().CreateSpriteVisual()).as<SpriteVisual>();
            visual.RelativeSizeAdjustment(winrt::float2::one());
            visual.Offset(winrt::float3(-MaxBlurRadius, -MaxBlurRadius, 0));
            visual.Size(winrt::float2(MaxBlurRadius * 2));
            visual.Brush(maskBrush);
            winrt::ElementCompositionPreview::SetElementChildVisual(context.Element(), visual);
        }
        else
        {
            base_type::SetElementChildVisual(context);

            // Reset context.SpriteVisual.Size and RelativeSizeAdjustment to default values
            // as they may be changed in the block above.
            if (context.SpriteVisual())
            {
                context.SpriteVisual().Size(winrt::float2::zero());
                context.SpriteVisual().RelativeSizeAdjustment(winrt::float2::one());
            }

            context.RemoveAndDisposeResource(OpacityMaskVisualSurfaceResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskSurfaceBrushResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskVisualResourceKey);
            context.RemoveAndDisposeResource(OpacityMaskBrushResourceKey);
        }
    }

    void AttachedCardShadow::OnSizeChanged(AttachedShadowElementContext const& context, winrt::Size newSize, winrt::Size previousSize)
    {
        winrt::float2 sizeAsVec2{ newSize.Width, newSize.Height };
        if (auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<winrt::CompositionRoundedRectangleGeometry>())
        {
            geometry.Size(sizeAsVec2);
        }

        if (auto visualSurface = context.GetResource(VisualSurfaceResourceKey).try_as<winrt::CompositionVisualSurface>())
        {
            visualSurface.SourceSize(sizeAsVec2);
        }

        if (auto shapeVisual = context.GetResource(ShapeVisualResourceKey).try_as<winrt::ShapeVisual>())
        {
            shapeVisual.Size(sizeAsVec2);
        }

        if (auto opacityMaskVisualSurface = context.GetResource(OpacityMaskVisualSurfaceResourceKey).try_as<winrt::CompositionVisualSurface>())
        {
            opacityMaskVisualSurface.SourceSize(sizeAsVec2 + winrt::float2(MaxBlurRadius * 2));
        }

        if (context.SpriteVisual() && InnerContentClipMode() == InnerContentClipMode::CompositionMaskBrush)
        {
            context.SpriteVisual().Size(sizeAsVec2);
        }

        UpdateShadowClip(context);
        UpdateVisualOpacityMask(context);

        base_type::OnSizeChanged(context, newSize, previousSize);
    }
}
