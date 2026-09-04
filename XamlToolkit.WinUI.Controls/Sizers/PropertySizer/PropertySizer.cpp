#include "pch.h"
#include "winrt_module_imports.h"
#include "PropertySizer.h"
#if __has_include("PropertySizer.g.cpp")
#include "PropertySizer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> PropertySizer::IsDragInvertedProperty =
		winrt::DependencyProperty::Register(
			L"IsDragInverted",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(false)));

	const wil::single_threaded_property<winrt::DependencyProperty> PropertySizer::BindingProperty =
		winrt::DependencyProperty::Register(
			L"Binding",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> PropertySizer::MinimumProperty =
		winrt::DependencyProperty::Register(
			L"Minimum",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(0.0)));

	const wil::single_threaded_property<winrt::DependencyProperty> PropertySizer::MaximumProperty =
		winrt::DependencyProperty::Register(
			L"Maximum",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(0.0)));

	PropertySizer::PropertySizer() : _currentSize(0.0)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void PropertySizer::OnDragStarting()
	{
		// We grab the current size of the bound value when we start a drag
		// and we manipulate from that set point.
		if (ReadLocalValue(BindingProperty) != winrt::DependencyProperty::UnsetValue())
		{
			_currentSize = Binding();
		}
	}

	bool PropertySizer::OnDragHorizontal(double horizontalChange)
	{
		// We use a central function for both horizontal/vertical as
		// a general property has no notion of direction when we
		// manipulate it, so the logic is abstracted.
		return ApplySizeChange(horizontalChange);
	}

	bool PropertySizer::OnDragVertical(double verticalChange)
	{
		return ApplySizeChange(verticalChange);
	}

	bool PropertySizer::ApplySizeChange(double newSize)
	{
		newSize = IsDragInverted() ? -newSize : newSize;

		// We want to be checking the modified final value for bounds checks.
		newSize += _currentSize;

		// Check if we hit the min/max value, as we should use that if we're on the edge
		if (ReadLocalValue(MinimumProperty) != winrt::DependencyProperty::UnsetValue() &&
			newSize < Minimum())
		{
			// We use SetValue here as that'll update our bound property vs. overwriting the binding itself.
			SetValue(BindingProperty(), winrt::box_value(Minimum()));
		}
		else if (ReadLocalValue(MaximumProperty) != winrt::DependencyProperty::UnsetValue() &&
			newSize > Maximum())
		{
			SetValue(BindingProperty(), winrt::box_value(Maximum()));
		}
		else
		{
			// Otherwise, we use the value provided.
			SetValue(BindingProperty(), winrt::box_value(newSize));
		}

		// We're always manipulating the value effectively.
		return true;
	}
}
