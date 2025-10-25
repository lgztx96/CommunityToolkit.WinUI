#pragma once

#include "DockPanel.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct DockPanel : DockPanelT<DockPanel>
	{
		static void DockChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& e);

		static void OnPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<DependencyProperty> DockProperty =
			DependencyProperty::RegisterAttached(
				L"Dock",
				winrt::xaml_typename<Dock>(),
				winrt::xaml_typename<FrameworkElement>(),
				PropertyMetadata{ winrt::box_value(Dock::Left), &DockPanel::DockChanged });

		static Dock GetDock(FrameworkElement const& obj)
		{
			return winrt::unbox_value<Dock>(obj.GetValue(DockProperty));
		}

		static void SetDock(FrameworkElement const& obj, Dock const& value)
		{
			obj.SetValue(DockProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> LastChildFillProperty =
			DependencyProperty::Register(
				L"LastChildFill",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(true), &DockPanel::OnPropertyChanged });

		bool LastChildFill()
		{
			return winrt::unbox_value<bool>(GetValue(LastChildFillProperty));
		}

		void LastChildFill(bool value)
		{
			SetValue(LastChildFillProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> PaddingProperty =
			DependencyProperty::Register(
				L"Padding",
				winrt::xaml_typename<Thickness>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(Thickness{ 0, 0, 0, 0 }), &DockPanel::OnPropertyChanged });

		Thickness Padding()
		{
			return winrt::unbox_value<Thickness>(GetValue(PaddingProperty));
		}

		void Padding(Thickness const& value)
		{
			SetValue(PaddingProperty, box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> HorizontalSpacingProperty =
			DependencyProperty::Register(
				L"HorizontalSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(0.0), &DockPanel::OnPropertyChanged });

		double HorizontalSpacing()
		{
			return winrt::unbox_value<double>(GetValue(HorizontalSpacingProperty));
		}

		void HorizontalSpacing(double value)
		{
			SetValue(HorizontalSpacingProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> VerticalSpacingProperty =
			DependencyProperty::Register(
				L"VerticalSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{
					winrt::box_value(0.0), &DockPanel::OnPropertyChanged });

		double VerticalSpacing()
		{
			return winrt::unbox_value<double>(GetValue(VerticalSpacingProperty));
		}

		void VerticalSpacing(double value)
		{
			SetValue(VerticalSpacingProperty, winrt::box_value(value));
		}

		Size ArrangeOverride(Size finalSize);

		Size MeasureOverride(Size availableSize);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct DockPanel : DockPanelT<DockPanel, implementation::DockPanel>
	{
	};
}
