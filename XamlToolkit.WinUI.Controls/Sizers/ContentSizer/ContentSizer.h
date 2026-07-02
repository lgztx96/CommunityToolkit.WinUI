#pragma once

#include "ContentSizer.g.h"
#include "../SizerBase.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::SizerBase>
	{
		ContentSizer();

		bool IsDragInverted() const { return winrt::unbox_value<bool>(GetValue(IsDragInvertedProperty)); }
		void IsDragInverted(bool value) { SetValue(IsDragInvertedProperty, winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> IsDragInvertedProperty;

		winrt::FrameworkElement TargetControl() const { return winrt::unbox_value<winrt::FrameworkElement>(GetValue(TargetControlProperty)); }
		void TargetControl(winrt::FrameworkElement const& value) { SetValue(TargetControlProperty, value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> TargetControlProperty;

		void OnLoaded(winrt::RoutedEventArgs const& e);

		void OnDragStarting();

		bool OnDragHorizontal(double horizontalChange);

		bool OnDragVertical(double verticalChange);

	private:
		double _currentSize;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::ContentSizer>
	{
	};
}
