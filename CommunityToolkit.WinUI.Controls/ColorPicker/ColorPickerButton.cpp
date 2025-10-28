#include "pch.h"
#include "ColorPickerButton.h"
#if __has_include("ColorPickerButton.g.cpp")
#include "ColorPickerButton.g.cpp"
#endif
#include "ColorPickerRenderingHelpers.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    ColorPickerButton::ColorPickerButton() : ColorPicker(nullptr)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

        // Workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/3502
        DefaultStyleResourceUri(Uri(L"ms-appx:///CommunityToolkit.WinUI.Controls/Themes/Generic.xaml"));
    }

    void ColorPickerButton::OnApplyTemplate()
    {
        if (ColorPicker())
        {
            _colorChangedRevoker.revoke();
        }

        base_type::OnApplyTemplate();

        if (ColorPickerStyle())
        {
            auto colorPicker = winrt::make<implementation::ColorPicker>();
            colorPicker.Style(ColorPickerStyle());
            ColorPicker = colorPicker;
        }
        else
        {
            ColorPicker = winrt::make<implementation::ColorPicker>();
        }

        ColorPicker().Color(SelectedColor());
        _colorChangedRevoker = ColorPicker().ColorChanged(winrt::auto_revoke, { this, & ColorPickerButton::ColorPicker_ColorChanged });

        if (!Flyout())
        {
            Microsoft::UI::Xaml::Controls::Flyout flyout;
            flyout.Placement(FlyoutPlacementMode::BottomEdgeAlignedLeft);
			flyout.FlyoutPresenterStyle(FlyoutPresenterStyle());
			flyout.Content(ColorPicker());
            Flyout(flyout);
        }

        if (CheckeredBackgroundBorder)
        {
            _checkeredLoadedRevoker.revoke();
        }

        CheckeredBackgroundBorder = GetTemplateChild(L"CheckeredBackgroundBorder").try_as<Border>();

        if (CheckeredBackgroundBorder)
        {
            _checkeredLoadedRevoker = CheckeredBackgroundBorder.Loaded(winrt::auto_revoke, { this, & ColorPickerButton::CheckeredBackgroundBorder_Loaded });
        }
    }

    void ColorPickerButton::SelectedColorChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        if (auto instance = d.try_as<class_type>(); instance && instance.ColorPicker())
        {
            instance.ColorPicker().Color(instance.SelectedColor());
        }
    }

    void ColorPickerButton::ColorPicker_ColorChanged([[maybe_unused]] winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
    {
        SelectedColor(args.NewColor());
    }

    winrt::Windows::Foundation::IAsyncAction ColorPickerButton::CheckeredBackgroundBorder_Loaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        if (auto border = sender.try_as<Border>())
        {
            co_await ColorPickerRenderingHelpers::UpdateBorderBackgroundWithCheckerAsync(border, ColorPicker().CheckerBackgroundColor()); // TODO: Check initialization
        }
    }
}
