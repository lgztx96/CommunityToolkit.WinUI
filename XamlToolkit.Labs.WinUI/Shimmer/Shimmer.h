#pragma once

#include "Shimmer.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <chrono>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Microsoft::UI::Xaml::Shapes;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct Shimmer : ShimmerT<Shimmer>
	{
		static constexpr float InitialStartPointX = -7.92f;
		static constexpr std::wstring_view PART_Shape = L"Shape";

		Shimmer();

		void OnApplyTemplate();

		static void PropertyChanged(winrt::DependencyObject const& s, winrt::DependencyPropertyChangedEventArgs e);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> DurationProperty =
			winrt::DependencyProperty::Register(
				L"Duration",
				winrt::xaml_typename<winrt::TimeSpan>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata
				{
					winrt::box_value(winrt::TimeSpan{ std::chrono::milliseconds(1600) }),
					&Shimmer::PropertyChanged
				}
			);

		winrt::TimeSpan Duration() const
		{
			return winrt::unbox_value<winrt::TimeSpan>(GetValue(DurationProperty()));
		}

		void Duration(winrt::TimeSpan const& value)
		{
			SetValue(DurationProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> IsActiveProperty =
			winrt::DependencyProperty::Register(
				L"IsActive",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(true), &Shimmer::PropertyChanged });

		bool IsActive() const
		{
			return winrt::unbox_value<bool>(GetValue(IsActiveProperty()));
		}

		void IsActive(bool value)
		{
			SetValue(IsActiveProperty(), winrt::box_value(value));
		}

	private:
		void OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnActualThemeChanged(winrt::FrameworkElement const& sender, winrt::IInspectable const& args);

		bool TryInitializationResource();

		void SetGradientAndStops();

		void SetGradientStopColorsByTheme();

		void TryStartAnimation();

		void StopAnimation();

	private:
		winrt::ExpressionAnimation _sizeAnimation{ nullptr };
		winrt::Vector2KeyFrameAnimation _gradientStartPointAnimation{ nullptr };
		winrt::Vector2KeyFrameAnimation _gradientEndPointAnimation{ nullptr };
		winrt::CompositionColorGradientStop _gradientStop1{ nullptr };
		winrt::CompositionColorGradientStop _gradientStop2{ nullptr };
		winrt::CompositionColorGradientStop _gradientStop3{ nullptr };
		winrt::CompositionColorGradientStop _gradientStop4{ nullptr };
		winrt::CompositionRoundedRectangleGeometry _rectangleGeometry{ nullptr };
		winrt::ShapeVisual _shapeVisual{ nullptr };
		winrt::CompositionLinearGradientBrush _shimmerMaskGradient{ nullptr };
		winrt::Border _shape{ nullptr };

		bool _initialized;
		bool _animationStarted;

		winrt::FrameworkElement::ActualThemeChanged_revoker _actualThemeChangedRevoker;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct Shimmer : ShimmerT<Shimmer, implementation::Shimmer>
	{
	};
}
