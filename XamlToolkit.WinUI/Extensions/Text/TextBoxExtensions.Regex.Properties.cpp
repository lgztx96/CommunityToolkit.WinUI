#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
#pragma region Regex - Dependency Properties Registration

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::RegexProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Regex",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(L""), &TextBoxExtensions::TextBoxRegexPropertyOnChange));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::IsValidProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"IsValid",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(false)));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::ValidationModeProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ValidationMode",
            winrt::xaml_typename<XamlToolkit::WinUI::ValidationMode>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::ValidationMode::Normal), &TextBoxExtensions::TextBoxRegexPropertyOnChange));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::ValidationTypeProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ValidationType",
            winrt::xaml_typename<XamlToolkit::WinUI::ValidationType>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::ValidationType::Custom), &TextBoxExtensions::TextBoxRegexPropertyOnChange));

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::RegexStateProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"RegexState",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(nullptr));

#pragma endregion

#pragma region Regex - Getters and Setters

    winrt::hstring TextBoxExtensions::GetRegex(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::hstring>(obj.GetValue(RegexProperty()));
    }

    void TextBoxExtensions::SetRegex(winrt::TextBox const& obj, winrt::hstring const& value)
    {
        obj.SetValue(RegexProperty(), winrt::box_value(value));
    }

    bool TextBoxExtensions::GetIsValid(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<bool>(obj.GetValue(IsValidProperty()));
    }

    void TextBoxExtensions::SetIsValid(winrt::TextBox const& obj, bool value)
    {
        obj.SetValue(IsValidProperty(), winrt::box_value(value));
    }

    winrt::XamlToolkit::WinUI::ValidationMode TextBoxExtensions::GetValidationMode(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::XamlToolkit::WinUI::ValidationMode>(obj.GetValue(ValidationModeProperty()));
    }

    void TextBoxExtensions::SetValidationMode(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::ValidationMode const& value)
    {
        obj.SetValue(ValidationModeProperty(), winrt::box_value(value));
    }

    winrt::XamlToolkit::WinUI::ValidationType TextBoxExtensions::GetValidationType(winrt::TextBox const& obj)
    {
        return winrt::unbox_value<winrt::XamlToolkit::WinUI::ValidationType>(obj.GetValue(ValidationTypeProperty()));
    }

    void TextBoxExtensions::SetValidationType(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::ValidationType const& value)
    {
        obj.SetValue(ValidationTypeProperty(), winrt::box_value(value));
    }

#pragma endregion
}
