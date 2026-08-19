#pragma once

#include "PropertySizer.g.h"
#include "../SizerBase.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct PropertySizer : PropertySizerT<PropertySizer, implementation::SizerBase>
	{
		PropertySizer();

		bool IsDragInverted() const { return winrt::unbox_value<bool>(GetValue(IsDragInvertedProperty())); }
		void IsDragInverted(bool value) { SetValue(IsDragInvertedProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> IsDragInvertedProperty;

		double Binding() const { return winrt::unbox_value<double>(GetValue(BindingProperty())); }
		void Binding(double value) { SetValue(BindingProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> BindingProperty;

		double Minimum() const { return winrt::unbox_value<double>(GetValue(MinimumProperty())); }
		void Minimum(double value) { SetValue(MinimumProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> MinimumProperty;

		double Maximum() const { return winrt::unbox_value<double>(GetValue(MaximumProperty())); }
		void Maximum(double value) { SetValue(MaximumProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> MaximumProperty;

		void OnDragStarting();

		bool OnDragHorizontal(double horizontalChange);

		bool OnDragVertical(double verticalChange);

		bool ApplySizeChange(double newSize);

	private:
		double _currentSize;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct PropertySizer : PropertySizerT<PropertySizer, implementation::PropertySizer>
	{
	};
}
