#include "pch.h"
#include "winrt_module_imports.h"
#include "ContrastHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::OpponentProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Opponent",
            winrt::xaml_typename<winrt::Color>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Colors::Transparent()), &ContrastHelper::OnOpponentChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::MinRatioProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"MinRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value(21.0), &ContrastHelper::OnMinRatioChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::ContrastRatioProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ContrastRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::OriginalColorProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"OriginalColor",
            winrt::xaml_typename<Windows::UI::Color>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value(Windows::UI::Colors::Transparent()) });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::OriginalContrastRatioProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"OriginalContrastRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::CallbackObjectProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"CallbackObject",
            winrt::xaml_typename<winrt::DependencyObject>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> ContrastHelper::CallbackProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Callback",
            winrt::xaml_typename<int64_t>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ContrastHelper>(),
            winrt::PropertyMetadata{ winrt::box_value<int64_t>(0) });

    winrt::Color ContrastHelper::GetOpponent(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<winrt::Color>(obj.GetValue(OpponentProperty()));
    }

    void ContrastHelper::SetOpponent(winrt::DependencyObject const& obj, winrt::Color const& value)
    {
        obj.SetValue(OpponentProperty(), winrt::box_value(value));
    }

    double ContrastHelper::GetMinRatio(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(MinRatioProperty()));
    }

    void ContrastHelper::SetMinRatio(winrt::DependencyObject const& obj, double value)
    {
        obj.SetValue(MinRatioProperty(), winrt::box_value(value));
    }

    double ContrastHelper::GetContrastRatio(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(ContrastRatioProperty()));
    }

    void ContrastHelper::SetContrastRatio(winrt::DependencyObject const& obj, double value)
    {
        obj.SetValue(ContrastRatioProperty(), winrt::box_value(value));
    }

    winrt::Color ContrastHelper::GetOriginalColor(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<winrt::Color>(obj.GetValue(OriginalColorProperty()));
    }

    void ContrastHelper::SetOriginalColor(winrt::DependencyObject const& obj, winrt::Color const& value)
    {
        obj.SetValue(OriginalColorProperty(), winrt::box_value(value));
    }

    double ContrastHelper::GetOriginalContrastRatio(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(OriginalContrastRatioProperty()));
    }

    void ContrastHelper::SetOriginalContrastRatio(winrt::DependencyObject const& obj, double value)
    {
        obj.SetValue(OriginalContrastRatioProperty(), winrt::box_value(value));
    }

    winrt::DependencyObject ContrastHelper::GetCallbackObject(winrt::DependencyObject const& obj)
    {
        return obj.GetValue(CallbackObjectProperty()).try_as<winrt::DependencyObject>();
    }

    void ContrastHelper::SetCallbackObject(winrt::DependencyObject const& obj, winrt::DependencyObject const& value)
    {
        obj.SetValue(CallbackObjectProperty(), value);
    }

    int64_t ContrastHelper::GetCallback(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<int64_t>(obj.GetValue(CallbackProperty()));
    }

    void ContrastHelper::SetCallback(winrt::DependencyObject const& obj, int64_t value)
    {
        obj.SetValue(CallbackProperty(), winrt::box_value(value));
    }
}
