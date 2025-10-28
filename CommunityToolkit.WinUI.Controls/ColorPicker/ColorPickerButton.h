#pragma once

#include "ColorPickerButton.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Microsoft::UI::Xaml;

	struct ColorPickerButton : ColorPickerButtonT<ColorPickerButton>
	{
	private:
		Border CheckeredBackgroundBorder;

	public:
		wil::single_threaded_rw_property<winrt::CommunityToolkit::WinUI::Controls::ColorPicker> ColorPicker;

		winrt::Microsoft::UI::Xaml::Style ColorPickerStyle()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Style>(GetValue(ColorPickerStyleProperty));
		}

		void ColorPickerStyle(winrt::Microsoft::UI::Xaml::Style const& value)
		{
			SetValue(ColorPickerStyleProperty, value);
		}

		static inline const wil::single_threaded_property<DependencyProperty>  ColorPickerStyleProperty = DependencyProperty::Register(L"ColorPickerStyle", winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		winrt::Microsoft::UI::Xaml::Style FlyoutPresenterStyle()
		{
			return winrt::unbox_value<struct Style>(GetValue(FlyoutPresenterStyleProperty));
		}

		void FlyoutPresenterStyle(winrt::Microsoft::UI::Xaml::Style const& value)
		{
			SetValue(FlyoutPresenterStyleProperty, value);
		}

		static inline const wil::single_threaded_property<DependencyProperty>  FlyoutPresenterStyleProperty = DependencyProperty::Register(L"FlyoutPresenterStyle", winrt::xaml_typename<struct Style>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		Color SelectedColor()
		{
			return winrt::unbox_value<Color>(GetValue(SelectedColorProperty));
		}

		void SelectedColor(Color const& value) 
		{
			SetValue(SelectedColorProperty, winrt::box_value(value));
		}

		static void SelectedColorChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<DependencyProperty> SelectedColorProperty =
			DependencyProperty::Register(L"SelectedColor", winrt::xaml_typename<Color>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, &ColorPickerButton::SelectedColorChanged));

		ColorPickerButton();

		void OnApplyTemplate();

	private:
		void ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);

		winrt::Windows::Foundation::IAsyncAction CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		ColorPicker::ColorChanged_revoker _colorChangedRevoker;
		FrameworkElement::Loaded_revoker _checkeredLoadedRevoker;
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct ColorPickerButton : ColorPickerButtonT<ColorPickerButton, implementation::ColorPickerButton>
	{
	};
}
