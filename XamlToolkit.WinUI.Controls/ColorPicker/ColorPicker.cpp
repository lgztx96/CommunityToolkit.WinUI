#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#endif
#include "ColorPicker.h"
#if __has_include("ColorPicker.g.cpp")
#include "ColorPicker.g.cpp"
#endif
#include "ColorToHexConverter.h"
#include "ColorPickerRenderingHelpers.h"
#include "FluentColorPalette.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	using WinUIColorPicker = winrt::Microsoft::UI::Xaml::Controls::ColorPicker;

	ColorPicker::ColorPicker()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// Workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/3502
		DefaultStyleResourceUri(Uri(L"ms-appx:///XamlToolkit.WinUI.Controls/Themes/Generic.xaml"));

		// Setup collections
		auto collection = winrt::single_threaded_observable_vector<winrt::Windows::UI::Color>();
		SetValue(CustomPaletteColorsProperty, collection);
		_vectorChangedRevoker = CustomPaletteColors().VectorChanged(winrt::auto_revoke, { this, &ColorPicker::CustomPaletteColors_CollectionChanged });

		Loaded({ this, &ColorPicker::ColorPickerButton_Loaded });

		// Checkered background color is found only one time for performance
		// This may need to change in the future if theme changes should be supported
		CheckerBackgroundColor = winrt::unbox_value<winrt::Windows::UI::Color>(Application::Current().Resources().Lookup(winrt::box_value(L"SystemListLowColor")));

		ConnectCallbacks(true);
		SetDefaultPalette();
		StartDispatcherQueueTimer();
		RegisterPropertyChangedCallback(WinUIColorPicker::IsColorChannelTextInputVisibleProperty(), { this, &ColorPicker::OnPanelVisibilityChanged });
		RegisterPropertyChangedCallback(WinUIColorPicker::IsColorSpectrumVisibleProperty(), { this,&ColorPicker::OnPanelVisibilityChanged });
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="ColorPicker"/> class.
	/// </summary>
	ColorPicker::~ColorPicker()
	{
		StopDispatcherQueueTimer();
	}

	/***************************************************************************************
	 *
	 * Methods
	 *
	 ***************************************************************************************/

	 /// <summary>
	 /// Gets whether or not the color is considered empty (all fields zero).
	 /// In the future Color.IsEmpty will hopefully be added to UWP.
	 /// </summary>
	 /// <param name="color">The Windows.UI.Color to calculate with.</param>
	 /// <returns>Whether the color is considered empty.</returns>
	bool ColorPicker::IsColorEmpty(winrt::Windows::UI::Color color)
	{
		return color.A == 0x00 &&
			color.R == 0x00 &&
			color.G == 0x00 &&
			color.B == 0x00;
	}

	/// <summary>
	/// Overrides when a template is applied in order to get the required controls.
	/// </summary>
	void ColorPicker::OnApplyTemplate()
	{
		// We need to disconnect old events first
		ConnectEvents(false);

		ColorPanelSelector = GetTemplateChild(L"ColorPanelSelector").try_as<Controls::Segmented>();
		ContentContainer = GetTemplateChild(L"ContentContainer").try_as<Controls::SwitchPresenter>();

		ColorSpectrumControl = GetTemplateChild(L"ColorSpectrumControl").try_as<ColorSpectrum>();
		ColorSpectrumAlphaSlider = GetTemplateChild(L"ColorSpectrumAlphaSlider").try_as<Primitives::ColorPickerSlider>();
		ColorSpectrumThirdDimensionSlider = GetTemplateChild(L"ColorSpectrumThirdDimensionSlider").try_as<Primitives::ColorPickerSlider>();

		HexInputTextBox = GetTemplateChild(L"HexInputTextBox").try_as<TextBox>();
		ColorModeComboBox = GetTemplateChild(L"ColorModeComboBox").try_as<ComboBox>();
		PaletteColorsView = GetTemplateChild(L"PaletteColorsView").try_as<ListViewBase>();

		Channel1NumberBox = GetTemplateChild(L"Channel1NumberBox").try_as<NumberBox>();
		Channel2NumberBox = GetTemplateChild(L"Channel2NumberBox").try_as<NumberBox>();
		Channel3NumberBox = GetTemplateChild(L"Channel3NumberBox").try_as<NumberBox>();
		AlphaChannelNumberBox = GetTemplateChild(L"AlphaChannelNumberBox").try_as<NumberBox>();

		Channel1Slider = GetTemplateChild(L"Channel1Slider").try_as<Primitives::ColorPickerSlider>();
		Channel2Slider = GetTemplateChild(L"Channel2Slider").try_as<Primitives::ColorPickerSlider>();
		Channel3Slider = GetTemplateChild(L"Channel3Slider").try_as<Primitives::ColorPickerSlider>();
		AlphaChannelSlider = GetTemplateChild(L"AlphaChannelSlider").try_as<Primitives::ColorPickerSlider>();

		ColorPreviewer = GetTemplateChild(L"ColorPreviewer").try_as<winrt::XamlToolkit::WinUI::Controls::Primitives::ColorPreviewer>();

		CheckeredBackground1Border = GetTemplateChild(L"CheckeredBackground1Border").try_as<Border>();
		CheckeredBackground2Border = GetTemplateChild(L"CheckeredBackground2Border").try_as<Border>();
		CheckeredBackground3Border = GetTemplateChild(L"CheckeredBackground3Border").try_as<Border>();
		CheckeredBackground4Border = GetTemplateChild(L"CheckeredBackground4Border").try_as<Border>();
		CheckeredBackground5Border = GetTemplateChild(L"CheckeredBackground5Border").try_as<Border>();
		CheckeredBackground6Border = GetTemplateChild(L"CheckeredBackground6Border").try_as<Border>();
		CheckeredBackground7Border = GetTemplateChild(L"CheckeredBackground7Border").try_as<Border>();
		CheckeredBackground8Border = GetTemplateChild(L"CheckeredBackground8Border").try_as<Border>();
		CheckeredBackground9Border = GetTemplateChild(L"CheckeredBackground9Border").try_as<Border>();
		CheckeredBackground10Border = GetTemplateChild(L"CheckeredBackground10Border").try_as<Border>();

		// Must connect after controls are resolved
		ConnectEvents(true);

		base_type::OnApplyTemplate();
		UpdateVisualState(false);
		ValidateSelectedPanel();
		isInitialized = true;

		SetActiveColorRepresentation(ColorRepresentation::Rgba);
		UpdateColorControlValues(); // TODO: This will also connect events after, can we optimize vs. doing it twice with the ConnectEvents above?
	}

	/// <summary>
	/// Connects or disconnects all dependency property callbacks.
	/// </summary>
	void ColorPicker::ConnectCallbacks(bool connected)
	{
		if (connected == true &&
			callbacksConnected == false)
		{
			// Add callbacks for dependency properties
			tokenColor = RegisterPropertyChangedCallback(WinUIColorPicker::ColorProperty(), { this, &ColorPicker::OnColorChanged });

			callbacksConnected = true;
		}
		else if (connected == false &&
			callbacksConnected == true)
		{
			// Remove callbacks for dependency properties
			UnregisterPropertyChangedCallback(WinUIColorPicker::ColorProperty(), tokenColor);

			callbacksConnected = false;
		}
	}

	/// <summary>
	/// Connects or disconnects all control event handlers.
	/// </summary>
	/// <param name="connected">True to connect event handlers, otherwise false.</param>
	void ColorPicker::ConnectEvents(bool connected)
	{
		if (connected == true &&
			eventsConnected == false)
		{
			// Add all events
			if (ColorPanelSelector)
			{
				_colorPanelSelectorSelectionChangedRevoker = ColorPanelSelector.SelectionChanged(
					winrt::auto_revoke, { this, &ColorPicker::ColorPanelSelector_SelectionChanged });
			}

			if (ColorSpectrumControl) 
			{ 
				_colorSpectrumColorChangedRevoker = ColorSpectrumControl.ColorChanged(winrt::auto_revoke, { this, &ColorPicker::ColorSpectrum_ColorChanged });
			}

			if (ColorSpectrumControl)
			{ 
				_colorSpectrumGotFocusRevoker = ColorSpectrumControl.GotFocus(winrt::auto_revoke, { this, &ColorPicker::ColorSpectrum_GotFocus });
			}

			if (HexInputTextBox) 
			{ 
				_hexInputTextBoxKeyDownRevoker = HexInputTextBox.KeyDown(winrt::auto_revoke, { this, &ColorPicker::HexInputTextBox_KeyDown }); 
			}

			if (HexInputTextBox) 
			{ 
				_hexInputTextBoxLostFocusRevoker = HexInputTextBox.LostFocus(winrt::auto_revoke, { this, &ColorPicker::HexInputTextBox_LostFocus });
			}

			if (ColorModeComboBox)
			{ 
				_colorModeComboBoxSelectionChangedRevoker = ColorModeComboBox.SelectionChanged(
					winrt::auto_revoke, { this, &ColorPicker::ColorModeComboBox_SelectionChanged });
			}

			if (PaletteColorsView)
			{
				_colorsViewSelectionChangedRevoker = PaletteColorsView.SelectionChanged(
					winrt::auto_revoke, { this, &ColorPicker::PaletteColorsView_SelectionChanged });
			}

			if (Channel1NumberBox) 
			{
				_channel1NumberBoxValueChangedRevoker = Channel1NumberBox.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelNumberBox_ValueChanged });
			}

			if (Channel2NumberBox) 
			{ 
				_channel2NumberBoxValueChangedRevoker = Channel2NumberBox.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelNumberBox_ValueChanged }); 
			}

			if (Channel3NumberBox)
			{ 
				_channel3NumberBoxValueChangedRevoker = Channel3NumberBox.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelNumberBox_ValueChanged });
			}

			if (AlphaChannelNumberBox) 
			{ 
				_alphaChannelNumberBoxValueChangedRevoker = AlphaChannelNumberBox.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelNumberBox_ValueChanged });
			}

			if (Channel1Slider) 
			{ 
				_channel1SliderValueChangedRevoker = Channel1Slider.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged });
			}

			if (Channel2Slider)
			{ 
				_channel2SliderValueChangedRevoker = Channel2Slider.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged });
			}

			if (Channel3Slider)
			{ 
				_channel3SliderValueChangedRevoker = Channel3Slider.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged });
			}

			if (AlphaChannelSlider)
			{ 
				_alphaChannelSliderValueChangedRevoker = AlphaChannelSlider.ValueChanged(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged });
			}

			if (ColorSpectrumAlphaSlider) 
			{ 
				_colorSpectrumAlphaSliderValueChangedRevoker = ColorSpectrumAlphaSlider.ValueChanged(
					winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged }); 
			}

			if (ColorSpectrumThirdDimensionSlider)
			{ 
				_colorSpectrumThirdDimensionSliderValueChangedRevoker = ColorSpectrumThirdDimensionSlider.ValueChanged(
					winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_ValueChanged });
			}

			if (Channel1Slider)
			{
				_channel1SliderLoadedRevoker = Channel1Slider.Loaded(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded });
			}

			if (Channel2Slider) 
			{ 
				_channel2SliderLoadedRevoker = Channel2Slider.Loaded(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded }); 
			}

			if (Channel3Slider)
			{ 
				_channel3SliderLoadedRevoker = Channel3Slider.Loaded(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded });
			}

			if (AlphaChannelSlider)
			{ 
				_alphaChannelSliderLoadedRevoker = AlphaChannelSlider.Loaded(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded });
			}

			if (ColorSpectrumAlphaSlider) 
			{ 
				_colorSpectrumAlphaSliderLoadedRevoker = ColorSpectrumAlphaSlider.Loaded(winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded }); 
			}

			if (ColorSpectrumThirdDimensionSlider) 
			{ 
				_colorSpectrumThirdDimensionSliderLoadedRevoker = ColorSpectrumThirdDimensionSlider.Loaded(
				winrt::auto_revoke, { this, &ColorPicker::ChannelSlider_Loaded });
			}

			if (ColorPreviewer) 
			{ 
				_colorPreviewerColorChangeRequestedRevoker = ColorPreviewer.ColorChangeRequested(
				winrt::auto_revoke, { this, &ColorPicker::ColorPreviewer_ColorChangeRequested }); 
			}

			if (CheckeredBackground1Border) 
			{ 
				_checkeredBackground1BorderLoadedRevoker = CheckeredBackground1Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			if (CheckeredBackground2Border) 
			{ 
				_checkeredBackground2BorderLoadedRevoker = CheckeredBackground2Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}
			if (CheckeredBackground3Border) 
			{ 
				_checkeredBackground3BorderLoadedRevoker = CheckeredBackground3Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded }); 
			}

			if (CheckeredBackground4Border)
			{ 
				_checkeredBackground4BorderLoadedRevoker = CheckeredBackground4Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			if (CheckeredBackground5Border) 
			{
				_checkeredBackground5BorderLoadedRevoker = CheckeredBackground5Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			if (CheckeredBackground6Border) 
			{
				_checkeredBackground6BorderLoadedRevoker = CheckeredBackground6Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded }); 
			}

			if (CheckeredBackground7Border) 
			{ _checkeredBackground7BorderLoadedRevoker = CheckeredBackground7Border.Loaded(
				winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded }); 
			}

			if (CheckeredBackground8Border) 
			{ 
				_checkeredBackground8BorderLoadedRevoker = CheckeredBackground8Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			if (CheckeredBackground9Border) 
			{ 
				_checkeredBackground9BorderLoadedRevoker = CheckeredBackground9Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			if (CheckeredBackground10Border) 
			{
				_checkeredBackground10BorderLoadedRevoker = CheckeredBackground10Border.Loaded(
					winrt::auto_revoke, { this, &ColorPicker::CheckeredBackgroundBorder_Loaded });
			}

			eventsConnected = true;
		}
		else if (connected == false && eventsConnected == true)
		{
			_colorPanelSelectorSelectionChangedRevoker.revoke();

			_colorSpectrumColorChangedRevoker.revoke();
			_colorSpectrumGotFocusRevoker.revoke();
			_hexInputTextBoxKeyDownRevoker.revoke();
			_hexInputTextBoxLostFocusRevoker.revoke();
			_colorModeComboBoxSelectionChangedRevoker.revoke();
			_colorsViewSelectionChangedRevoker.revoke();

			_channel1NumberBoxValueChangedRevoker.revoke();
			_channel2NumberBoxValueChangedRevoker.revoke();
			_channel3NumberBoxValueChangedRevoker.revoke();
			_alphaChannelNumberBoxValueChangedRevoker.revoke();

			_channel1SliderValueChangedRevoker.revoke();
			_channel2SliderValueChangedRevoker.revoke();
			_channel3SliderValueChangedRevoker.revoke();
			_alphaChannelSliderValueChangedRevoker.revoke();
			_colorSpectrumAlphaSliderValueChangedRevoker.revoke();
			_colorSpectrumThirdDimensionSliderValueChangedRevoker.revoke();

			_channel1SliderLoadedRevoker.revoke();
			_channel2SliderLoadedRevoker.revoke();
			_channel3SliderLoadedRevoker.revoke();
			_alphaChannelSliderLoadedRevoker.revoke();
			_colorSpectrumAlphaSliderLoadedRevoker.revoke();
			_colorSpectrumThirdDimensionSliderLoadedRevoker.revoke();

			_colorPreviewerColorChangeRequestedRevoker.revoke();

			_checkeredBackground1BorderLoadedRevoker.revoke();
			_checkeredBackground2BorderLoadedRevoker.revoke();
			_checkeredBackground3BorderLoadedRevoker.revoke();
			_checkeredBackground4BorderLoadedRevoker.revoke();
			_checkeredBackground5BorderLoadedRevoker.revoke();
			_checkeredBackground6BorderLoadedRevoker.revoke();
			_checkeredBackground7BorderLoadedRevoker.revoke();
			_checkeredBackground8BorderLoadedRevoker.revoke();
			_checkeredBackground9BorderLoadedRevoker.revoke();
			_checkeredBackground10BorderLoadedRevoker.revoke();

			eventsConnected = false;
		}
	}

	/// <summary>
	/// Updates all visual states based on current control properties.
	/// </summary>
	/// <param name="useTransitions">Whether transitions should occur when changing states.</param>
	void ColorPicker::UpdateVisualState(bool useTransitions)
	{
		VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", useTransitions);
		VisualStateManager::GoToState(*this, GetActiveColorRepresentation() == ColorRepresentation::Hsva ? L"HsvSelected" : L"RgbSelected", useTransitions);
		VisualStateManager::GoToState(*this, IsColorPaletteVisible() ? L"ColorPaletteVisible" : L"ColorPaletteCollapsed", useTransitions);

		// Check if only a single vie is selected and hide the Segmented control
		VisualStateManager::GoToState(*this, (Truth({ IsColorPaletteVisible(), IsColorSpectrumVisible(), IsColorChannelTextInputVisible() }) <= 1) ? L"ColorPanelSelectorCollapsed" : L"ColorPanelSelectorVisible", useTransitions);
	}

	int ColorPicker::Truth(std::initializer_list<bool> booleans)
	{
		return static_cast<int>(std::count_if(booleans.begin(), booleans.end(), [](bool b) { return b; }));
	}

	/// <summary>
	/// Gets the active representation of the color: HSV or RGB.
	/// </summary>
	ColorRepresentation ColorPicker::GetActiveColorRepresentation()
	{
		// If the HSV representation control is missing for whatever reason,
		// the default will be RGB

		if (ColorModeComboBox &&
			ColorModeComboBox.SelectedIndex() == 1)
		{
			return ColorRepresentation::Hsva;
		}

		return ColorRepresentation::Rgba;
	}

	/// <summary>
	/// Sets the active color representation in the UI controls.
	/// </summary>
	/// <param name="colorRepresentation">The color representation to set.
	/// Setting to null (the default) will attempt to keep the current state.</param>
	void ColorPicker::SetActiveColorRepresentation(std::optional<ColorRepresentation> colorRepresentation)
	{
		bool eventsDisconnectedByMethod = false;

		if (!colorRepresentation)
		{
			// Use the control's current value
			colorRepresentation = GetActiveColorRepresentation();
		}

		// Disable events during the update
		if (eventsConnected)
		{
			ConnectEvents(false);
			eventsDisconnectedByMethod = true;
		}

		// Sync the UI controls and visual state
		// The default is always RGBA
		if (colorRepresentation == ColorRepresentation::Hsva)
		{
			ColorModeComboBox.SelectedIndex(1);
		}
		else
		{
			ColorModeComboBox.SelectedIndex(0);
		}

		UpdateVisualState(false);

		if (eventsDisconnectedByMethod)
		{
			ConnectEvents(true);
		}
	}

	/// <summary>
	/// Gets the active third dimension in the color spectrum: Hue, Saturation or Value.
	/// </summary>
	ColorChannel ColorPicker::GetActiveColorSpectrumThirdDimension()
	{
		switch (ColorSpectrumComponents())
		{
		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::SaturationValue:
		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::ValueSaturation:
		{
			// Hue
			return ColorChannel::Channel1;
		}

		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::HueValue:
		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::ValueHue:
		{
			// Saturation
			return ColorChannel::Channel2;
		}

		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::HueSaturation:
		case Microsoft::UI::Xaml::Controls::ColorSpectrumComponents::SaturationHue:
		{
			// Value
			return ColorChannel::Channel3;
		}
		}

		return ColorChannel::Alpha; // Error, should never get here
	}

	/// <summary>
	/// Declares a new color to set to the control.
	/// Application of this color will be scheduled to avoid overly rapid updates.
	/// </summary>
	/// <param name="newColor">The new color to set to the control. </param>
	void ColorPicker::ScheduleColorUpdate(winrt::Windows::UI::Color newColor)
	{
		// Coerce the value as needed
		if (IsAlphaEnabled() == false)
		{
			newColor = winrt::Windows::UI::Color
			{
				.A = 255,
				.R = newColor.R,
				.G = newColor.G,
				.B = newColor.B,
			};
		}

		updatedRgbColor = newColor;
	}

	/// <summary>
	/// Updates the color values in all editing controls to match the current color.
	/// </summary>
	void ColorPicker::UpdateColorControlValues()
	{
		bool eventsDisconnectedByMethod = false;
		winrt::Windows::UI::Color rgbColor = Color();
		HsvColor hsvColor;

		if (isInitialized)
		{
			// Disable events during the update
			if (eventsConnected)
			{
				ConnectEvents(false);
				eventsDisconnectedByMethod = true;
			}

			if (HexInputTextBox)
			{
				if (IsAlphaEnabled())
				{
					// Remove only the "#" sign
					auto hex = Helpers::ColorHelper::ToHex(rgbColor);
					HexInputTextBox.Text(winrt::hstring{ hex.data() + 1, hex.size() - 1 });
				}
				else
				{
					// Remove the "#" sign and alpha hex
					auto hex = Helpers::ColorHelper::ToHex(rgbColor);
					HexInputTextBox.Text(winrt::hstring{ hex.data() + 3, hex.size() - 3 });
				}
			}

			// Regardless of the active color representation, the spectrum is always HSV
			// Therefore, always calculate HSV color here
			// Warning: Always maintain/use HSV information in the saved HSV color
			// This avoids loss of precision and drift caused by continuously converting to/from RGB
			if (!savedHsvColor)
			{
				hsvColor = Helpers::ColorHelper::ToHsv(rgbColor);

				static auto round = [](double value, int decimals) -> double
					{
						double factor = std::pow(10.0, decimals);
						return std::round(value * factor) / factor;
					};
				// Round the channels, be sure rounding matches with the scaling next
				// Rounding of SVA requires at MINIMUM 2 decimal places
				int decimals = 0;
				hsvColor = HsvColor
				{
					.H = round(hsvColor.H, decimals),
					.S = round(hsvColor.S, 2 + decimals),
					.V = round(hsvColor.V, 2 + decimals),
					.A = round(hsvColor.A, 2 + decimals)
				};

				// Must update HSV color
				savedHsvColor = hsvColor;
				savedHsvColorRgbEquivalent = rgbColor;
			}
			else
			{
				hsvColor = savedHsvColor.value();
			}

			// Update the color spectrum
			// Remember the spectrum is always HSV and must be updated as such to avoid
			// conversion errors
			if (ColorSpectrumControl)
			{
				winrt::Windows::Foundation::Numerics::float4 hsvColorFloat4;
				hsvColorFloat4.x = static_cast<float>(hsvColor.H);
				hsvColorFloat4.y = static_cast<float>(hsvColor.S);
				hsvColorFloat4.z = static_cast<float>(hsvColor.V);
				hsvColorFloat4.w = static_cast<float>(hsvColor.A);
				ColorSpectrumControl.HsvColor(hsvColorFloat4);
			}

			// Update the color spectrum third dimension channel
			if (ColorSpectrumThirdDimensionSlider)
			{
				// Convert the channels into a usable range for the user
				double hue = hsvColor.H;
				double staturation = hsvColor.S * 100;
				double value = hsvColor.V * 100;

				switch (GetActiveColorSpectrumThirdDimension())
				{
				case ColorChannel::Channel1:
				{
					// Hue
					ColorSpectrumThirdDimensionSlider.Minimum(0);
					ColorSpectrumThirdDimensionSlider.Maximum(360);
					ColorSpectrumThirdDimensionSlider.Value(hue);
					break;
				}

				case ColorChannel::Channel2:
				{
					// Saturation
					ColorSpectrumThirdDimensionSlider.Minimum(0);
					ColorSpectrumThirdDimensionSlider.Maximum(100);
					ColorSpectrumThirdDimensionSlider.Value(staturation);
					break;
				}

				case ColorChannel::Channel3:
				{
					// Value
					ColorSpectrumThirdDimensionSlider.Minimum(0);
					ColorSpectrumThirdDimensionSlider.Maximum(100);
					ColorSpectrumThirdDimensionSlider.Value(value);
					break;
				}
				}
			}

			// Update the preview color
			if (ColorPreviewer)
			{
				ColorPreviewer.HsvColor(hsvColor);
			}

			// Update all other color channels
			if (GetActiveColorRepresentation() == ColorRepresentation::Hsva)
			{
				// Convert the channels into a usable range for the user
				double hue = hsvColor.H;
				double staturation = hsvColor.S * 100;
				double value = hsvColor.V * 100;
				double alpha = hsvColor.A * 100;

				// Hue
				if (Channel1NumberBox)
				{
					Channel1NumberBox.Minimum(0);
					Channel1NumberBox.Maximum(360);
					Channel1NumberBox.Value(hue);
				}

				if (Channel1Slider)
				{
					Channel1Slider.Minimum(0);
					Channel1Slider.Maximum(360);
					Channel1Slider.Value(hue);
				}

				// Saturation
				if (Channel2NumberBox)
				{
					Channel2NumberBox.Minimum(0);
					Channel2NumberBox.Maximum(100);
					Channel2NumberBox.Value(staturation);
				}

				if (Channel2Slider)
				{
					Channel2Slider.Minimum(0);
					Channel2Slider.Maximum(100);
					Channel2Slider.Value(staturation);
				}

				// Value
				if (Channel3NumberBox)
				{
					Channel3NumberBox.Minimum(0);
					Channel3NumberBox.Maximum(100);
					Channel3NumberBox.Value(value);
				}

				if (Channel3Slider)
				{
					Channel3Slider.Minimum(0);
					Channel3Slider.Maximum(100);
					Channel3Slider.Value(value);
				}

				// Alpha
				if (AlphaChannelNumberBox)
				{
					AlphaChannelNumberBox.Minimum(0);
					AlphaChannelNumberBox.Maximum(100);
					AlphaChannelNumberBox.Value(alpha);
				}

				if (AlphaChannelSlider)
				{
					AlphaChannelSlider.Minimum(0);
					AlphaChannelSlider.Maximum(100);
					AlphaChannelSlider.Value(alpha);
				}

				// Color spectrum alpha
				if (ColorSpectrumAlphaSlider)
				{
					ColorSpectrumAlphaSlider.Minimum(0);
					ColorSpectrumAlphaSlider.Maximum(100);
					ColorSpectrumAlphaSlider.Value(alpha);
				}
			}
			else
			{
				// Red
				if (Channel1NumberBox)
				{
					Channel1NumberBox.Minimum(0);
					Channel1NumberBox.Maximum(255);
					Channel1NumberBox.Value(static_cast<double>(rgbColor.R));
				}

				if (Channel1Slider)
				{
					Channel1Slider.Minimum(0);
					Channel1Slider.Maximum(255);
					Channel1Slider.Value(static_cast<double>(rgbColor.R));
				}

				// Green
				if (Channel2NumberBox)
				{
					Channel2NumberBox.Minimum(0);
					Channel2NumberBox.Maximum(255);
					Channel2NumberBox.Value(static_cast<double>(rgbColor.G));
				}

				if (Channel2Slider)
				{
					Channel2Slider.Minimum(0);
					Channel2Slider.Maximum(255);
					Channel2Slider.Value(static_cast<double>(rgbColor.G));
				}

				// Blue
				if (Channel3NumberBox)
				{
					Channel3NumberBox.Minimum(0);
					Channel3NumberBox.Maximum(255);
					Channel3NumberBox.Value(static_cast<double>(rgbColor.B));
				}

				if (Channel3Slider)
				{
					Channel3Slider.Minimum(0);
					Channel3Slider.Maximum(255);
					Channel3Slider.Value(static_cast<double>(rgbColor.B));
				}

				// Alpha
				if (AlphaChannelNumberBox)
				{
					AlphaChannelNumberBox.Minimum(0);
					AlphaChannelNumberBox.Maximum(255);
					AlphaChannelNumberBox.Value(static_cast<double>(rgbColor.A));
				}

				if (AlphaChannelSlider)
				{
					AlphaChannelSlider.Minimum(0);
					AlphaChannelSlider.Maximum(255);
					AlphaChannelSlider.Value(static_cast<double>(rgbColor.A));
				}

				// Color spectrum alpha
				if (ColorSpectrumAlphaSlider)
				{
					ColorSpectrumAlphaSlider.Minimum(0);
					ColorSpectrumAlphaSlider.Maximum(255);
					ColorSpectrumAlphaSlider.Value(static_cast<double>(rgbColor.A));
				}
			}

			if (PaletteColorsView != nullptr)
			{
				bool contains = false;
				if (auto colors = CustomPaletteColors())
				{
					uint32_t index;
					contains = colors.IndexOf(rgbColor, index);
				}

				PaletteColorsView.SelectedItem(contains ? winrt::box_value(rgbColor) : nullptr);
			}

			if (eventsDisconnectedByMethod)
			{
				ConnectEvents(true);
			}
		}
	}

	/// <summary>
	/// Sets a new color channel value to the current color.
	/// Only the specified color channel will be modified.
	/// </summary>
	/// <param name="colorRepresentation">The color representation of the given channel.</param>
	/// <param name="channel">The specified color channel to modify.</param>
	/// <param name="newValue">The new color channel value.</param>
	void ColorPicker::SetColorChannel(
		ColorRepresentation colorRepresentation,
		ColorChannel channel,
		double newValue)
	{
		winrt::Windows::UI::Color oldRgbColor = Color();
		winrt::Windows::UI::Color newRgbColor;
		HsvColor oldHsvColor;

		if (colorRepresentation == ColorRepresentation::Hsva)
		{
			// Warning: Always maintain/use HSV information in the saved HSV color
			// This avoids loss of precision and drift caused by continuously converting to/from RGB
			if (!savedHsvColor)
			{
				oldHsvColor = Helpers::ColorHelper::ToHsv(oldRgbColor);
			}
			else
			{
				oldHsvColor = savedHsvColor.value();
			}

			double hue = oldHsvColor.H;
			double saturation = oldHsvColor.S;
			double value = oldHsvColor.V;
			double alpha = oldHsvColor.A;

			switch (channel)
			{
			case ColorChannel::Channel1:
			{
				hue = std::clamp((std::isnan(newValue) ? 0.0 : newValue), 0.0, 360.0);
				break;
			}

			case ColorChannel::Channel2:
			{
				saturation = std::clamp((std::isnan(newValue) ? 0 : newValue) / 100.0, 0.0, 1.0);
				break;
			}

			case ColorChannel::Channel3:
			{
				value = std::clamp((std::isnan(newValue) ? 0 : newValue) / 100, 0.0, 1.0);
				break;
			}

			case ColorChannel::Alpha:
			{
				// Unlike color channels, default to no transparency
				alpha = std::clamp((std::isnan(newValue) ? 100 : newValue) / 100, 0.0, 1.0);
				break;
			}
			}

			newRgbColor = Helpers::ColorHelper::FromHsv(
				hue,
				saturation,
				value,
				alpha);

			// Must update HSV color
			savedHsvColor = HsvColor
			{
				.H = hue,
				.S = saturation,
				.V = value,
				.A = alpha
			};
			savedHsvColorRgbEquivalent = newRgbColor;
		}
		else
		{
			byte red = oldRgbColor.R;
			byte green = oldRgbColor.G;
			byte blue = oldRgbColor.B;
			byte alpha = oldRgbColor.A;

			switch (channel)
			{
			case ColorChannel::Channel1:
			{
				red = static_cast<uint8_t>(std::clamp(std::isnan(newValue) ? 0 : newValue, 0.0, 255.0));
				break;
			}

			case ColorChannel::Channel2:
			{
				green = static_cast<uint8_t>(std::clamp(std::isnan(newValue) ? 0 : newValue, 0.0, 255.0));
				break;
			}

			case ColorChannel::Channel3:
			{
				blue = static_cast<uint8_t>(std::clamp(std::isnan(newValue) ? 0 : newValue, 0.0, 255.0));
				break;
			}

			case ColorChannel::Alpha:
			{
				// Unlike color channels, default to no transparency
				alpha = static_cast<uint8_t>(std::clamp(std::isnan(newValue) ? 255 : newValue, 0.0, 255.0));
				break;
			}
			}

			newRgbColor = winrt::Windows::UI::Color
			{
				.A = alpha,
				.R = red,
				.G = green,
				.B = blue,
			};

			// Must clear saved HSV color
			savedHsvColor = std::nullopt;
			savedHsvColorRgbEquivalent = std::nullopt;
		}

		ScheduleColorUpdate(newRgbColor);
	}

	/// <summary>
	/// Updates all channel slider control backgrounds.
	/// </summary>
	void ColorPicker::UpdateChannelSliderBackgrounds()
	{
		UpdateChannelSliderBackground(Channel1Slider);
		UpdateChannelSliderBackground(Channel2Slider);
		UpdateChannelSliderBackground(Channel3Slider);
		UpdateChannelSliderBackground(AlphaChannelSlider);
		UpdateChannelSliderBackground(ColorSpectrumAlphaSlider);
		UpdateChannelSliderBackground(ColorSpectrumThirdDimensionSlider);
	}

	/// <summary>
	/// Updates a specific channel slider control background.
	/// </summary>
	/// <param name="slider">The color channel slider to update the background for.</param>
	void ColorPicker::UpdateChannelSliderBackground(Primitives::ColorPickerSlider const& slider)
	{
		if (slider)
		{
			// Regardless of the active color representation, the sliders always use HSV
			// Therefore, always calculate HSV color here
			// Warning: Always maintain/use HSV information in the saved HSV color
			// This avoids loss of precision and drift caused by continuously converting to/from RGB
			if (savedHsvColor == std::nullopt)
			{
				auto rgbColor = Color();

				// Must update HSV color
				savedHsvColor = Helpers::ColorHelper::ToHsv(rgbColor);
				savedHsvColorRgbEquivalent = rgbColor;
			}

			slider.IsAutoUpdatingEnabled(false);

			if (slider == Channel1Slider)
			{
				slider.ColorChannel(ColorChannel::Channel1);
				slider.ColorRepresentation(GetActiveColorRepresentation());
			}
			else if (slider == Channel2Slider)
			{
				slider.ColorChannel(ColorChannel::Channel2);
				slider.ColorRepresentation(GetActiveColorRepresentation());
			}
			else if (slider == Channel3Slider)
			{
				slider.ColorChannel(ColorChannel::Channel3);
				slider.ColorRepresentation(GetActiveColorRepresentation());
			}
			else if (slider == AlphaChannelSlider)
			{
				slider.ColorChannel(ColorChannel::Alpha);
				slider.ColorRepresentation(GetActiveColorRepresentation());
			}
			else if (slider == ColorSpectrumAlphaSlider)
			{
				slider.ColorChannel(ColorChannel::Alpha);
				slider.ColorRepresentation(GetActiveColorRepresentation());
			}
			else if (slider == ColorSpectrumThirdDimensionSlider)
			{
				slider.ColorChannel(GetActiveColorSpectrumThirdDimension());
				slider.ColorRepresentation(ColorRepresentation::Hsva); // Always HSV
			}

			slider.HsvColor(savedHsvColor.value());
			slider.UpdateColors();
		}
	}

	/// <summary>
	/// Sets the default color palette to the control.
	/// </summary>
	void ColorPicker::SetDefaultPalette()
	{
		CustomPalette(winrt::make<FluentColorPalette>());
	}

	/// <summary>
	/// Validates and updates the current 'tab' or 'panel' selection.
	/// If the currently selected tab is collapsed, the next visible tab will be selected.
	/// </summary>
	void ColorPicker::ValidateSelectedPanel()
	{
		IInspectable selectedItem{ nullptr };

		if (ColorPanelSelector)
		{
			if (!ColorPanelSelector.SelectedItem() &&
				ColorPanelSelector.Items().Size() > 0)
			{
				// As a failsafe, forcefully select the first item
				selectedItem = ColorPanelSelector.Items().GetAt(0);
			}
			else
			{
				selectedItem = ColorPanelSelector.SelectedItem();
			}

			if (auto selectedElement = selectedItem.try_as<UIElement>(); selectedElement &&
				selectedElement.Visibility() == Visibility::Collapsed)
			{
				// Select the first visible item instead
				for (auto item : ColorPanelSelector.Items())
				{
					if (auto element = item.try_as<UIElement>(); element &&
						element.Visibility() == Visibility::Visible)
					{
						selectedItem = item;
						break;
					}
				}
			}

			ColorPanelSelector.SelectedItem(selectedItem);
		}

		return;
	}

	void ColorPicker::OnPanelVisibilityChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		UpdateVisualState(false);
		ValidateSelectedPanel();
	}

	void ColorPicker::OnDependencyPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs args)
	{
		if (auto senderControl = sender.try_as<class_type>())
		{
			/* Note: ColorProperty is defined in the base class and cannot be used here
			 * See the OnColorChanged callback below
			 */
			auto self = winrt::get_self<ColorPicker>(senderControl)->get_strong();
			if (args.Property() == CustomPaletteProperty)
			{
				IColorPalette palette = self->CustomPalette();

				if (palette)
				{
					self->CustomPaletteColumnCount(palette.ColorCount());
					self->CustomPaletteColors().Clear();

					for (int shadeIndex = 0; shadeIndex < palette.ShadeCount(); shadeIndex++)
					{
						for (int colorIndex = 0; colorIndex < palette.ColorCount(); colorIndex++)
						{
							self->CustomPaletteColors().Append(palette.GetColor(colorIndex, shadeIndex));
						}
					}
				}
			}
			else if (args.Property() == IsColorPaletteVisibleProperty())
			{
				self->UpdateVisualState(false);
				self->ValidateSelectedPanel();
			}
		}
	}

	/***************************************************************************************
	 *
	 * Color Update Timer
	 *
	 ***************************************************************************************/

	void ColorPicker::StartDispatcherQueueTimer()
	{
		dispatcherQueueTimer = DispatcherQueue().GetForCurrentThread().CreateTimer();
		dispatcherQueueTimer->Interval(std::chrono::milliseconds(ColorUpdateInterval));
		dispatcherQueueTimer->Tick({ get_weak(), &ColorPicker::DispatcherQueueTimer_Tick });
		dispatcherQueueTimer->Start();
	}

	void ColorPicker::StopDispatcherQueueTimer()
	{
		if (dispatcherQueueTimer)
		{
			dispatcherQueueTimer->Stop();
		}
	}

	void ColorPicker::DispatcherQueueTimer_Tick([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IInspectable const& e)
	{
		if (updatedRgbColor)
		{
			const auto& newColor = updatedRgbColor.value();

			// Clear first to avoid timing issues if it takes longer than the timer interval to set the new color
			updatedRgbColor = std::nullopt;

			// An equality check here is important
			// Without it, OnColorChanged would continuously be invoked and preserveHsvColor overwritten when not wanted
			if (newColor != GetValue(WinUIColorPicker::ColorProperty()).try_as<winrt::Windows::UI::Color>())
			{
				// Disable events here so the color update isn't repeated as other controls in the UI are updated through binding.
				// For example, the Spectrum should be bound to Color, as soon as Color is changed here the Spectrum is updated.
				// Then however, the ColorSpectrum.ColorChanged event would fire which would schedule a new color update --
				// with the same color. This causes several problems:
				//   1. Layout cycle that may crash the app
				//   2. A performance hit recalculating for no reason
				//   3. preserveHsvColor gets overwritten unexpectedly by the ColorChanged handler
				ConnectEvents(false);
				SetValue(WinUIColorPicker::ColorProperty(), winrt::box_value(newColor));
				ConnectEvents(true);
			}
		}
	}

	/***************************************************************************************
	 *
	 * Callbacks
	 *
	 ***************************************************************************************/

	 /// <summary>
	 /// Callback for when the <see cref="Microsoft.UI.Xaml.Controls.ColorPicker.Color"/> dependency property value changes.
	 /// </summary>
	void ColorPicker::OnColorChanged(DependencyObject const& d, DependencyProperty const& e)
	{
		// TODO: Coerce the value if Alpha is disabled, is this handled in the base ColorPicker?
		if ((savedHsvColor) &&
			(d.GetValue(e).try_as<winrt::Windows::UI::Color>() != savedHsvColorRgbEquivalent))
		{
			// The color was updated from an unknown source
			// The RGB and HSV colors are no longer in sync so the HSV color must be cleared
			savedHsvColor = std::nullopt;
			savedHsvColorRgbEquivalent = std::nullopt;
		}

		UpdateColorControlValues();
		UpdateChannelSliderBackgrounds();
	}

	/***************************************************************************************
	 *
	 * Event Handling
	 *
	 ***************************************************************************************/

	 /// <summary>
	 /// Event handler for when the control has finished loaded.
	 /// </summary>
	void ColorPicker::ColorPickerButton_Loaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		// Available but not currently used
	}

	/// <summary>
	/// Event handler for when a color channel slider is loaded.
	/// This will draw an initial background.
	/// </summary>
	void ColorPicker::ChannelSlider_Loaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (auto slider = sender.try_as<Primitives::ColorPickerSlider>())
		{
			UpdateChannelSliderBackground(slider);
		}
	}

	/// <summary>
	/// Event handler to draw checkered backgrounds on-demand as controls are loaded.
	/// </summary>
	winrt::Windows::Foundation::IAsyncAction ColorPicker::CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{
		if (auto border = sender.try_as<Border>())
		{
			co_await ColorPickerRenderingHelpers::UpdateBorderBackgroundWithCheckerAsync(border, CheckerBackgroundColor);
		}
	}

	/// <summary>
	/// Event handler for when the list of custom palette colors is changed.
	/// </summary>
	void ColorPicker::CustomPaletteColors_CollectionChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IVectorChangedEventArgs const& e)
	{
		// Available but not currently used
	}

	/// <summary>
	/// Event handler for when the color panel selector selection changes.
	/// We are setting the value here instead of ElementName binding as a workaround for AoT issues.
	/// (See https://github.com/microsoft/microsoft-ui-xaml/issues/10214)
	/// </summary>
	void ColorPicker::ColorPanelSelector_SelectionChanged(IInspectable const& sender, [[maybe_unused]] SelectionChangedEventArgs const& e)
	{
		if (ContentContainer == nullptr)
		{
			return;
		}

		auto segmented = sender.try_as<Controls::Segmented>();
		if (!segmented)
		{
			return;
		}

		if (auto selectedItem = segmented.SelectedItem().try_as<FrameworkElement>()) {
			ContentContainer.Value(winrt::box_value(selectedItem.Name()));
		}
	}

	/// <summary>
	/// Event handler for when the color spectrum color is changed.
	/// This occurs when the user presses on the spectrum to select a new color.
	/// </summary>
	void ColorPicker::ColorSpectrum_ColorChanged([[maybe_unused]] ColorSpectrum const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
	{
		// It is OK in this case to use the RGB representation
		ScheduleColorUpdate(ColorSpectrumControl.Color());
	}

	/// <summary>
	/// Event handler for when the color spectrum is focused.
	/// This is used only to work around some bugs that cause usability problems.
	/// </summary>
	void ColorPicker::ColorSpectrum_GotFocus([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		winrt::Windows::UI::Color rgbColor = ColorSpectrumControl.Color();

		/* If this control has a color that is currently empty (#00000000),
		 * selecting a new color directly in the spectrum will fail. This is
		 * a bug in the color spectrum. Selecting a new color in the spectrum will
		 * keep zero for all channels (including alpha and the third dimension).
		 *
		 * In practice this means a new color cannot be selected using the spectrum
		 * until both the alpha and third dimension slider are raised above zero.
		 * This is extremely user unfriendly and must be corrected as best as possible.
		 *
		 * In order to work around this, detect when the color spectrum has selected
		 * a new color and then automatically set the alpha and third dimension
		 * channel to maximum. However, the color spectrum has a second bug, the
		 * ColorChanged event is never raised if the color is empty. This prevents
		 * automatically setting the other channels where it normally should be done
		 * (in the ColorChanged event).
		 *
		 * In order to work around this second bug, the GotFocus event is used
		 * to detect when the spectrum is engaged by the user. It's somewhat equivalent
		 * to ColorChanged for this purpose. Then when the GotFocus event is fired
		 * set the alpha and third channel values to maximum. The problem here is that
		 * the GotFocus event does not have access to the new color that was selected
		 * in the spectrum. It is not available due to the afore mentioned bug or due to
		 * timing. This means the best that can be done is to just set a 'neutral'
		 * color such as white.
		 *
		 * There is still a small usability issue with this as it requires two
		 * presses to set a color. That's far better than having to slide up both
		 * sliders though.
		 *
		 *  1. If the color is empty, the first press on the spectrum will set white
		 *     and ignore the pressed color on the spectrum
		 *  2. The second press on the spectrum will be correctly handled.
		 *
		 */

		 // In the future Color.IsEmpty will hopefully be added to UWP
		if (IsColorEmpty(rgbColor))
		{
			/* The following code may be used in the future if ever the selected color is available

			Color newColor = ColorSpectrum.Color;
			HsvColor newHsvColor = newColor.ToHsv();

			switch (GetActiveColorSpectrumThirdDimension())
			{
				case ColorChannel.Channel1:
					{
						newColor = Microsoft.Toolkit.Uwp.Helpers.ColorHelper.FromHsv
						(
							360.0,
							newHsvColor.S,
							newHsvColor.V,
							100.0
						);
						break;
					}

				case ColorChannel.Channel2:
					{
						newColor = Microsoft.Toolkit.Uwp.Helpers.ColorHelper.FromHsv
						(
							newHsvColor.H,
							100.0,
							newHsvColor.V,
							100.0
						);
						break;
					}

				case ColorChannel.Channel3:
					{
						newColor = Microsoft.Toolkit.Uwp.Helpers.ColorHelper.FromHsv
						(
							newHsvColor.H,
							newHsvColor.S,
							100.0,
							100.0
						);
						break;
					}
			}
			*/

			ScheduleColorUpdate(Colors::White());
		}
		else if (rgbColor.A == 0x00)
		{
			// As an additional usability improvement, reset alpha to maximum when the spectrum is used.
			// The color spectrum has no alpha channel and it is much more intuitive to do this for the user
			// especially if the picker was initially set with Colors.Transparent.
			ScheduleColorUpdate(winrt::Windows::UI::ColorHelper::FromArgb(0xFF, rgbColor.R, rgbColor.G, rgbColor.B));
		}
	}

	/// <summary>
	/// Event handler for when the selected color representation changes.
	/// This will convert between RGB and HSV.
	/// </summary>
	void ColorPicker::ColorModeComboBox_SelectionChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SelectionChangedEventArgs const& e)
	{
		if (ColorModeComboBox.SelectedIndex() == 1)
		{
			SetActiveColorRepresentation(ColorRepresentation::Hsva);
		}
		else
		{
			SetActiveColorRepresentation(ColorRepresentation::Rgba);
		}

		UpdateColorControlValues();
		UpdateChannelSliderBackgrounds();
	}

	void ColorPicker::PaletteColorsView_SelectionChanged(IInspectable const& sender, [[maybe_unused]] SelectionChangedEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
		{
			if (auto selectedColor = listViewBase.SelectedValue().try_as<winrt::Windows::UI::Color>())
			{
				Color(*selectedColor);
			}
		}
	}

	/// <summary>
	/// Event handler for when the color previewer requests a new color.
	/// </summary>
	void ColorPicker::ColorPreviewer_ColorChangeRequested([[maybe_unused]] IInspectable const& sender, HsvColor hsvColor)
	{
		winrt::Windows::UI::Color rgbColor = Helpers::ColorHelper::FromHsv(hsvColor.H, hsvColor.S, hsvColor.V, hsvColor.A);

		// Regardless of the active color model, the previewer always uses HSV
		// Therefore, always calculate HSV color here
		// Warning: Always maintain/use HSV information in the saved HSV color
		// This avoids loss of precision and drift caused by continuously converting to/from RGB
		savedHsvColor = hsvColor;
		savedHsvColorRgbEquivalent = rgbColor;

		ScheduleColorUpdate(rgbColor);
	}

	/// <summary>
	/// Event handler for when a key is pressed within the Hex RGB value TextBox.
	/// This is used to trigger a re-evaluation of the color based on the TextBox value.
	/// </summary>
	void ColorPicker::HexInputTextBox_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (e.Key() == winrt::Windows::System::VirtualKey::Enter)
		{
			try
			{
				auto converter = winrt::make<implementation::ColorToHexConverter>();
				Color(converter.ConvertBack(winrt::box_value(sender.as<TextBox>().Text()), winrt::xaml_typename<TextBox>(), nullptr, L"")
					.as<winrt::Windows::UI::Color>());
			}
			catch (...)
			{
				// Reset hex value
				UpdateColorControlValues();
				UpdateChannelSliderBackgrounds();
			}
		}
	}

	/// <summary>
	/// Event handler for when the Hex RGB value TextBox looses focus.
	/// This is used to trigger a re-evaluation of the color based on the TextBox value.
	/// </summary>
	void ColorPicker::HexInputTextBox_LostFocus(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		try
		{
			auto converter = winrt::make<implementation::ColorToHexConverter>();
			Color(converter.ConvertBack(winrt::box_value(sender.as<TextBox>().Text()), winrt::xaml_typename<TextBox>(), nullptr, L"").as<winrt::Windows::UI::Color>());
		}
		catch (...)
		{
			// Reset hex value
			UpdateColorControlValues();
			UpdateChannelSliderBackgrounds();
		}
	}

	/// <summary>
	/// Event handler for when the value within one of the channel NumberBoxes is changed.
	/// </summary>
	void ColorPicker::ChannelNumberBox_ValueChanged(NumberBox const& sender, [[maybe_unused]] NumberBoxValueChangedEventArgs const& args)
	{
		double senderValue = sender.Value();

		if (sender == Channel1NumberBox)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel1, senderValue);
		}
		else if (sender == Channel2NumberBox)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel2, senderValue);
		}
		else if (sender == Channel3NumberBox)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel3, senderValue);
		}
		else if (sender == AlphaChannelNumberBox)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Alpha, senderValue);
		}
	}

	/// <summary>
	/// Event handler for when the value within one of the channel Sliders is changed.
	/// </summary>
	void ColorPicker::ChannelSlider_ValueChanged(IInspectable const& sender, [[maybe_unused]] RangeBaseValueChangedEventArgs const& e)
	{
		double senderValue = (sender.try_as<Slider>()) ? sender.as<Slider>().Value() : std::numeric_limits<double>::quiet_NaN();

		if (sender == Channel1Slider)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel1, senderValue);
		}
		else if (sender == Channel2Slider)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel2, senderValue);
		}
		else if (sender == Channel3Slider)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Channel3, senderValue);
		}
		else if (sender == AlphaChannelSlider)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Alpha, senderValue);
		}
		else if (sender == ColorSpectrumAlphaSlider)
		{
			SetColorChannel(GetActiveColorRepresentation(), ColorChannel::Alpha, senderValue);
		}
		else if (sender == ColorSpectrumThirdDimensionSlider)
		{
			// Regardless of the active color representation, the spectrum is always HSV
			SetColorChannel(ColorRepresentation::Hsva, GetActiveColorSpectrumThirdDimension(), senderValue);
		}
	}
}
