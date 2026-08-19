#pragma once

#include "DockPanel.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <cmath>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace XamlToolkit::WinUI::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct DockPanel : DockPanelT<DockPanel>
	{
		DockPanel() = default;

		winrt::Size ArrangeOverride(winrt::Size finalSize);

		winrt::Size MeasureOverride(winrt::Size availableSize);

		static const wil::single_threaded_property<winrt::DependencyProperty> DockProperty;

		static winrt::Dock GetDock(winrt::FrameworkElement const& obj)
		{
			return winrt::unbox_value<winrt::Dock>(obj.GetValue(DockProperty()));
		}

		static void SetDock(winrt::FrameworkElement const& obj, winrt::Dock const& value)
		{
			obj.SetValue(DockProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> LastChildFillProperty;

		bool LastChildFill() const
		{
			return winrt::unbox_value<bool>(GetValue(LastChildFillProperty()));
		}

		void LastChildFill(bool value)
		{
			SetValue(LastChildFillProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> PaddingProperty;

		winrt::Thickness Padding() const
		{
			return winrt::unbox_value<winrt::Thickness>(GetValue(PaddingProperty()));
		}

		void Padding(winrt::Thickness const& value)
		{
			SetValue(PaddingProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalSpacingProperty;

		double HorizontalSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(HorizontalSpacingProperty()));
		}

		void HorizontalSpacing(double value)
		{
			SetValue(HorizontalSpacingProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> VerticalSpacingProperty;

		double VerticalSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(VerticalSpacingProperty()));
		}

		void VerticalSpacing(double value)
		{
			SetValue(VerticalSpacingProperty(), winrt::box_value(value));
		}

	private:
		static void DockChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct DockPanel : DockPanelT<DockPanel, implementation::DockPanel>
	{
	};
}
