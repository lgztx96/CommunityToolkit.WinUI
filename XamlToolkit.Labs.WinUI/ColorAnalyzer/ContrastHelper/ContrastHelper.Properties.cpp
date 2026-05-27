#include "pch.h"
#include "winrt_module_imports.h"
#include "ContrastHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::OpponentProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"Opponent",
            winrt::xaml_typename<Windows::UI::Color>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(Windows::UI::Colors::Transparent()), &ContrastHelper::OnOpponentChanged });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::MinRatioProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"MinRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(21.0), &ContrastHelper::OnMinRatioChanged });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::ContrastRatioProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"ContrastRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::OriginalColorProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"OriginalColor",
            winrt::xaml_typename<Windows::UI::Color>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(Windows::UI::Colors::Transparent()) });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::OriginalContrastRatioProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"OriginalContrastRatio",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0) });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::CallbackObjectProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"CallbackObject",
            winrt::xaml_typename<Microsoft::UI::Xaml::DependencyObject>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastHelper::CallbackProperty =
        Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
            L"Callback",
            winrt::xaml_typename<int64_t>(),
            winrt::xaml_typename<class_type>(),
            Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value<int64_t>(0) });

    Windows::UI::Color ContrastHelper::GetOpponent(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<Windows::UI::Color>(obj.GetValue(OpponentProperty));
    }

    void ContrastHelper::SetOpponent(Microsoft::UI::Xaml::DependencyObject const& obj, Windows::UI::Color const& value)
    {
        obj.SetValue(OpponentProperty, winrt::box_value(value));
    }

    double ContrastHelper::GetMinRatio(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(MinRatioProperty));
    }

    void ContrastHelper::SetMinRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value)
    {
        obj.SetValue(MinRatioProperty, winrt::box_value(value));
    }

    double ContrastHelper::GetContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(ContrastRatioProperty));
    }

    void ContrastHelper::SetContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value)
    {
        obj.SetValue(ContrastRatioProperty, winrt::box_value(value));
    }

    Windows::UI::Color ContrastHelper::GetOriginalColor(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<Windows::UI::Color>(obj.GetValue(OriginalColorProperty));
    }

    void ContrastHelper::SetOriginalColor(Microsoft::UI::Xaml::DependencyObject const& obj, Windows::UI::Color const& value)
    {
        obj.SetValue(OriginalColorProperty, winrt::box_value(value));
    }

    double ContrastHelper::GetOriginalContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<double>(obj.GetValue(OriginalContrastRatioProperty));
    }

    void ContrastHelper::SetOriginalContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value)
    {
        obj.SetValue(OriginalContrastRatioProperty, winrt::box_value(value));
    }

    Microsoft::UI::Xaml::DependencyObject ContrastHelper::GetCallbackObject(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return obj.GetValue(CallbackObjectProperty).try_as<Microsoft::UI::Xaml::DependencyObject>();
    }

    void ContrastHelper::SetCallbackObject(Microsoft::UI::Xaml::DependencyObject const& obj, Microsoft::UI::Xaml::DependencyObject const& value)
    {
        obj.SetValue(CallbackObjectProperty, value);
    }

    int64_t ContrastHelper::GetCallback(Microsoft::UI::Xaml::DependencyObject const& obj)
    {
        return winrt::unbox_value<int64_t>(obj.GetValue(CallbackProperty));
    }

    void ContrastHelper::SetCallback(Microsoft::UI::Xaml::DependencyObject const& obj, int64_t value)
    {
        obj.SetValue(CallbackProperty, winrt::box_value(value));
    }
}
