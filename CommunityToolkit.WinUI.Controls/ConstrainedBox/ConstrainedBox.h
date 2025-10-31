#pragma once

#include "ConstrainedBox.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleXProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"ScaleX",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(1.0), &ConstrainedBox::ConstraintPropertyChanged });

		double ScaleX()
		{
			return GetValue(ScaleXProperty).as<double>();
		}

		void ScaleX(double value)
		{
			SetValue(ScaleXProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleYProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"ScaleY",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(1.0), &ConstrainedBox::ConstraintPropertyChanged });

		double ScaleY()
		{
			return GetValue(ScaleYProperty).as<double>();
		}

		void ScaleY(double value)
		{
			SetValue(ScaleYProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MultipleXProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"MultipleX",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &ConstrainedBox::ConstraintPropertyChanged });

		int MultipleX()
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleXProperty), 0);
		}

		void MultipleX(int value)
		{
			SetValue(MultipleXProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MultipleYProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"MultipleY",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr, PropertyChangedCallback{ &ConstrainedBox::ConstraintPropertyChanged } });

		int MultipleY()
		{
			return winrt::unbox_value_or<int>(GetValue(MultipleYProperty), 0);
		}

		void MultipleY(int value)
		{
			SetValue(MultipleYProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AspectRatioProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"AspectRatio",
				winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::AspectRatio>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &ConstrainedBox::ConstraintPropertyChanged });

		winrt::CommunityToolkit::WinUI::Controls::AspectRatio AspectRatio()
		{
			return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::AspectRatio>(GetValue(AspectRatioProperty));
		}

		void AspectRatio(winrt::CommunityToolkit::WinUI::Controls::AspectRatio const& value)
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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct ConstrainedBox : ConstrainedBoxT<ConstrainedBox, implementation::ConstrainedBox>
	{
	};
}
