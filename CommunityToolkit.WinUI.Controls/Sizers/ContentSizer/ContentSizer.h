#pragma once

#include "ContentSizer.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "../SizerBase.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::SizerBase>
	{
		double _currentSize;

		ContentSizer() : _currentSize(0.0) {}

		bool IsDragInverted() { return winrt::unbox_value<bool>(GetValue(IsDragInvertedProperty)); }
		void IsDragInverted(bool value) { SetValue(IsDragInvertedProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> IsDragInvertedProperty =
			DependencyProperty::Register(L"IsDragInverted", winrt::xaml_typename<bool>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(false)));

		FrameworkElement TargetControl() { return winrt::unbox_value<FrameworkElement>(GetValue(TargetControlProperty)); }
		void TargetControl(FrameworkElement value) { SetValue(TargetControlProperty, value); }

		static inline const wil::single_threaded_property<DependencyProperty> TargetControlProperty =
			DependencyProperty::Register(L"TargetControl", winrt::xaml_typename<FrameworkElement>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		template <typename T>
		static T FindAscendant(DependencyObject element)
		{
			while (true)
			{
				DependencyObject parent = winrt::Microsoft::UI::Xaml::Media::VisualTreeHelper::GetParent(element);

				if (!parent)
				{
					return { nullptr };
				}

				if (auto result = parent.try_as<T>())
				{
					return result;
				}

				element = parent;
			}
		}

		void OnLoaded(RoutedEventArgs const& e);

		void OnDragStarting();

		bool OnDragHorizontal(double horizontalChange);

		bool OnDragVertical(double verticalChange);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct ContentSizer : ContentSizerT<ContentSizer, implementation::ContentSizer>
	{
	};
}
