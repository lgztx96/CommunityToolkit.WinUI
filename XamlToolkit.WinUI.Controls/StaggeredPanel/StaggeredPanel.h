#pragma once

#include "StaggeredPanel.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <span>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct StaggeredPanel : StaggeredPanelT<StaggeredPanel>
	{
		StaggeredPanel();

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::Size finalSize);

		static const wil::single_threaded_property<winrt::DependencyProperty> DesiredColumnWidthProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> PaddingProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ColumnSpacingProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> RowSpacingProperty;

		double DesiredColumnWidth() const
		{
			return winrt::unbox_value<double>(GetValue(DesiredColumnWidthProperty()));
		}

		void DesiredColumnWidth(double value)
		{
			SetValue(DesiredColumnWidthProperty(), winrt::box_value(value));
		}

		winrt::Thickness Padding() const
		{
			return winrt::unbox_value<winrt::Thickness>(GetValue(PaddingProperty()));
		}

		void Padding(winrt::Thickness value)
		{
			SetValue(PaddingProperty(), winrt::box_value(value));
		}

		double ColumnSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty()));
		}

		void ColumnSpacing(double value)
		{
			SetValue(ColumnSpacingProperty(), winrt::box_value(value));
		}

		double RowSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(RowSpacingProperty()));
		}

		void RowSpacing(double value)
		{
			SetValue(RowSpacingProperty(), winrt::box_value(value));
		}

	private:
		int GetColumnIndex(std::span<const double> columnHeights);

		void OnHorizontalAlignmentChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		static void OnDesiredColumnWidthChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnPaddingChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		double _columnWidth{ 0.0 };
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct StaggeredPanel : StaggeredPanelT<StaggeredPanel, implementation::StaggeredPanel>
	{
	};
}
