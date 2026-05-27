#pragma once

#include "ColorPicker.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#include <initializer_list>
#endif

import winrt.XamlToolkit.WinUI.Controls;
import winrt.XamlToolkit.WinUI.Controls.Primitives;

namespace winrt
{
	using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace XamlToolkit::WinUI;
	using namespace XamlToolkit::WinUI::Helpers;
	using namespace XamlToolkit::WinUI::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ColorPicker : ColorPickerT<ColorPicker>
	{
		wil::single_threaded_rw_property<winrt::Windows::UI::Color> CheckerBackgroundColor = Microsoft::UI::ColorHelper::FromArgb(0x19, 0x80, 0x80, 0x80); // Overridden later
	private:
		/// <summary>
		/// The period that scheduled color updates will be applied.
		/// This is only used when updating colors using the ScheduleColorUpdate() method.
		/// Color changes made directly to the Color property will apply instantly.
		/// </summary>
		static constexpr int ColorUpdateInterval = 30; // Milliseconds

		int64_t tokenColor;

		bool callbacksConnected = false;
		bool eventsConnected = false;
		bool isInitialized = false;

		// Color information for updates
		std::optional<winrt::XamlToolkit::WinUI::HsvColor> savedHsvColor;
		std::optional<winrt::Windows::UI::Color> savedHsvColorRgbEquivalent;
		std::optional<winrt::Windows::UI::Color> updatedRgbColor;
		std::optional<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer> dispatcherQueueTimer;

		Controls::Segmented       ColorPanelSelector{ nullptr };
		Controls::SwitchPresenter ContentContainer{ nullptr };

		ColorSpectrum     ColorSpectrumControl{ nullptr };
		Primitives::ColorPickerSlider ColorSpectrumAlphaSlider{ nullptr };
		Primitives::ColorPickerSlider ColorSpectrumThirdDimensionSlider{ nullptr };
		TextBox           HexInputTextBox{ nullptr };
		ComboBox          ColorModeComboBox{ nullptr };
		ListViewBase      PaletteColorsView{ nullptr };

		NumberBox         Channel1NumberBox{ nullptr };
		NumberBox         Channel2NumberBox{ nullptr };
		NumberBox         Channel3NumberBox{ nullptr };
		NumberBox         AlphaChannelNumberBox{ nullptr };

		Primitives::ColorPickerSlider Channel1Slider{ nullptr };
		Primitives::ColorPickerSlider Channel2Slider{ nullptr };
		Primitives::ColorPickerSlider Channel3Slider{ nullptr };
		Primitives::ColorPickerSlider AlphaChannelSlider{ nullptr };

		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPreviewer ColorPreviewer{ nullptr };

		// Up to 10 checkered backgrounds may be used by name anywhere in the template
		Border CheckeredBackground1Border{ nullptr };
		Border CheckeredBackground2Border{ nullptr };
		Border CheckeredBackground3Border{ nullptr };
		Border CheckeredBackground4Border{ nullptr };
		Border CheckeredBackground5Border{ nullptr };
		Border CheckeredBackground6Border{ nullptr };
		Border CheckeredBackground7Border{ nullptr };
		Border CheckeredBackground8Border{ nullptr };
		Border CheckeredBackground9Border{ nullptr };
		Border CheckeredBackground10Border{ nullptr };

		winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Color>::VectorChanged_revoker _vectorChangedRevoker;

		Selector::SelectionChanged_revoker _colorPanelSelectorSelectionChangedRevoker;

		Selector::SelectionChanged_revoker _colorsViewSelectionChangedRevoker;

		ColorSpectrum::ColorChanged_revoker _colorSpectrumColorChangedRevoker;
		Control::GotFocus_revoker _colorSpectrumGotFocusRevoker;

		Control::KeyDown_revoker _hexInputTextBoxKeyDownRevoker;
		Control::LostFocus_revoker _hexInputTextBoxLostFocusRevoker;

		Selector::SelectionChanged_revoker _colorModeComboBoxSelectionChangedRevoker;

		NumberBox::ValueChanged_revoker _channel1NumberBoxValueChangedRevoker;
		NumberBox::ValueChanged_revoker _channel2NumberBoxValueChangedRevoker;
		NumberBox::ValueChanged_revoker _channel3NumberBoxValueChangedRevoker;
		NumberBox::ValueChanged_revoker _alphaChannelNumberBoxValueChangedRevoker;

		Slider::ValueChanged_revoker _channel1SliderValueChangedRevoker;
		Slider::ValueChanged_revoker _channel2SliderValueChangedRevoker;
		Slider::ValueChanged_revoker _channel3SliderValueChangedRevoker;
		Slider::ValueChanged_revoker _alphaChannelSliderValueChangedRevoker;
		Slider::ValueChanged_revoker _colorSpectrumAlphaSliderValueChangedRevoker;
		Slider::ValueChanged_revoker _colorSpectrumThirdDimensionSliderValueChangedRevoker;

		FrameworkElement::Loaded_revoker _channel1SliderLoadedRevoker;
		FrameworkElement::Loaded_revoker _channel2SliderLoadedRevoker;
		FrameworkElement::Loaded_revoker _channel3SliderLoadedRevoker;
		FrameworkElement::Loaded_revoker _alphaChannelSliderLoadedRevoker;
		FrameworkElement::Loaded_revoker _colorSpectrumAlphaSliderLoadedRevoker;
		FrameworkElement::Loaded_revoker _colorSpectrumThirdDimensionSliderLoadedRevoker;

		Primitives::ColorPreviewer::ColorChangeRequested_revoker _colorPreviewerColorChangeRequestedRevoker;

		FrameworkElement::Loaded_revoker _checkeredBackground1BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground2BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground3BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground4BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground5BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground6BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground7BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground8BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground9BorderLoadedRevoker;
		FrameworkElement::Loaded_revoker _checkeredBackground10BorderLoadedRevoker;

	public:

		static void OnDependencyPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs args);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CustomPaletteColorsProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CustomPaletteColors",
				winrt::xaml_typename<winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Color>>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata(nullptr, &ColorPicker::OnDependencyPropertyChanged));

		winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Color> CustomPaletteColors() const
		{
			return GetValue(CustomPaletteColorsProperty)
				.as<winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Color>>();
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CustomPaletteColumnCountProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CustomPaletteColumnCount",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(4), &ColorPicker::OnDependencyPropertyChanged));

		int CustomPaletteColumnCount() const
		{
			return winrt::unbox_value<int>(GetValue(CustomPaletteColumnCountProperty));
		}

		void CustomPaletteColumnCount(int value)
		{
			if (value != CustomPaletteColumnCount())
			{
				SetValue(CustomPaletteColumnCountProperty, winrt::box_value(value));
			}
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CustomPaletteProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CustomPalette",
				winrt::xaml_typename<IColorPalette>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata(nullptr, &ColorPicker::OnDependencyPropertyChanged));

		IColorPalette CustomPalette() const
		{
			return GetValue(CustomPaletteProperty).as<IColorPalette>();
		}

		void CustomPalette(IColorPalette const& value)
		{
			if (value != CustomPalette())
			{
				SetValue(CustomPaletteProperty, value);
			}
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsColorPaletteVisibleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsColorPaletteVisible",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(true), &ColorPicker::OnDependencyPropertyChanged));

		bool IsColorPaletteVisible() const
		{
			return winrt::unbox_value<bool>(GetValue(IsColorPaletteVisibleProperty));
		}

		void IsColorPaletteVisible(bool value)
		{
			if (value != IsColorPaletteVisible())
			{
				SetValue(IsColorPaletteVisibleProperty, winrt::box_value(value));
			}
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ShowAccentColorsProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"ShowAccentColors",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(true), &ColorPicker::OnDependencyPropertyChanged));

		bool ShowAccentColors() const
		{
			return winrt::unbox_value<bool>(GetValue(ShowAccentColorsProperty));
		}

		void ShowAccentColors(bool value)
		{
			if (value != ShowAccentColors())
			{
				SetValue(ShowAccentColorsProperty, winrt::box_value(value));
			}
		}

		ColorPicker();

		~ColorPicker();

		void OnApplyTemplate();

	private:
		/// <summary>
		/// Gets whether or not the color is considered empty (all fields zero).
		/// In the future Color.IsEmpty will hopefully be added to UWP.
		/// </summary>
		/// <param name="color">The Windows.UI.Color to calculate with.</param>
		/// <returns>Whether the color is considered empty.</returns>
		static bool IsColorEmpty(winrt::Windows::UI::Color color);

		/// <summary>
		/// Connects or disconnects all dependency property callbacks.
		/// </summary>
		void ConnectCallbacks(bool connected);

		/// <summary>
		/// Connects or disconnects all control event handlers.
		/// </summary>
		/// <param name="connected">True to connect event handlers, otherwise false.</param>
		void ConnectEvents(bool connected);

		/// <summary>
		/// Updates all visual states based on current control properties.
		/// </summary>
		/// <param name="useTransitions">Whether transitions should occur when changing states.</param>
		void UpdateVisualState(bool useTransitions = true);

		static int Truth(std::initializer_list<bool> booleans);

		/// <summary>
		/// Gets the active representation of the color: HSV or RGB.
		/// </summary>
		ColorRepresentation GetActiveColorRepresentation();

		/// <summary>
		/// Sets the active color representation in the UI controls.
		/// </summary>
		/// <param name="colorRepresentation">The color representation to set.
		/// Setting to null (the default) will attempt to keep the current state.</param>
		void SetActiveColorRepresentation(std::optional<ColorRepresentation> colorRepresentation = std::nullopt);

		/// <summary>
		/// Gets the active third dimension in the color spectrum: Hue, Saturation or Value.
		/// </summary>
		ColorChannel GetActiveColorSpectrumThirdDimension();

		/// <summary>
		/// Declares a new color to set to the control.
		/// Application of this color will be scheduled to avoid overly rapid updates.
		/// </summary>
		/// <param name="newColor">The new color to set to the control. </param>
		void ScheduleColorUpdate(winrt::Windows::UI::Color newColor);

		/// <summary>
		/// Updates the color values in all editing controls to match the current color.
		/// </summary>
		void UpdateColorControlValues();

		/// <summary>
		/// Sets a new color channel value to the current color.
		/// Only the specified color channel will be modified.
		/// </summary>
		/// <param name="colorRepresentation">The color representation of the given channel.</param>
		/// <param name="channel">The specified color channel to modify.</param>
		/// <param name="newValue">The new color channel value.</param>
		void SetColorChannel(ColorRepresentation colorRepresentation, ColorChannel channel, double newValue);

		/// <summary>
		/// Updates all channel slider control backgrounds.
		/// </summary>
		void UpdateChannelSliderBackgrounds();

		/// <summary>
		/// Updates a specific channel slider control background.
		/// </summary>
		/// <param name="slider">The color channel slider to update the background for.</param>
		void UpdateChannelSliderBackground(Primitives::ColorPickerSlider const& slider);

		/// <summary>
		/// Sets the default color palette to the control.
		/// </summary>
		void SetDefaultPalette();

		/// <summary>
		/// Validates and updates the current 'tab' or 'panel' selection.
		/// If the currently selected tab is collapsed, the next visible tab will be selected.
		/// </summary>
		void ValidateSelectedPanel();

		void OnPanelVisibilityChanged(DependencyObject const& sender, DependencyProperty const& dp);

		/***************************************************************************************
		 *
		 * Color Update Timer
		 *
		 ***************************************************************************************/

		void StartDispatcherQueueTimer();

		void StopDispatcherQueueTimer();

		void DispatcherQueueTimer_Tick(IInspectable const& sender, IInspectable const& e);

		/***************************************************************************************
		 *
		 * Callbacks
		 *
		 ***************************************************************************************/

		 /// <summary>
		 /// Callback for when the <see cref="Microsoft.UI.Xaml.Controls.ColorPicker.Color"/> dependency property value changes.
		 /// </summary>
		void OnColorChanged(DependencyObject const& d, DependencyProperty const& e);

		/***************************************************************************************
		 *
		 * Event Handling
		 *
		 ***************************************************************************************/

		 /// <summary>
		 /// Event handler for when the control has finished loaded.
		 /// </summary>
		void ColorPickerButton_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when a color channel slider is loaded.
		/// This will draw an initial background.
		/// </summary>
		void ChannelSlider_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		/// <summary>
		/// Event handler to draw checkered backgrounds on-demand as controls are loaded.
		/// </summary>
		winrt::Windows::Foundation::IAsyncAction CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the list of custom palette colors is changed.
		/// </summary>
		void CustomPaletteColors_CollectionChanged(IInspectable const& sender, IVectorChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color panel selector selection changes.
		/// We are setting the value here instead of ElementName binding as a workaround for AoT issues.
		/// (See https://github.com/microsoft/microsoft-ui-xaml/issues/10214)
		/// </summary>
		void ColorPanelSelector_SelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color spectrum color is changed.
		/// This occurs when the user presses on the spectrum to select a new color.
		/// </summary>
		void ColorSpectrum_ColorChanged(ColorSpectrum const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);

		/// <summary>
		/// Event handler for when the color spectrum is focused.
		/// This is used only to work around some bugs that cause usability problems.
		/// </summary>
		void ColorSpectrum_GotFocus(IInspectable const& sender, RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the selected color representation changes.
		/// This will convert between RGB and HSV.
		/// </summary>
		void ColorModeComboBox_SelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when a color is selected from the palette.
		/// This will update the current color.
		/// </summary>
		void PaletteColorsView_SelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color previewer requests a new color.
		/// </summary>
		void ColorPreviewer_ColorChangeRequested(IInspectable const& sender, HsvColor hsvColor);

		/// <summary>
		/// Event handler for when a key is pressed within the Hex RGB value TextBox.
		/// This is used to trigger a re-evaluation of the color based on the TextBox value.
		/// </summary>
		void HexInputTextBox_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the Hex RGB value TextBox looses focus.
		/// This is used to trigger a re-evaluation of the color based on the TextBox value.
		/// </summary>
		void HexInputTextBox_LostFocus(IInspectable const& sender, RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the value within one of the channel NumberBoxes is changed.
		/// </summary>
		void ChannelNumberBox_ValueChanged(NumberBox const& sender, NumberBoxValueChangedEventArgs const& args);

		/// <summary>
		/// Event handler for when the value within one of the channel Sliders is changed.
		/// </summary>
		void ChannelSlider_ValueChanged(IInspectable const& sender, RangeBaseValueChangedEventArgs const& e);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ColorPicker : ColorPickerT<ColorPicker, implementation::ColorPicker>
	{
	};
}
