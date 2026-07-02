#include "pch.h"
#include "winrt_module_imports.h"
#include "ContentSizer.h"
#if __has_include("ContentSizer.g.cpp")
#include "ContentSizer.g.cpp"
#endif
#include "../XamlToolkit.WinUI/common.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> ContentSizer::IsDragInvertedProperty =
		winrt::DependencyProperty::Register(
			L"IsDragInverted",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(false)));

	const wil::single_threaded_property<winrt::DependencyProperty> ContentSizer::TargetControlProperty =
		winrt::DependencyProperty::Register(
			L"TargetControl",
			winrt::xaml_typename<winrt::FrameworkElement>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	ContentSizer::ContentSizer() : _currentSize(0.0)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void ContentSizer::OnLoaded([[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (TargetControl() == nullptr)
		{
			TargetControl(DependencyObjectEx::FindAscendant<winrt::FrameworkElement>(*this));
		}
	}

	void ContentSizer::OnDragStarting()
	{
		if (const auto targetControl = TargetControl())
		{
			_currentSize =
				Orientation() == winrt::Orientation::Vertical ?
				targetControl.ActualWidth() :
				targetControl.ActualHeight();
		}
	}

	bool ContentSizer::OnDragHorizontal(double horizontalChange)
	{
		if (const auto targetControl = TargetControl())
		{
			horizontalChange = IsDragInverted() ? -horizontalChange : horizontalChange;

			if (!IsValidWidth(targetControl, _currentSize + horizontalChange, ActualWidth()))
			{
				return false;
			}

			targetControl.Width(_currentSize + horizontalChange);

			return true;
		}

		return false;
	}

	bool ContentSizer::OnDragVertical(double verticalChange)
	{
		if (const auto targetControl = TargetControl())
		{
			verticalChange = IsDragInverted() ? -verticalChange : verticalChange;

			if (!IsValidHeight(targetControl, _currentSize + verticalChange, ActualHeight()))
			{
				return false;
			}

			targetControl.Height(_currentSize + verticalChange);

			return true;
		}

		return false;
	}
}
