#pragma once

#include "AttachedDropShadow.g.h"
#include "../Shadows/TypedResourceKey.h"
#include "AttachedShadowBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Shapes;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct AttachedDropShadow : AttachedDropShadowT<AttachedDropShadow, implementation::AttachedShadowBase>
	{
	private:
		static constexpr float MaxBlurRadius = 72;

		winrt::ContainerVisual _container{ nullptr };

		static constexpr TypedResourceKey<CompositionRoundedRectangleGeometry> RoundedRectangleGeometryResourceKey{ L"RoundedGeometry" };
		static constexpr TypedResourceKey<CompositionSpriteShape> ShapeResourceKey{ L"Shape" };
		static constexpr TypedResourceKey<ShapeVisual> ShapeVisualResourceKey{ L"ShapeVisual" };
		static constexpr TypedResourceKey<CompositionSurfaceBrush> SurfaceBrushResourceKey{ L"SurfaceBrush" };
		static constexpr TypedResourceKey<CompositionVisualSurface> VisualSurfaceResourceKey{ L"VisualSurface" };

		winrt::FrameworkElement::SizeChanged_revoker _castToSizeChangedRevoker;

		static void OnCastToPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

	public:
		bool SupportsOnSizeChangedEvent() const noexcept { return true; }

		void OnElementContextUninitialized(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void SetElementChildVisual(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		winrt::CompositionBrush GetShadowMask(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

	    void OnSizeChanged(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context, winrt::Size newSize, winrt::Size previousSize);

		void OnPropertyChanged(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context, winrt::DependencyProperty const& property, winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> IsMaskedProperty =
			winrt::DependencyProperty::Register(
				L"IsMasked",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(true), &AttachedDropShadow::OnDependencyPropertyChanged });

		bool IsMasked() const
		{
			return winrt::unbox_value<bool>(GetValue(IsMaskedProperty()));
		}

		void IsMasked(bool value)
		{
			SetValue(IsMaskedProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CornerRadiusProperty =
			winrt::DependencyProperty::Register(
				L"CornerRadius",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(4.0), &AttachedDropShadow::OnDependencyPropertyChanged });

		double CornerRadius() const
		{
			return winrt::unbox_value<double>(GetValue(CornerRadiusProperty()));
		}

		void CornerRadius(double value)
		{
			SetValue(CornerRadiusProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CastToProperty =
			winrt::DependencyProperty::Register(
				L"CastTo",
				winrt::xaml_typename<winrt::FrameworkElement>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &AttachedDropShadow::OnCastToPropertyChanged });

		winrt::FrameworkElement CastTo() const
		{
			return GetValue(CastToProperty()).try_as<winrt::FrameworkElement>();
		}

		void CastTo(winrt::FrameworkElement const& value)
		{
			SetValue(CastToProperty(), value);
		}

	private:
		void CastToElement_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		void Element_LayoutUpdated(winrt::IInspectable const& sender, winrt::IInspectable const& e);

		void Element_VisibilityChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		static void BindSizeAndScale(winrt::CompositionObject const& source, winrt::UIElement const& target);

		void CustomMaskedElement_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct AttachedDropShadow : AttachedDropShadowT<AttachedDropShadow, implementation::AttachedDropShadow>
	{
	};
}
