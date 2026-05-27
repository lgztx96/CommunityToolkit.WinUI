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
	private:
		double _columnWidth{ 0.0 };

		int GetColumnIndex(std::span<const double> columnHeights);

		void OnHorizontalAlignmentChanged(DependencyObject const& sender, DependencyProperty const& dp);

		static void OnDesiredColumnWidthChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnPaddingChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:
		StaggeredPanel();

		Size MeasureOverride(Size availableSize);

		Size ArrangeOverride(Size finalSize);

		static inline const wil::single_threaded_property<DependencyProperty> DesiredColumnWidthProperty =
			DependencyProperty::Register(
				L"DesiredColumnWidth",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(250.0), &StaggeredPanel::OnDesiredColumnWidthChanged });

		static inline const wil::single_threaded_property<DependencyProperty> PaddingProperty =
			DependencyProperty::Register(
				L"Padding",
				winrt::xaml_typename<Thickness>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(Thickness{ 0, 0, 0, 0 }), &StaggeredPanel::OnPaddingChanged });

		static inline const wil::single_threaded_property<DependencyProperty> ColumnSpacingProperty =
			DependencyProperty::Register(
				L"ColumnSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(0.0), &StaggeredPanel::OnPaddingChanged });

		static inline const wil::single_threaded_property<DependencyProperty> RowSpacingProperty =
			DependencyProperty::Register(
				L"RowSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(0.0), &StaggeredPanel::OnPaddingChanged });

		double DesiredColumnWidth()
		{
			return winrt::unbox_value<double>(GetValue(DesiredColumnWidthProperty));
		}

		void DesiredColumnWidth(double value)
		{
			SetValue(DesiredColumnWidthProperty, winrt::box_value(value));
		}

		Thickness Padding()
		{
			return winrt::unbox_value<Thickness>(GetValue(PaddingProperty));
		}

		void Padding(Thickness value)
		{
			SetValue(PaddingProperty, winrt::box_value(value));
		}

		double ColumnSpacing()
		{
			return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty));
		}

		void ColumnSpacing(double value)
		{
			SetValue(ColumnSpacingProperty, winrt::box_value(value));
		}

		double RowSpacing()
		{
			return winrt::unbox_value<double>(GetValue(RowSpacingProperty));
		}

		void RowSpacing(double value)
		{
			SetValue(RowSpacingProperty, winrt::box_value(value));
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct StaggeredPanel : StaggeredPanelT<StaggeredPanel, implementation::StaggeredPanel>
	{
	};
}
