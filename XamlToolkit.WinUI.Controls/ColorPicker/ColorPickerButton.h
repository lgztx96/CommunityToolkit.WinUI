#pragma once

#include "ColorPickerButton.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ColorPickerButton : ColorPickerButtonT<ColorPickerButton>
	{
		ColorPickerButton();

		void OnApplyTemplate();

		wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Controls::ColorPicker> ColorPicker;

		winrt::Style ColorPickerStyle() const
		{
			return GetValue(ColorPickerStyleProperty()).try_as<winrt::Style>();
		}

		void ColorPickerStyle(winrt::Style const& value)
		{
			SetValue(ColorPickerStyleProperty(), value);
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerStyleProperty;

		winrt::Style FlyoutPresenterStyle() const
		{
			return GetValue(FlyoutPresenterStyleProperty()).try_as<winrt::Style>();
		}

		void FlyoutPresenterStyle(winrt::Style const& value)
		{
			SetValue(FlyoutPresenterStyleProperty(), value);
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> FlyoutPresenterStyleProperty;

		winrt::Color SelectedColor() const
		{
			return winrt::unbox_value<winrt::Color>(GetValue(SelectedColorProperty()));
		}

		void SelectedColor(winrt::Color const& value) 
		{
			SetValue(SelectedColorProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> SelectedColorProperty;

	private:
		static void SelectedColorChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		void ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);

		winrt::fire_and_forget CheckeredBackgroundBorder_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		winrt::Microsoft::UI::Xaml::Controls::ColorPicker::ColorChanged_revoker _colorChangedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredLoadedRevoker;

		winrt::Border CheckeredBackgroundBorder;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ColorPickerButton : ColorPickerButtonT<ColorPickerButton, implementation::ColorPickerButton>
	{
	};
}
