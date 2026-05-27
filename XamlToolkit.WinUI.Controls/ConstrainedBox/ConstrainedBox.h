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
	private:
		static constexpr double CalculationTolerance = 1.5;

		Size _originalSize;
		Size _lastMeasuredSize;

		bool _propertyUpdating;

		bool IsPositiveRealNumber(double value) { return !std::isnan(value) && !std::isinf(value) && value > 0; }

		static void ConstraintPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:

		ConstrainedBox() : _propertyUpdating(false) {}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleXProperty =
			winrt::DependencyProperty::Register(
				L"ScaleX",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(1.0), &ConstrainedBox::ConstraintPropertyChanged });

		double ScaleX()
		{
			return GetValue(ScaleXProperty()).as<double>();
		}

		void ScaleX(double value)
		{
			SetValue(ScaleXProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleYProperty =
			winrt::DependencyProperty::Register(
				L"ScaleY",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(1.0), &ConstrainedBox::ConstraintPropertyChanged });

		double ScaleY()
		{
			return GetValue(ScaleYProperty).as<double>();
		}

		void ScaleY(double value)
		{
			SetValue(ScaleYProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> MultipleXProperty =
			winrt::DependencyProperty::Register(
				L"MultipleX",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &ConstrainedBox::ConstraintPropertyChanged });

		int MultipleX()
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleXProperty), 0);
		}

		void MultipleX(int value)
		{
			SetValue(MultipleXProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> MultipleYProperty =
			winrt::DependencyProperty::Register(
				L"MultipleY",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, PropertyChangedCallback{ &ConstrainedBox::ConstraintPropertyChanged } });

		int MultipleY()
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleYProperty), 0);
		}

		void MultipleY(int value)
		{
			SetValue(MultipleYProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AspectRatioProperty =
			winrt::DependencyProperty::Register(
				L"AspectRatio",
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::AspectRatio>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &ConstrainedBox::ConstraintPropertyChanged });

		winrt::XamlToolkit::WinUI::Controls::AspectRatio AspectRatio()
		{
			return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::AspectRatio>(GetValue(AspectRatioProperty));
		}

		void AspectRatio(winrt::XamlToolkit::WinUI::Controls::AspectRatio const& value)
		{
			SetValue(AspectRatioProperty, winrt::box_value(value));
		}

		Size MeasureOverride(Size availableSize);

		Size ArrangeOverride(Size finalSize);

	private:
		void CalculateConstrainedSize(Size& availableSize);

		void CoerceValues();
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ConstrainedBox : ConstrainedBoxT<ConstrainedBox, implementation::ConstrainedBox>
	{
	};
}
