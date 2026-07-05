#pragma once

#include "ColorPicker.g.h"

#ifdef __INTELLISENSE__
#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <initializer_list>
#include <optional>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <winrt/XamlToolkit.WinUI.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.XamlToolkit.WinUI.Controls;
import winrt.XamlToolkit.WinUI.Controls.Primitives;
#endif

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
	using namespace XamlToolkit::WinUI::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct ColorPicker : ColorPickerT<ColorPicker>
	{
		ColorPicker();

		~ColorPicker();

		void OnApplyTemplate();

		wil::single_threaded_rw_property<winrt::Color> CheckerBackgroundColor = winrt::Microsoft::UI::ColorHelper::FromArgb(0x19, 0x80, 0x80, 0x80); // Overridden later

		static const wil::single_threaded_property<winrt::DependencyProperty> CustomPaletteColorsProperty;

		winrt::IObservableVector<winrt::Color> CustomPaletteColors() const
		{
			return GetValue(CustomPaletteColorsProperty()).try_as<winrt::IObservableVector<winrt::Color>>();
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> CustomPaletteColumnCountProperty;

		int CustomPaletteColumnCount() const
		{
			return winrt::unbox_value<int>(GetValue(CustomPaletteColumnCountProperty()));
		}

		void CustomPaletteColumnCount(int value)
		{
			if (value != CustomPaletteColumnCount())
			{
				SetValue(CustomPaletteColumnCountProperty(), winrt::box_value(value));
			}
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> CustomPaletteProperty;

		winrt::XamlToolkit::WinUI::Controls::IColorPalette CustomPalette() const
		{
			return GetValue(CustomPaletteProperty()).try_as<winrt::XamlToolkit::WinUI::Controls::IColorPalette>();
		}

		void CustomPalette(IColorPalette const& value)
		{
			if (value != CustomPalette())
			{
				SetValue(CustomPaletteProperty(), value);
			}
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> IsColorPaletteVisibleProperty;

		bool IsColorPaletteVisible() const
		{
			return winrt::unbox_value<bool>(GetValue(IsColorPaletteVisibleProperty()));
		}

		void IsColorPaletteVisible(bool value)
		{
			if (value != IsColorPaletteVisible())
			{
				SetValue(IsColorPaletteVisibleProperty(), winrt::box_value(value));
			}
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> ShowAccentColorsProperty;

		bool ShowAccentColors() const
		{
			return winrt::unbox_value<bool>(GetValue(ShowAccentColorsProperty()));
		}

		void ShowAccentColors(bool value)
		{
			if (value != ShowAccentColors())
			{
				SetValue(ShowAccentColorsProperty(), winrt::box_value(value));
			}
		}

	private:
		/// <summary>
		/// Gets whether or not the color is considered empty (all fields zero).
		/// In the future Color.IsEmpty will hopefully be added to UWP.
		/// </summary>
		/// <param name="color">The Windows.UI.Color to calculate with.</param>
		/// <returns>Whether the color is considered empty.</returns>
		static bool IsColorEmpty(winrt::Color color);

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
		void ScheduleColorUpdate(winrt::Color newColor);

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
		void UpdateChannelSliderBackground(winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider const& slider);

		/// <summary>
		/// Sets the default color palette to the control.
		/// </summary>
		void SetDefaultPalette();

		/// <summary>
		/// Validates and updates the current 'tab' or 'panel' selection.
		/// If the currently selected tab is collapsed, the next visible tab will be selected.
		/// </summary>
		void ValidateSelectedPanel();

		void OnPanelVisibilityChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		static void OnDependencyPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		/***************************************************************************************
		 *
		 * Color Update Timer
		 *
		 ***************************************************************************************/

		void StartDispatcherQueueTimer();

		void StopDispatcherQueueTimer();

		void DispatcherQueueTimer_Tick(winrt::IInspectable const& sender, winrt::IInspectable const& e);

		/***************************************************************************************
		 *
		 * Callbacks
		 *
		 ***************************************************************************************/

		 /// <summary>
		 /// Callback for when the <see cref="Microsoft.UI.Xaml.Controls.ColorPicker.Color"/> dependency property value changes.
		 /// </summary>
		void OnColorChanged(winrt::DependencyObject const& d, winrt::DependencyProperty const& e);

		/***************************************************************************************
		 *
		 * Event Handling
		 *
		 ***************************************************************************************/

		 /// <summary>
		 /// Event handler for when the control has finished loaded.
		 /// </summary>
		void ColorPickerButton_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when a color channel slider is loaded.
		/// This will draw an initial background.
		/// </summary>
		void ChannelSlider_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		/// <summary>
		/// Event handler to draw checkered backgrounds on-demand as controls are loaded.
		/// </summary>
		winrt::fire_and_forget CheckeredBackgroundBorder_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the list of custom palette colors is changed.
		/// </summary>
		void CustomPaletteColors_CollectionChanged(winrt::IInspectable const& sender, winrt::IVectorChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color panel selector selection changes.
		/// We are setting the value here instead of ElementName binding as a workaround for AoT issues.
		/// (See https://github.com/microsoft/microsoft-ui-xaml/issues/10214)
		/// </summary>
		void ColorPanelSelector_SelectionChanged(winrt::IInspectable const& sender, winrt::SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color spectrum color is changed.
		/// This occurs when the user presses on the spectrum to select a new color.
		/// </summary>
		void ColorSpectrum_ColorChanged(winrt::ColorSpectrum const& sender, winrt::ColorChangedEventArgs const& args);

		/// <summary>
		/// Event handler for when the color spectrum is focused.
		/// This is used only to work around some bugs that cause usability problems.
		/// </summary>
		void ColorSpectrum_GotFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the selected color representation changes.
		/// This will convert between RGB and HSV.
		/// </summary>
		void ColorModeComboBox_SelectionChanged(winrt::IInspectable const& sender, winrt::SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when a color is selected from the palette.
		/// This will update the current color.
		/// </summary>
		void PaletteColorsView_SelectionChanged(winrt::IInspectable const& sender, winrt::SelectionChangedEventArgs const& e);

		/// <summary>
		/// Event handler for when the color previewer requests a new color.
		/// </summary>
		void ColorPreviewer_ColorChangeRequested(winrt::IInspectable const& sender, HsvColor hsvColor);

		/// <summary>
		/// Event handler for when a key is pressed within the Hex RGB value TextBox.
		/// This is used to trigger a re-evaluation of the color based on the winrt::TextBox value.
		/// </summary>
		void HexInputTextBox_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the Hex RGB value winrt::TextBox looses focus.
		/// This is used to trigger a re-evaluation of the color based on the winrt::TextBox value.
		/// </summary>
		void HexInputTextBox_LostFocus(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		/// <summary>
		/// Event handler for when the value within one of the channel NumberBoxes is changed.
		/// </summary>
		void ChannelNumberBox_ValueChanged(winrt::NumberBox const& sender, winrt::NumberBoxValueChangedEventArgs const& args);

		/// <summary>
		/// Event handler for when the value within one of the channel Sliders is changed.
		/// </summary>
		void ChannelSlider_ValueChanged(winrt::IInspectable const& sender, winrt::RangeBaseValueChangedEventArgs const& e);

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
		std::optional<winrt::Color> savedHsvColorRgbEquivalent;
		std::optional<winrt::Color> updatedRgbColor;
		std::optional<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer> dispatcherQueueTimer;

		winrt::Segmented         ColorPanelSelector{ nullptr };
		winrt::SwitchPresenter   ContentContainer{ nullptr };

		winrt::ColorSpectrum     ColorSpectrumControl{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider ColorSpectrumAlphaSlider{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider ColorSpectrumThirdDimensionSlider{ nullptr };
		winrt::TextBox           HexInputTextBox{ nullptr };
		winrt::ComboBox          ColorModeComboBox{ nullptr };
		winrt::ListViewBase      PaletteColorsView{ nullptr };

		winrt::NumberBox         Channel1NumberBox{ nullptr };
		winrt::NumberBox         Channel2NumberBox{ nullptr };
		winrt::NumberBox         Channel3NumberBox{ nullptr };
		winrt::NumberBox         AlphaChannelNumberBox{ nullptr };

		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider Channel1Slider{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider Channel2Slider{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider Channel3Slider{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPickerSlider AlphaChannelSlider{ nullptr };

		winrt::ColorPreviewer ColorPreviewer{ nullptr };

		// Up to 10 checkered backgrounds may be used by name anywhere in the template
		winrt::Border CheckeredBackground1Border{ nullptr };
		winrt::Border CheckeredBackground2Border{ nullptr };
		winrt::Border CheckeredBackground3Border{ nullptr };
		winrt::Border CheckeredBackground4Border{ nullptr };
		winrt::Border CheckeredBackground5Border{ nullptr };
		winrt::Border CheckeredBackground6Border{ nullptr };
		winrt::Border CheckeredBackground7Border{ nullptr };
		winrt::Border CheckeredBackground8Border{ nullptr };
		winrt::Border CheckeredBackground9Border{ nullptr };
		winrt::Border CheckeredBackground10Border{ nullptr };

		winrt::IObservableVector<winrt::Color>::VectorChanged_revoker _vectorChangedRevoker;

		winrt::Selector::SelectionChanged_revoker _colorPanelSelectorSelectionChangedRevoker;

		winrt::Selector::SelectionChanged_revoker _colorsViewSelectionChangedRevoker;

		winrt::ColorSpectrum::ColorChanged_revoker _colorSpectrumColorChangedRevoker;
		winrt::Control::GotFocus_revoker _colorSpectrumGotFocusRevoker;

		winrt::Control::KeyDown_revoker _hexInputTextBoxKeyDownRevoker;
		winrt::Control::LostFocus_revoker _hexInputTextBoxLostFocusRevoker;

		winrt::Selector::SelectionChanged_revoker _colorModeComboBoxSelectionChangedRevoker;

		winrt::NumberBox::ValueChanged_revoker _channel1NumberBoxValueChangedRevoker;
		winrt::NumberBox::ValueChanged_revoker _channel2NumberBoxValueChangedRevoker;
		winrt::NumberBox::ValueChanged_revoker _channel3NumberBoxValueChangedRevoker;
		winrt::NumberBox::ValueChanged_revoker _alphaChannelNumberBoxValueChangedRevoker;

		winrt::Slider::ValueChanged_revoker _channel1SliderValueChangedRevoker;
		winrt::Slider::ValueChanged_revoker _channel2SliderValueChangedRevoker;
		winrt::Slider::ValueChanged_revoker _channel3SliderValueChangedRevoker;
		winrt::Slider::ValueChanged_revoker _alphaChannelSliderValueChangedRevoker;
		winrt::Slider::ValueChanged_revoker _colorSpectrumAlphaSliderValueChangedRevoker;
		winrt::Slider::ValueChanged_revoker _colorSpectrumThirdDimensionSliderValueChangedRevoker;

		winrt::FrameworkElement::Loaded_revoker _channel1SliderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _channel2SliderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _channel3SliderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _alphaChannelSliderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _colorSpectrumAlphaSliderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _colorSpectrumThirdDimensionSliderLoadedRevoker;

		winrt::ColorPreviewer::ColorChangeRequested_revoker _colorPreviewerColorChangeRequestedRevoker;

		winrt::FrameworkElement::Loaded_revoker _checkeredBackground1BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground2BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground3BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground4BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground5BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground6BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground7BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground8BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground9BorderLoadedRevoker;
		winrt::FrameworkElement::Loaded_revoker _checkeredBackground10BorderLoadedRevoker;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ColorPicker : ColorPickerT<ColorPicker, implementation::ColorPicker>
	{
	};
}
