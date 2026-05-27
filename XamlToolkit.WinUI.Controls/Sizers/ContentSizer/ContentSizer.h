#pragma once

#include "ContentSizer.g.h"
#include "../SizerBase.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::SizerBase>
	{
		double _currentSize;

		ContentSizer();

		bool IsDragInverted() { return winrt::unbox_value<bool>(GetValue(IsDragInvertedProperty)); }
		void IsDragInverted(bool value) { SetValue(IsDragInvertedProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> IsDragInvertedProperty =
			DependencyProperty::Register(L"IsDragInverted", winrt::xaml_typename<bool>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(false)));

		FrameworkElement TargetControl() { return winrt::unbox_value<FrameworkElement>(GetValue(TargetControlProperty)); }
		void TargetControl(FrameworkElement value) { SetValue(TargetControlProperty, value); }

		static inline const wil::single_threaded_property<DependencyProperty> TargetControlProperty =
			DependencyProperty::Register(L"TargetControl", winrt::xaml_typename<FrameworkElement>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		void OnLoaded(RoutedEventArgs const& e);

		void OnDragStarting();

		bool OnDragHorizontal(double horizontalChange);

		bool OnDragVertical(double verticalChange);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::ContentSizer>
	{
	};
}
