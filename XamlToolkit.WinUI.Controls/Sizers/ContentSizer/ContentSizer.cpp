#include "pch.h"
#include "winrt_module_imports.h"
#include "ContentSizer.h"
#if __has_include("ContentSizer.g.cpp")
#include "ContentSizer.g.cpp"
#endif
#include "../XamlToolkit.WinUI/common.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	ContentSizer::ContentSizer() : _currentSize(0.0)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void ContentSizer::OnLoaded([[maybe_unused]] RoutedEventArgs const& e)
	{
		if (TargetControl() == nullptr)
		{
			TargetControl(DependencyObjectEx::FindAscendant<FrameworkElement>(*this));
		}
	}

	void ContentSizer::OnDragStarting()
	{
		if (TargetControl() != nullptr)
		{
			_currentSize =
				Orientation() == Orientation::Vertical ?
				TargetControl().ActualWidth() :
				TargetControl().ActualHeight();
		}
	}

	bool ContentSizer::OnDragHorizontal(double horizontalChange)
	{
		if (TargetControl() == nullptr)
		{
			return true;
		}

		horizontalChange = IsDragInverted() ? -horizontalChange : horizontalChange;

		if (!IsValidWidth(TargetControl(), _currentSize + horizontalChange, ActualWidth()))
		{
			return false;
		}

		TargetControl().Width(_currentSize + horizontalChange);

		return true;
	}

	bool ContentSizer::OnDragVertical(double verticalChange)
	{
		if (TargetControl() == nullptr)
		{
			return false;
		}

		verticalChange = IsDragInverted() ? -verticalChange : verticalChange;

		if (!IsValidHeight(TargetControl(), _currentSize + verticalChange, ActualHeight()))
		{
			return false;
		}

		TargetControl().Height(_currentSize + verticalChange);

		return true;
	}
}
