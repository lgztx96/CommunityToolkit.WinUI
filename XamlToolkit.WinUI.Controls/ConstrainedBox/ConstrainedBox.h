#pragma once

#include "ConstrainedBox.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <cmath>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ConstrainedBox : ConstrainedBoxT<ConstrainedBox>
	{
		ConstrainedBox() : _propertyUpdating(false) {}

		static const wil::single_threaded_property<winrt::DependencyProperty> ScaleXProperty;

		double ScaleX() const
		{
			return winrt::unbox_value<double>(GetValue(ScaleXProperty()));
		}

		void ScaleX(double value)
		{
			SetValue(ScaleXProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> ScaleYProperty;

		double ScaleY() const
		{
			return winrt::unbox_value<double>(GetValue(ScaleYProperty()));
		}

		void ScaleY(double value)
		{
			SetValue(ScaleYProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> MultipleXProperty;

		int MultipleX() const
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleXProperty()), 0);
		}

		void MultipleX(int value)
		{
			SetValue(MultipleXProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> MultipleYProperty;

		int MultipleY() const
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleYProperty()), 0);
		}

		void MultipleY(int value)
		{
			SetValue(MultipleYProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> AspectRatioProperty;

		winrt::XamlToolkit::WinUI::Controls::AspectRatio AspectRatio() const
		{
			return GetValue(AspectRatioProperty).try_as<winrt::XamlToolkit::WinUI::Controls::AspectRatio>();
		}

		void AspectRatio(winrt::XamlToolkit::WinUI::Controls::AspectRatio const& value)
		{
			SetValue(AspectRatioProperty, winrt::box_value(value));
		}

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::Size finalSize);

	private:
		void CalculateConstrainedSize(winrt::Size& availableSize);

		void CoerceValues();

		bool IsPositiveRealNumber(double value) { return !std::isnan(value) && !std::isinf(value) && value > 0; }

		static void ConstraintPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static constexpr double CalculationTolerance = 1.5;

		winrt::Size _originalSize;
		winrt::Size _lastMeasuredSize;

		bool _propertyUpdating;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ConstrainedBox : ConstrainedBoxT<ConstrainedBox, implementation::ConstrainedBox>
	{
	};
}
