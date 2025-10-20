#include "pch.h"
#include "PropertySizer.h"
#if __has_include("PropertySizer.g.cpp")
#include "PropertySizer.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void PropertySizer::OnDragStarting()
	{
		// We grab the current size of the bound value when we start a drag
		// and we manipulate from that set point.
		if (ReadLocalValue(BindingProperty) != DependencyProperty::UnsetValue())
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
		if (ReadLocalValue(MinimumProperty) != DependencyProperty::UnsetValue() &&
			newSize < Minimum())
		{
			// We use SetValue here as that'll update our bound property vs. overwriting the binding itself.
			SetValue(BindingProperty, winrt::box_value(Minimum()));
		}
		else if (ReadLocalValue(MaximumProperty) != DependencyProperty::UnsetValue() &&
			newSize > Maximum())
		{
			SetValue(BindingProperty, winrt::box_value(Maximum()));
		}
		else
		{
			// Otherwise, we use the value provided.
			SetValue(BindingProperty, winrt::box_value(newSize));
		}

		// We're always manipulating the value effectively.
		return true;
	}
}
