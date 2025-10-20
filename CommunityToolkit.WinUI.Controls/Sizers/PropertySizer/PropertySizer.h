#pragma once

#include "PropertySizer.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "../SizerBase.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Xaml::Input;

	struct PropertySizer : PropertySizerT<PropertySizer, implementation::SizerBase>
	{
		double _currentSize;

		PropertySizer() : _currentSize(0.0) {}

		bool IsDragInverted() { return winrt::unbox_value<double>(GetValue(IsDragInvertedProperty)); }
		void IsDragInverted(bool value) { SetValue(IsDragInvertedProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> IsDragInvertedProperty =
			DependencyProperty::Register(L"IsDragInverted", winrt::xaml_typename<bool>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(false)));

		double Binding() { return winrt::unbox_value<double>(GetValue(BindingProperty)); }
		void Binding(double value) { SetValue(BindingProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> BindingProperty =
			DependencyProperty::Register(L"Binding", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		double Minimum() { return winrt::unbox_value<double>(GetValue(MinimumProperty)); }
		void Minimum(double value) { SetValue(MinimumProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> MinimumProperty =
			DependencyProperty::Register(L"Minimum", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0)));

		double Maximum() { return winrt::unbox_value<double>(GetValue(MaximumProperty)); }
		void Maximum(double value) { SetValue(MaximumProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> MaximumProperty =
			DependencyProperty::Register(L"Maximum", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0)));

		void OnDragStarting();

		bool OnDragHorizontal(double horizontalChange);

		bool OnDragVertical(double verticalChange);

		bool ApplySizeChange(double newSize);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct PropertySizer : PropertySizerT<PropertySizer, implementation::PropertySizer>
	{
	};
}
