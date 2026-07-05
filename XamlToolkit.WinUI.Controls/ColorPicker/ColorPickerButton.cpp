#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorPickerButton.h"
#if __has_include("ColorPickerButton.g.cpp")
#include "ColorPickerButton.g.cpp"
#endif
#include "ColorPickerRenderingHelpers.h"
#include "ColorPicker.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerButton::ColorPickerStyleProperty =
		winrt::DependencyProperty::Register(
			L"ColorPickerStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerButton::FlyoutPresenterStyleProperty =
		winrt::DependencyProperty::Register(
			L"FlyoutPresenterStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPickerButton::SelectedColorProperty =
		winrt::DependencyProperty::Register(
			L"SelectedColor",
			winrt::xaml_typename<winrt::Color>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &ColorPickerButton::SelectedColorChanged));

    ColorPickerButton::ColorPickerButton() : ColorPicker(nullptr)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

        // Workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/3502
        DefaultStyleResourceUri(winrt::Uri(L"ms-appx:///XamlToolkit.WinUI.Controls/Themes/Generic.xaml"));
    }

    void ColorPickerButton::OnApplyTemplate()
    {
        if (ColorPicker())
        {
            _colorChangedRevoker.revoke();
        }

        base_type::OnApplyTemplate();

        if (auto style = ColorPickerStyle())
        {
            const auto colorPicker = winrt::make<implementation::ColorPicker>();
            colorPicker.Style(style);
            ColorPicker = colorPicker;
        }
        else
        {
            ColorPicker = winrt::make<implementation::ColorPicker>();
        }

        ColorPicker().Color(SelectedColor());
        _colorChangedRevoker = ColorPicker().ColorChanged(winrt::auto_revoke, { this, &ColorPickerButton::ColorPicker_ColorChanged });

        if (!Flyout())
        {
            winrt::Flyout flyout;
            flyout.Placement(winrt::FlyoutPlacementMode::BottomEdgeAlignedLeft);
			if (auto flyoutPresenterStyle = FlyoutPresenterStyle())
			{
				flyout.FlyoutPresenterStyle(flyoutPresenterStyle);
			}
			flyout.Content(ColorPicker());
            Flyout(flyout);
        }

        if (CheckeredBackgroundBorder)
        {
            _checkeredLoadedRevoker.revoke();
        }

        CheckeredBackgroundBorder = GetTemplateChild(L"CheckeredBackgroundBorder").try_as<winrt::Border>();

        if (CheckeredBackgroundBorder)
        {
            _checkeredLoadedRevoker = CheckeredBackgroundBorder.Loaded(winrt::auto_revoke, { this, &ColorPickerButton::CheckeredBackgroundBorder_Loaded });
        }
    }

    void ColorPickerButton::SelectedColorChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (const auto instance = d.try_as<class_type>())
        {
            if (const auto picker = instance.ColorPicker()) 
            {
                picker.Color(instance.SelectedColor());
            }
        }
    }

    void ColorPickerButton::ColorPicker_ColorChanged([[maybe_unused]] winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
    {
        SelectedColor(args.NewColor());
    }

    winrt::fire_and_forget ColorPickerButton::CheckeredBackgroundBorder_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (const auto border = sender.try_as<winrt::Border>())
        {
            co_await ColorPickerRenderingHelpers::UpdateBorderBackgroundWithCheckerAsync(border, ColorPicker().CheckerBackgroundColor()); // TODO: Check initialization
        }
    }
}
