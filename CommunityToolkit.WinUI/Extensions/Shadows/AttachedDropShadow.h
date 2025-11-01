#pragma once

#include "AttachedDropShadow.g.h"
#include "common.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "AttachedShadowBase.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;

	struct AttachedDropShadow : AttachedDropShadowT<AttachedDropShadow, implementation::AttachedShadowBase>
	{
	private:
		static constexpr float MaxBlurRadius = 72;

		ContainerVisual _container{ nullptr };

		static constexpr TypedResourceKey<CompositionRoundedRectangleGeometry> RoundedRectangleGeometryResourceKey{ L"RoundedGeometry" };
		static constexpr TypedResourceKey<CompositionSpriteShape> ShapeResourceKey{ L"Shape" };
		static constexpr TypedResourceKey<ShapeVisual> ShapeVisualResourceKey{ L"ShapeVisual" };
		static constexpr TypedResourceKey<CompositionSurfaceBrush> SurfaceBrushResourceKey{ L"SurfaceBrush" };
		static constexpr TypedResourceKey<CompositionVisualSurface> VisualSurfaceResourceKey{ L"VisualSurface" };

		FrameworkElement::SizeChanged_revoker _castToSizeChangedRevoker;

		static void OnCastToPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:
		bool SupportsOnSizeChangedEvent() const noexcept { return true; }

		void OnElementContextUninitialized(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void SetElementChildVisual(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		CompositionBrush GetShadowMask(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

	    void OnSizeChanged(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context, Size newSize, Size previousSize);

		void OnPropertyChanged(winrt::CommunityToolkit::WinUI::AttachedShadowElementContext const& context, DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsMaskedProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsMasked",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(true), &AttachedDropShadow::OnDependencyPropertyChanged });

		bool IsMasked() const
		{
			return winrt::unbox_value<bool>(GetValue(IsMaskedProperty));
		}

		void IsMasked(bool value)
		{
			SetValue(IsMaskedProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CornerRadiusProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CornerRadius",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(4.0), &AttachedDropShadow::OnDependencyPropertyChanged });

		double CornerRadius() const
		{
			return winrt::unbox_value<double>(GetValue(CornerRadiusProperty));
		}

		void CornerRadius(double value)
		{
			SetValue(CornerRadiusProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CastToProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CastTo",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::FrameworkElement>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr, &AttachedDropShadow::OnCastToPropertyChanged });

		winrt::Microsoft::UI::Xaml::FrameworkElement CastTo() const
		{
			return GetValue(CastToProperty).try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
		}

		void CastTo(winrt::Microsoft::UI::Xaml::FrameworkElement const& value)
		{
			SetValue(CastToProperty, value);
		}

	private:
		void CastToElement_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void Element_LayoutUpdated(IInspectable const& sender, IInspectable const& e);

		void Element_VisibilityChanged(DependencyObject const& sender, DependencyProperty const& dp);

		static void BindSizeAndScale(CompositionObject const& source, UIElement const& target);

		void CustomMaskedElement_Loaded(IInspectable const& sender, RoutedEventArgs const& e);
	};
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct AttachedDropShadow : AttachedDropShadowT<AttachedDropShadow, implementation::AttachedDropShadow>
	{
	};
}
