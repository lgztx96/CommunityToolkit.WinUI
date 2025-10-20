#include "pch.h"
#include "ContentSizer.h"
#if __has_include("ContentSizer.g.cpp")
#include "ContentSizer.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void ContentSizer::OnLoaded([[maybe_unused]] RoutedEventArgs const& e)
	{
		if (TargetControl() == nullptr)
		{
			TargetControl(FindAscendant<FrameworkElement>(*this));
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
