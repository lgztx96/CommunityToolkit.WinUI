#include "pch.h"
#include "AttachedDropShadow.h"
#if __has_include("AttachedDropShadow.g.cpp")
#include "AttachedDropShadow.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	static inline bool Contains(Microsoft::UI::Composition::VisualCollection const& collection, Visual const& visual)
	{
		return std::ranges::any_of(collection, [&](auto const& item) { return item == visual; });
	}

	void AttachedDropShadow::OnCastToPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto obj = d.try_as<class_type>())
		{
			auto shadow = winrt::get_self<AttachedDropShadow>(obj)->get_strong();
			if (auto element = e.OldValue().try_as<FrameworkElement>())
			{
				ElementCompositionPreview::SetElementChildVisual(element, nullptr);
				shadow->_castToSizeChangedRevoker.revoke();
			}

			if (auto elementNew = e.NewValue().try_as<FrameworkElement>())
			{
				auto prevContainer = shadow->_container;

				auto child = ElementCompositionPreview::GetElementChildVisual(elementNew);
				if (auto visual = child.try_as<ContainerVisual>())
				{
					shadow->_container = visual;
				}
				else
				{
					auto compositor = ElementCompositionPreview::GetElementVisual(shadow->CastTo()).Compositor();
					shadow->_container = compositor.CreateContainerVisual();

					ElementCompositionPreview::SetElementChildVisual(elementNew, shadow->_container);
				}

				// Need to remove all old children from previous container if it's changed
				if (prevContainer && prevContainer != shadow->_container)
				{
					for (const auto& context : shadow->EnumerateElementContexts())
					{
						if (context.IsInitialized()
							//&& prevContainer.Children().IndexOf(context.SpriteVisual())
							)
						{
							prevContainer.Children().Remove(context.SpriteVisual());
						}
					}
				}

				// Make sure all child shadows are hooked into container
				for (const auto& context : shadow->EnumerateElementContexts())
				{
					if (context.IsInitialized())
					{
						shadow->SetElementChildVisual(context);
					}
				}

				shadow->_castToSizeChangedRevoker = elementNew.SizeChanged(winrt::auto_revoke,
					[shadowWeak(winrt::make_weak(obj))](auto& s, auto& e)
					{
						if (auto shadow = shadowWeak.get())
						{
							auto shadowSelf = winrt::get_self<AttachedDropShadow>(shadow)->get_strong();
							shadowSelf->CastToElement_SizeChanged(s, e);
						}
					});

				// Re-trigger updates to all shadow locations for new parent
				shadow->CastToElement_SizeChanged(nullptr, nullptr);
			}
		}
	}

	void AttachedDropShadow::CastToElement_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		// Don't use sender or 'e' here as related to container element not
		// element for shadow, grab values off context. (Also may be null from internal call.)
		for (const auto& context : EnumerateElementContexts())
		{
			if (context.IsInitialized())
			{
				// TODO: Should we use ActualWidth/Height instead of RenderSize?
				auto renderSize = context.Element().RenderSize();
				OnSizeChanged(context, renderSize, renderSize);
			}
		}
	}

	void AttachedDropShadow::OnElementContextUninitialized(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		auto constSelf = winrt::get_self<AttachedShadowElementContext>(context)->get_strong();

		if (_container != nullptr
			//&& _container.Children().Contains(context.SpriteVisual())
			)
		{
			_container.Children().Remove(context.SpriteVisual());
		}

		if (auto spriteVisual = context.SpriteVisual()) spriteVisual.StopAnimation(L"Size");

		constSelf->_layoutUpdatedRevoker.revoke();

		if (constSelf->VisibilityToken().has_value())
		{
			context.Element().UnregisterPropertyChangedCallback(UIElement::VisibilityProperty(), constSelf->VisibilityToken.value());
			constSelf->VisibilityToken(std::nullopt);
		}

		base_type::OnElementContextUninitialized(context);
	}

	void AttachedDropShadow::SetElementChildVisual(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (_container && !Contains(_container.Children(), context.SpriteVisual()))
		{
			_container.Children().InsertAtTop(context.SpriteVisual());
		}

		auto constSelf = winrt::get_self<AttachedShadowElementContext>(context)->get_strong();
		// Handles size changing and other elements around it updating.
		constSelf->_layoutUpdatedRevoker = context.Element().LayoutUpdated(winrt::auto_revoke, { get_weak(), &AttachedDropShadow::Element_LayoutUpdated });

		if (auto visibilityToken = constSelf->VisibilityToken())
		{
			context.Element().UnregisterPropertyChangedCallback(UIElement::VisibilityProperty(), visibilityToken.value());
			constSelf->VisibilityToken(std::nullopt);
		}

		constSelf->VisibilityToken(context.Element().RegisterPropertyChangedCallback(UIElement::VisibilityProperty(), { get_weak(), &AttachedDropShadow::Element_VisibilityChanged }));
	}

	void AttachedDropShadow::Element_LayoutUpdated([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IInspectable const& e)
	{
		// Update other shadows to account for layout changes
		CastToElement_SizeChanged(nullptr, nullptr);
	}

	void AttachedDropShadow::Element_VisibilityChanged(DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		auto element = sender.try_as<FrameworkElement>();
		auto context = element ? GetElementContext(element).try_as<winrt::CommunityToolkit::WinUI::AttachedShadowElementContext>() : nullptr;
		if (element && context)
		{
			if (element.Visibility() == Visibility::Collapsed)
			{
				if (_container != nullptr
					//&& _container.Children().Contains(context.SpriteVisual())
					)
				{
					_container.Children().Remove(context.SpriteVisual());
				}
			}
			else
			{
				if (_container && !Contains(_container.Children(), context.SpriteVisual()))
				{
					_container.Children().InsertAtTop(context.SpriteVisual());
				}
			}
		}

		// Update other shadows to account for layout changes
		CastToElement_SizeChanged(nullptr, nullptr);
	}

	CompositionBrush AttachedDropShadow::GetShadowMask(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		CompositionBrush mask{ nullptr };

		if (context.Element())
		{
			if (IsMasked())
			{
				// We check for IAlphaMaskProvider first, to ensure that we use the custom
				// alpha mask even if Content happens to extend any of the other classes
				if (auto maskedControl = context.Element().try_as<IAlphaMaskProvider>())
				{
					if (maskedControl.WaitUntilLoaded() && !context.Element().IsLoaded())
					{
						auto elementLoadedToken = std::make_shared<winrt::event_token>();
						auto element = context.Element();
						*elementLoadedToken = element.Loaded([element, elementLoadedToken, weakThis{ get_weak() }](auto& s, auto& e)
						{
							if (auto shadow = weakThis.get()) {
								shadow->CustomMaskedElement_Loaded(s, e);
							}

							element.Loaded(*elementLoadedToken);
						});
					}
					else
					{
						mask = maskedControl.GetAlphaMask();
					}
				}
				else if (auto image = context.Element().try_as<Image>())
				{
					mask = image.GetAlphaMask();
				}
				else if (auto shape = context.Element().try_as<winrt::Microsoft::UI::Xaml::Shapes::Shape>())
				{
					mask = shape.GetAlphaMask();
				}
				else if (auto textBlock = context.Element().try_as<TextBlock>())
				{
					mask = textBlock.GetAlphaMask();
				}
			}

			// If we don't have a mask and have specified rounded corners, we'll generate a simple quick mask.
			// This is the same code from link:AttachedCardShadow.cs:GetShadowMask
			if (mask == nullptr
				&& CornerRadius() > 0
				&& context.Compositor())
			{
				// Create rounded rectangle geometry and add it to a shape
				auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>();
				if (!geometry) geometry = context.AddResource(RoundedRectangleGeometryResourceKey,
					context.Compositor().CreateRoundedRectangleGeometry()).as<CompositionRoundedRectangleGeometry>();

				geometry.CornerRadius(float2((float)CornerRadius()));

				auto shape = context.GetResource(ShapeResourceKey).try_as<CompositionSpriteShape>();
				if (!shape) shape = context.AddResource(ShapeResourceKey, context.Compositor().CreateSpriteShape(geometry)).as<CompositionSpriteShape>();
				shape.FillBrush(context.Compositor().CreateColorBrush(Microsoft::UI::Colors::Black()));

				// Create a ShapeVisual so that our geometry can be rendered to a visual
				auto shapeVisual = context.GetResource(ShapeVisualResourceKey).try_as<ShapeVisual>();
				if (!shapeVisual) shapeVisual = context.AddResource(ShapeVisualResourceKey, context.Compositor().CreateShapeVisual()).as<ShapeVisual>();
				if (uint32_t index; !shapeVisual.Shapes().IndexOf(shape, index))
				{
					shapeVisual.Shapes().Append(shape);
				}

				// Create a CompositionVisualSurface, which renders our ShapeVisual to a texture
				auto visualSurface = context.GetResource(VisualSurfaceResourceKey).try_as<CompositionVisualSurface>();
				if (!visualSurface) visualSurface = context.AddResource(VisualSurfaceResourceKey, context.Compositor().CreateVisualSurface()).as<CompositionVisualSurface>();
				visualSurface.SourceVisual(shapeVisual);

				// Create a CompositionSurfaceBrush to render our CompositionVisualSurface to a brush.
				// Now we have a rounded rectangle brush that can be used on as the mask for our shadow.
				auto surfaceBrush = context.GetResource(SurfaceBrushResourceKey).try_as<CompositionSurfaceBrush>();
				if (!surfaceBrush) surfaceBrush = context.AddResource(
					SurfaceBrushResourceKey,
					context.Compositor().CreateSurfaceBrush(visualSurface)).as<CompositionSurfaceBrush>();

				float2 size(context.Element().RenderSize());
				shapeVisual.Size(size);
				visualSurface.SourceSize(size);
				geometry.Size(size);

				mask = surfaceBrush;
			}

			// Position our shadow in the correct spot to match the corresponding element.
			if (auto spriteVisual = context.SpriteVisual())
			{
				auto point = UIElementExtensions::CoordinatesFrom(context.Element(), CastTo());
				auto offset = float3(float2(point), 0.0f);
				spriteVisual.Offset(offset);

				BindSizeAndScale(spriteVisual, context.Element());
			}
		}

		return mask;
	}

	void AttachedDropShadow::BindSizeAndScale(CompositionObject const& source, UIElement const& target)
	{
		auto visual = ElementCompositionPreview::GetElementVisual(target);
		auto bindSizeAnimation = source.Compositor().CreateExpressionAnimation(L"visual.Size * visual.Scale.XY");

		bindSizeAnimation.SetReferenceParameter(L"visual", visual);

		// Start the animation
		source.StartAnimation(L"Size", bindSizeAnimation);
	}

	void AttachedDropShadow::CustomMaskedElement_Loaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		auto element = sender.try_as<FrameworkElement>();
		if (auto context = GetElementContext(element).try_as<winrt::CommunityToolkit::WinUI::AttachedShadowElementContext>())
		{
			// context.Element().Loaded -= CustomMaskedElement_Loaded;

			UpdateShadowClip(context);
			UpdateShadowMask(context);
		}
	}

	void AttachedDropShadow::OnSizeChanged(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context, Size newSize, Size previousSize)
	{
		if (auto spriteVisual = context.SpriteVisual())
		{
			auto point = UIElementExtensions::CoordinatesFrom(context.Element(), CastTo());
			auto offset = float3(float2(point), 0.0f);
			spriteVisual.Offset(offset);
		}

		UpdateShadowClip(context);
		UpdateShadowMask(context);

		base_type::OnSizeChanged(context, newSize, previousSize);
	}

	void AttachedDropShadow::OnPropertyChanged(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context, DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue)
	{
		if (property == IsMaskedProperty())
		{
			UpdateShadowMask(context);
		}
		else if (property == CornerRadiusProperty())
		{
			if (auto geometry = context.GetResource(RoundedRectangleGeometryResourceKey).try_as<CompositionRoundedRectangleGeometry>())
			{
				geometry.CornerRadius(float2((float)winrt::unbox_value<double>(newValue)));
			}

			UpdateShadowMask(context);
		}
		else
		{
			base_type::OnPropertyChanged(context, property, oldValue, newValue);
		}
	}
}
