#pragma once

#include "TextBoxExtensions.g.h"

#ifdef __INTELLISENSE__
#include <string>
#include <unordered_map>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.UI.Input.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Windows::UI::Input;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct TextBoxExtensions
    {
#pragma region Mask
        static const wil::single_threaded_property<winrt::DependencyProperty> MaskProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> MaskPlaceholderProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> CustomMaskProperty;

        static winrt::hstring GetMask(winrt::TextBox const& obj);

        static void SetMask(winrt::TextBox const& obj, winrt::hstring const& value);

        static winrt::hstring GetMaskPlaceholder(winrt::TextBox const& obj);

        static void SetMaskPlaceholder(winrt::TextBox const& obj, winrt::hstring const& value);

        static winrt::hstring GetCustomMask(winrt::TextBox const& obj);

        static void SetCustomMask(winrt::TextBox const& obj, winrt::hstring const& value);

    private:
        static constexpr wchar_t DefaultPlaceHolder = L'_';
        static constexpr wchar_t EscapeChar = L'\\';
        static constexpr std::pair<wchar_t, std::wstring_view> AlphaCharacterRepresentation = { L'a', L"[A-Za-z]" };
        static constexpr std::pair<wchar_t, std::wstring_view> NumericCharacterRepresentation = { L'9', L"[0-9]" };
        static constexpr std::pair<wchar_t, std::wstring_view> AlphaNumericRepresentation = { L'*', L"[A-Za-z0-9]" };

        static void InitTextBoxMask(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void Textbox_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void Textbox_GotFocus_Mask(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static winrt::fire_and_forget Textbox_Paste(winrt::IInspectable sender, winrt::TextControlPasteEventArgs const& e);

        static void Textbox_SelectionChanged(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void Textbox_TextChanging(winrt::TextBox const& sender, winrt::TextBoxTextChangingEventArgs const& args);

        static void SetTextBoxValue(
            std::wstring_view newValue,
            winrt::TextBox const& textbox,
            std::wstring_view mask,
            const std::vector<int>& escapedChars,
            const std::unordered_map<wchar_t, std::wstring>& representationDictionary,
            wchar_t placeholder,
            int oldSelectionStart);

        static int GetSelectionStart(
            std::wstring_view mask,
            const std::vector<int>& escapedChars,
            int selectionIndex,
            const std::unordered_map<wchar_t, std::wstring>& representationDictionary);

#pragma endregion

#pragma region Regex
    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> RegexProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> IsValidProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ValidationModeProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ValidationTypeProperty;

        static winrt::hstring GetRegex(winrt::TextBox const& obj);

        static void SetRegex(winrt::TextBox const& obj, winrt::hstring const& value);

        static bool GetIsValid(winrt::TextBox const& obj);

        static void SetIsValid(winrt::TextBox const& obj, bool value);

        static winrt::XamlToolkit::WinUI::ValidationMode GetValidationMode(winrt::TextBox const& obj);

        static void SetValidationMode(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::ValidationMode const& value);

        static winrt::XamlToolkit::WinUI::ValidationType GetValidationType(winrt::TextBox const& obj);

        static void SetValidationType(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::ValidationType const& value);

    private:
        static void TextBoxRegexPropertyOnChange(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

        static void TextBox_TextChanged_Regex(winrt::IInspectable const& sender, winrt::TextChangedEventArgs const& e);

        static void TextBox_Loaded_Regex(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void TextBox_LostFocus_Regex(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void ValidateTextBox(winrt::TextBox const& textBox, bool force = true);

        static bool IsEmail(std::wstring_view text);

        static bool IsDecimal(std::wstring_view text);

        static bool IsNumeric(std::wstring_view text);

        static bool IsPhoneNumber(std::wstring_view text);

        static bool IsCharacterString(std::wstring_view text);
#pragma endregion

#pragma region SurfaceDial
    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> SurfaceDialOptionsProperty;

        static winrt::XamlToolkit::WinUI::SurfaceDialOptions GetSurfaceDialOptions(winrt::TextBox const& obj);

        static void SetSurfaceDialOptions(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::SurfaceDialOptions const& value);

        static bool IsSurfaceDialOptionsSupported();

        static winrt::RadialController Controller();

        static void Controller(winrt::RadialController const& value);

    private:
        static winrt::RadialController _controller;
        static winrt::RadialControllerMenuItem _stepTextMenuItem;
        static winrt::weak_ref<winrt::TextBox> _textBox;
        static winrt::event_token _gotFocusToken;
        static winrt::event_token _lostFocusToken;
        static winrt::event_token _rotationToken;
        static winrt::event_token _buttonToken;

        static void OnSurfaceDialOptionsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void TextBox_GotFocus_SurfaceDial(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void TextBox_LostFocus_SurfaceDial(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void Controller_RotationChanged(winrt::RadialController const& sender, winrt::RadialControllerRotationChangedEventArgs const& args);

        static void Controller_ButtonClicked(winrt::RadialController const& sender, winrt::RadialControllerButtonClickedEventArgs const& args);
#pragma endregion
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct TextBoxExtensions : TextBoxExtensionsT<TextBoxExtensions, implementation::TextBoxExtensions>
    {
    };
}
