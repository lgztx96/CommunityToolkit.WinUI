#include "pch.h"
#include "AttachedCardShadow.h"
#if __has_include("AttachedCardShadow.g.cpp")
#include "AttachedCardShadow.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Media::implementation
{
	void AttachedCardShadow::OnElementContextInitialized(AttachedShadowElementContext const& context)
	{
		UpdateVisualOpacityMask(context);
		base_type::OnElementContextInitialized(context);
	}

	CompositionBrush AttachedCardShadow::GetShadowMask(CommunityToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (context.Compositor() == nullptr)
		{
			return nullptr;
		}

		// Create rounded rectangle geometry and add it to a shape
		auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>();
		if (!geometry) geometry = context.AddResource(
			RoundedRectangleGeometryResourceKey,
			context.Compositor().CreateRoundedRectangleGeometry()).as<CompositionRoundedRectangleGeometry>();
		geometry.CornerRadius(float2(static_cast<float>(CornerRadius())));

		auto shape = context.GetResource(ShapeResourceKey).try_as<CompositionSpriteShape>();
		if (!shape) shape = context.AddResource(ShapeResourceKey, context.Compositor().CreateSpriteShape(geometry)).as<CompositionSpriteShape>();
		shape.FillBrush(context.Compositor().CreateColorBrush(Microsoft::UI::Colors::Black()));

		// Create a ShapeVisual so that our geometry can be rendered to a visual
		auto shapeVisual = context.GetResource(ShapeVisualResourceKey).try_as<ShapeVisual>();
		if (!shapeVisual) shapeVisual = context.AddResource(ShapeVisualResourceKey, context.Compositor().CreateShapeVisual()).as<ShapeVisual>();
		shapeVisual.Shapes().Append(shape);

		// Create a CompositionVisualSurface, which renders our ShapeVisual to a texture
		auto visualSurface = context.GetResource(VisualSurfaceResourceKey).try_as<CompositionVisualSurface>();
		if (!visualSurface) visualSurface = context.AddResource(VisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
		visualSurface.SourceVisual(shapeVisual);

		// Create a CompositionSurfaceBrush to render our CompositionVisualSurface to a brush.
		// Now we have a rounded rectangle brush that can be used on as the mask for our shadow.
		auto surfaceBrush = context.GetResource(SurfaceBrushResourceKey).try_as<CompositionSurfaceBrush>();
		if (!surfaceBrush) surfaceBrush =
			context.AddResource(
				SurfaceBrushResourceKey,
				context.Compositor().CreateSurfaceBrush(visualSurface)).as<CompositionSurfaceBrush>();
		shapeVisual.Size({ context.Element().RenderSize().Width,context.Element().RenderSize().Height });
		visualSurface.SourceSize(shapeVisual.Size());
		geometry.Size(shapeVisual.Size());

		return surfaceBrush;
	}

	CompositionClip AttachedCardShadow::GetShadowClip(CommunityToolkit::WinUI::AttachedShadowElementContext const& context) const
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
		auto pathGeom = context.GetResource(PathGeometryResourceKey).try_as<CompositionPathGeometry>();
		if (pathGeom == nullptr) pathGeom = context.AddResource(PathGeometryResourceKey, context.Compositor().CreatePathGeometry()).as<CompositionPathGeometry>();
		auto clip = context.GetResource(ClipResourceKey).try_as<CompositionGeometricClip>();
		if (clip == nullptr) clip = context.AddResource(ClipResourceKey, context.Compositor().CreateGeometricClip(pathGeom)).as<CompositionGeometricClip>();

		// Create rounded rectangle geometry at a larger size that compensates for the size of the stroke,
		// as we want the inside edge of the stroke to match the edges of the element.
		// Additionally, the inside edge of the stroke will have a smaller radius than the radius we specified.
		// Using "(StrokeThickness / 2) + Radius" as our rectangle's radius will give us an inside stroke radius that matches the radius we want.
		auto canvasRectangle = CanvasGeometry::CreateRoundedRectangle(
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
		ShapeVisual shapeVisual = context.GetResource(OpacityMaskShapeVisualResourceKey).try_as<ShapeVisual>();
		if (!shapeVisual) shapeVisual = context.AddResource(OpacityMaskShapeVisualResourceKey, context.Compositor().CreateShapeVisual()).as<ShapeVisual>();

		CompositionRoundedRectangleGeometry geometry = context.GetResource(OpacityMaskGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>();
		if (!geometry) geometry = context.AddResource(OpacityMaskGeometryResourceKey, context.Compositor().CreateRoundedRectangleGeometry()).as<CompositionRoundedRectangleGeometry>();
		CompositionSpriteShape shape = context.GetResource(OpacityMaskSpriteShapeResourceKey).try_as<CompositionSpriteShape>();
		if (!shape) shape = context.AddResource(OpacityMaskSpriteShapeResourceKey, context.Compositor().CreateSpriteShape(geometry)).as<CompositionSpriteShape>();

		// Set the attributes of the geometry, and add the CompositionSpriteShape to the ShapeVisual.
		// The geometry will have a thick outline and no fill, meaning that when used as a mask,
		// the shadow will only be rendered on the outer area covered by the outline, clipping out its inner portion.
		geometry.Offset(float2(MaxBlurRadius / 2));
		geometry.CornerRadius(float2((MaxBlurRadius / 2) + static_cast<float>(CornerRadius())));
		shape.StrokeThickness(MaxBlurRadius);
		if (!shape.StrokeBrush()) shape.StrokeBrush(context.Compositor().CreateColorBrush(Microsoft::UI::Colors::Black()));

		if (uint32_t index; !shapeVisual.Shapes().IndexOf(shape, index))
		{
			shapeVisual.Shapes().Append(shape);
		}

		// Create CompositionVisualSurface using the ShapeVisual as the source visual.
		CompositionVisualSurface visualSurface = context.GetResource(OpacityMaskShapeVisualSurfaceResourceKey).try_as<CompositionVisualSurface>();
		if (!visualSurface) visualSurface = context.AddResource(OpacityMaskShapeVisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
		visualSurface.SourceVisual(shapeVisual);

		geometry.Size(float2(static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight())) + float2(MaxBlurRadius));

		visualSurface.SourceSize(float2{ static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight()) } + float2(MaxBlurRadius * 2));
		shapeVisual.Size(visualSurface.SourceSize());

		// Create a CompositionSurfaceBrush using the CompositionVisualSurface as the source, this essentially converts the ShapeVisual into a brush.
		// This brush can then be used as a mask.
		CompositionSurfaceBrush opacityMask = context.GetResource(OpacityMaskShapeVisualSurfaceBrushResourceKey).try_as<CompositionSurfaceBrush>();
		if (!opacityMask) opacityMask = context.AddResource(OpacityMaskShapeVisualSurfaceBrushResourceKey, context.Compositor().CreateSurfaceBrush()).as<CompositionSurfaceBrush>();
		opacityMask.Surface(visualSurface);
	}

	void AttachedCardShadow::OnPropertyChanged(AttachedShadowElementContext const& context, DependencyProperty const& property, winrt::Windows::Foundation::IInspectable const& oldValue, winrt::Windows::Foundation::IInspectable const& newValue)
	{
		if (property == CornerRadiusProperty)
		{
			UpdateShadowClip(context);
			UpdateVisualOpacityMask(context);

			if (auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>())
			{
				geometry.CornerRadius(winrt::Windows::Foundation::Numerics::float2(static_cast<float>(winrt::unbox_value<double>(newValue))));
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
		if (auto opacityMask = context.GetResource(OpacityMaskShapeVisualSurfaceBrushResourceKey).try_as<CompositionSurfaceBrush>(); opacityMask && context.Compositor())
		{
			// If the resource for OpacityMaskShapeVisualSurfaceBrushResourceKey exists it means this.InnerContentClipMode == CompositionVisualSurface,
			// which means we need to take some steps to set up an opacity mask.

			// Create a CompositionVisualSurface, and use the SpriteVisual containing the shadow as the source.
			CompositionVisualSurface shadowVisualSurface = context.GetResource(OpacityMaskVisualSurfaceResourceKey).try_as<CompositionVisualSurface>();
			if (!shadowVisualSurface) shadowVisualSurface = context.AddResource(OpacityMaskVisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
			shadowVisualSurface.SourceVisual(context.SpriteVisual());
			if (context.SpriteVisual())
			{
				context.SpriteVisual().RelativeSizeAdjustment(float2::zero());
				context.SpriteVisual().Size({ static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight()) });
			}

			// Adjust the offset and size of the CompositionVisualSurface to accommodate the thick outline of the shape created in UpdateVisualOpacityMask().
			shadowVisualSurface.SourceOffset(float2(-MaxBlurRadius));
			shadowVisualSurface.SourceSize(float2(static_cast<float>(context.Element().ActualWidth()), static_cast<float>(context.Element().ActualHeight())) + float2(144.0f));

			// Create a CompositionSurfaceBrush from the CompositionVisualSurface. This allows us to render the shadow in a brush.
			CompositionSurfaceBrush shadowSurfaceBrush = context.GetResource(OpacityMaskSurfaceBrushResourceKey).try_as<CompositionSurfaceBrush>();
			if (!shadowSurfaceBrush) shadowSurfaceBrush = context.AddResource(OpacityMaskSurfaceBrushResourceKey, context.Compositor().CreateSurfaceBrush()).as<CompositionSurfaceBrush>();
			shadowSurfaceBrush.Surface(shadowVisualSurface);
			shadowSurfaceBrush.Stretch(CompositionStretch::None);

			// Create a CompositionMaskBrush, using the CompositionSurfaceBrush of the shadow as the source,
			// and the CompositionSurfaceBrush created in UpdateVisualOpacityMask() as the mask.
			// This creates a brush that renders the shadow with its inner portion clipped out.
			CompositionMaskBrush maskBrush = context.GetResource(OpacityMaskBrushResourceKey).try_as<CompositionMaskBrush>();
			if (!maskBrush) maskBrush = context.AddResource(OpacityMaskBrushResourceKey, context.Compositor().CreateMaskBrush()).as<CompositionMaskBrush>();
			maskBrush.Source(shadowSurfaceBrush);
			maskBrush.Mask(opacityMask);

			// Create a SpriteVisual and set its brush to the CompositionMaskBrush created in the previous step,
			// then set it as the child of the element in the context.
			SpriteVisual visual = context.GetResource(OpacityMaskVisualResourceKey).try_as<SpriteVisual>();
			if (!visual) visual = context.AddResource(OpacityMaskVisualResourceKey, context.Compositor().CreateSpriteVisual()).as<SpriteVisual>();
			visual.RelativeSizeAdjustment(float2::one());
			visual.Offset(float3(-MaxBlurRadius, -MaxBlurRadius, 0));
			visual.Size(float2(MaxBlurRadius * 2));
			visual.Brush(maskBrush);
			ElementCompositionPreview::SetElementChildVisual(context.Element(), visual);
		}
		else
		{
			base_type::SetElementChildVisual(context);

			// Reset context.SpriteVisual.Size and RelativeSizeAdjustment to default values
			// as they may be changed in the block above.
			if (context.SpriteVisual())
			{
				context.SpriteVisual().Size(float2::zero());
				context.SpriteVisual().RelativeSizeAdjustment(float2::one());
			}

			context.RemoveAndDisposeResource(OpacityMaskVisualSurfaceResourceKey);
			context.RemoveAndDisposeResource(OpacityMaskSurfaceBrushResourceKey);
			context.RemoveAndDisposeResource(OpacityMaskVisualResourceKey);
			context.RemoveAndDisposeResource(OpacityMaskBrushResourceKey);
		}
	}

	void AttachedCardShadow::OnSizeChanged(AttachedShadowElementContext const& context, Windows::Foundation::Size newSize, Windows::Foundation::Size previousSize)
	{
		float2 sizeAsVec2{ newSize.Width, newSize.Height };
		if (auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>())
		{
			geometry.Size(sizeAsVec2);
		}

		if (auto visualSurface = context.GetResource(VisualSurfaceResourceKey).try_as<CompositionVisualSurface>())
		{
			visualSurface.SourceSize(sizeAsVec2);
		}

		if (auto shapeVisual = context.GetResource(ShapeVisualResourceKey).try_as<ShapeVisual>())
		{
			shapeVisual.Size(sizeAsVec2);
		}

		if (auto opacityMaskVisualSurface = context.GetResource(OpacityMaskVisualSurfaceResourceKey).try_as<CompositionVisualSurface>())
		{
			opacityMaskVisualSurface.SourceSize(sizeAsVec2 + float2(MaxBlurRadius * 2));
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
