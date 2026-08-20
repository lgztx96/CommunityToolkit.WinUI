#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "ColorPickerSlider.h"
#if __has_include("Primitives.ColorPickerSlider.g.cpp")
#include "Primitives.ColorPickerSlider.g.cpp"
#endif
#include "ColorPickerRenderingHelpers.h"

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::ColorProperty =
		winrt::DependencyProperty::Register(
			L"Color",
			winrt::xaml_typename<winrt::Windows::UI::Color>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(winrt::Microsoft::UI::Colors::White()), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::ColorChannelProperty =
		winrt::DependencyProperty::Register(
			L"ColorChannel",
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::ColorChannel>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(ColorChannel::Channel1), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::ColorRepresentationProperty =
		winrt::DependencyProperty::Register(
			L"ColorRepresentation",
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::ColorRepresentation>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(ColorRepresentation::Rgba), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::DefaultForegroundProperty =
		winrt::DependencyProperty::Register(
			L"DefaultForeground",
			winrt::xaml_typename<winrt::Brush>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::HsvColorProperty =
		winrt::DependencyProperty::Register(
			L"HsvColor",
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::HsvColor>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHsv(winrt::Microsoft::UI::Colors::White())), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::IsAlphaMaxForcedProperty =
		winrt::DependencyProperty::Register(
			L"IsAlphaMaxForced",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::IsAutoUpdatingEnabledProperty =
		winrt::DependencyProperty::Register(
			L"IsAutoUpdatingEnabled",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerSlider::IsSaturationValueMaxForcedProperty =
		winrt::DependencyProperty::Register(
			L"IsSaturationValueMaxForced",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));

	/***************************************************************************************
	 *
	 * Constructor/Destructor
	 *
	 ***************************************************************************************/

	 /// <summary>
	 /// Initializes a new instance of the <see cref="ColorPickerSlider"/> class.
	 /// </summary>
	ColorPickerSlider::ColorPickerSlider()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	/***************************************************************************************
	 *
	 * Methods
	 *
	 ***************************************************************************************/

	 /// <summary>
	 /// Update the slider's Foreground and Background brushes based on the current slider state and color.
	 /// </summary>
	 /// <remarks>
	 /// Manually refreshes the background gradient of the slider.
	 /// This is callable separately for performance reasons.
	 /// </remarks>
	void ColorPickerSlider::UpdateColors()
	{
		auto hsvColor = HsvColor();

		// Calculate and set the background
		UpdateBackground(hsvColor);

		// Calculate and set the foreground ensuring contrast with the background
		auto rgbColor = winrt::XamlToolkit::WinUI::Helpers::ColorHelper::FromHsv(hsvColor.H, hsvColor.S, hsvColor.V, hsvColor.A);
		winrt::Color selectedRgbColor;
		const double sliderPercent = Value() / (Maximum() - Minimum());

		if (ColorRepresentation() == ColorRepresentation::Hsva)
		{
			if (IsAlphaMaxForced() &&
				ColorChannel() != ColorChannel::Alpha)
			{
				hsvColor = winrt::XamlToolkit::WinUI::HsvColor
				{
					.H = hsvColor.H,
					.S = hsvColor.S,
					.V = hsvColor.V,
					.A = 1.0
				};
			}

			switch (ColorChannel())
			{
			case ColorChannel::Channel1:
			{
				auto channelValue = std::clamp(sliderPercent * 360.0, 0.0, 360.0);

				hsvColor = winrt::XamlToolkit::WinUI::HsvColor
				{
					.H = channelValue,
					.S = IsSaturationValueMaxForced() ? 1.0 : hsvColor.S,
					.V = IsSaturationValueMaxForced() ? 1.0 : hsvColor.V,
					.A = hsvColor.A
				};
				break;
			}

			case ColorChannel::Channel2:
			{
				auto channelValue = std::clamp(sliderPercent * 1.0, 0.0, 1.0);

				hsvColor = winrt::XamlToolkit::WinUI::HsvColor
				{
					.H = hsvColor.H,
					.S = channelValue,
					.V = IsSaturationValueMaxForced() ? 1.0 : hsvColor.V,
					.A = hsvColor.A
				};
				break;
			}

			case ColorChannel::Channel3:
			{
				auto channelValue = std::clamp(sliderPercent * 1.0, 0.0, 1.0);

				hsvColor = winrt::XamlToolkit::WinUI::HsvColor
				{
					.H = hsvColor.H,
					.S = IsSaturationValueMaxForced() ? 1.0 : hsvColor.S,
					.V = channelValue,
					.A = hsvColor.A
				};
				break;
			}
			}

			selectedRgbColor = winrt::XamlToolkit::WinUI::Helpers::ColorHelper::FromHsv(
				hsvColor.H,
				hsvColor.S,
				hsvColor.V,
				hsvColor.A);
		}
		else
		{
			if (IsAlphaMaxForced() &&
				ColorChannel() != ColorChannel::Alpha)
			{
				rgbColor = winrt::Windows::UI::Color
				{
					.A = 255,
					.R = rgbColor.R,
					.G = rgbColor.G,
					.B = rgbColor.B,
				};
			}

			uint8_t channelValue = static_cast<uint8_t>(std::clamp(sliderPercent * 255.0, 0.0, 255.0));

			switch (ColorChannel())
			{
			case ColorChannel::Channel1:
				rgbColor = winrt::Windows::UI::Color
				{
					.A = rgbColor.A,
					.R = channelValue,
					.G = rgbColor.G,
					.B = rgbColor.B,

				};
				break;
			case ColorChannel::Channel2:
				rgbColor = winrt::Windows::UI::Color
				{
					.A = rgbColor.A,
					.R = rgbColor.R,
					.G = channelValue,
					.B = rgbColor.B,
				};
				break;
			case ColorChannel::Channel3:
				rgbColor = winrt::Windows::UI::Color
				{
					.A = rgbColor.A,
					.R = rgbColor.R,
					.G = rgbColor.G,
					.B = channelValue,
				};
				break;
			}

			selectedRgbColor = rgbColor;
		}
	}

	/// <summary>
	/// Generates a new background image for the color channel slider and applies it.
	/// </summary>
	winrt::fire_and_forget ColorPickerSlider::UpdateBackground(winrt::XamlToolkit::WinUI::HsvColor color)
	{
		/* Updates may be requested when sliders are not in the visual tree.
		 * For first-time load this is handled by the Loaded event.
		 * However, after that problems may arise, consider the following case:
		 *
		 *   (1) Backgrounds are drawn normally the first time on Loaded.
		 *       Actual height/width are available.
		 *   (2) The palette tab is selected which has no sliders
		 *   (3) The picker flyout is closed
		 *   (4) Externally the color is changed
		 *       The color change will trigger slider background updates but
		 *       with the flyout closed, actual height/width are zero.
		 *       No zero size bitmap can be generated.
		 *   (5) The picker flyout is re-opened by the user and the default
		 *       last-opened tab will be viewed: palette.
		 *       No loaded events will be fired for sliders. The color change
		 *       event was already handled in (4). The sliders will never
		 *       be updated.
		 *
		 * In this case the sliders become out of sync with the Color because there is no way
		 * to tell when they actually come into view. To work around this, force a re-render of
		 * the background with the last size of the slider. This last size will be when it was
		 * last loaded or updated.
		 *
		 * In the future additional consideration may be required for SizeChanged of the control.
		 * This work-around will also cause issues if display scaling changes in the special
		 * case where cached sizes are required.
		 */
		auto strongThis = get_strong();

		auto width = static_cast<int32_t>(ActualWidth());
		auto height = static_cast<int32_t>(ActualHeight());

		if (width == 0 || height == 0)
		{
			// Attempt to use the last size if it was available
			if (cachedSize.Width != 0 && cachedSize.Height != 0)
			{
				width = static_cast<int32_t>(cachedSize.Width);
				height = static_cast<int32_t>(cachedSize.Height);
			}
		}
		else
		{
			cachedSize = winrt::Size(static_cast<float>(width), static_cast<float>(height));
		}

		const auto& bitmap = co_await ColorPickerRenderingHelpers::CreateChannelBitmapAsync(
			width,
			height,
			Orientation(),
			ColorRepresentation(),
			ColorChannel(),
			color,
			CheckerBackgroundColor,
			IsAlphaMaxForced(),
			IsSaturationValueMaxForced());

		if (bitmap)
		{
			Background(ColorPickerRenderingHelpers::BitmapToBrush(bitmap, width, height));
		}
	}

	/// <summary>
	/// Measures the size in layout required for child elements and determines a size for the
	/// winrt::FrameworkElement-derived class.
	/// </summary>
	/// <remarks>
	///
	/// Slider has some critical bugs:
	///
	///  * https://github.com/microsoft/microsoft-ui-xaml/issues/477
	///  * https://social.msdn.microsoft.com/Forums/sqlserver/en-US/0d3a2e64-d192-4250-b583-508a02bd75e1/uwp-bug-crash-layoutcycleexception-because-of-slider-under-certain-circumstances?forum=wpdevelop
	///
	/// </remarks>
	/// <param name="availableSize">The available size that this element can give to child elements.
	/// Infinity can be specified as a value to indicate that the element will size to whatever content
	/// is available.</param>
	/// <returns>The size that this element determines it needs during layout,
	/// based on its calculations of child element sizes.</returns>
	winrt::Size ColorPickerSlider::MeasureOverride(winrt::Size availableSize)
	{
		if (oldSize != availableSize)
		{
			measuredSize = base_type::MeasureOverride(availableSize);
			oldSize = availableSize;
		}

		return measuredSize;
	}

	void ColorPickerSlider::OnDependencyPropertyChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto slider = sender.try_as<class_type>())
		{
			auto self = winrt::get_self<ColorPickerSlider>(slider);
			if (args.Property() == ColorProperty())
			{
				// Sync with HSV (which is primary)
				self->HsvColor(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHsv(self->Color()));
			}

			if (self->IsAutoUpdatingEnabled())
			{
				self->UpdateColors();
			}
		}
	}
}
