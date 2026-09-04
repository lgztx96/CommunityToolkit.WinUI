#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
#pragma region Mask - Dependency Properties Registration

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::MaskProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Mask",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(L""), &TextBoxExtensions::InitTextBoxMask));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::MaskPlaceholderProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"MaskPlaceholder",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(L"_"), &TextBoxExtensions::InitTextBoxMask));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::CustomMaskProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"CustomMask",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(L""), &TextBoxExtensions::InitTextBoxMask));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::MaskStateProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"MaskState",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(nullptr));

#pragma endregion

#pragma region Mask - Getters and Setters

    winrt::hstring TextBoxExtensions::GetMask(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::hstring>(obj.GetValue(MaskProperty()));
    }

    void TextBoxExtensions::SetMask(winrt::TextBox const& obj, winrt::hstring const& value)
    {
        obj.SetValue(MaskProperty(), winrt::box_value(value));
    }

    winrt::hstring TextBoxExtensions::GetMaskPlaceholder(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::hstring>(obj.GetValue(MaskPlaceholderProperty()));
    }

    void TextBoxExtensions::SetMaskPlaceholder(winrt::TextBox const& obj, winrt::hstring const& value)
    {
        obj.SetValue(MaskPlaceholderProperty(), winrt::box_value(value));
    }

    winrt::hstring TextBoxExtensions::GetCustomMask(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::hstring>(obj.GetValue(CustomMaskProperty()));
    }

    void TextBoxExtensions::SetCustomMask(winrt::TextBox const& obj, winrt::hstring const& value)
    {
        obj.SetValue(CustomMaskProperty(), winrt::box_value(value));
    }

#pragma endregion
}
