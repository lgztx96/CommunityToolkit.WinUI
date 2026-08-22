#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

#ifdef __INTELLISENSE__
#include <regex>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::com_ptr<RegexState> TextBoxExtensions::GetRegexState(winrt::TextBox const& textbox)
    {
        const auto value = textbox.GetValue(RegexStateProperty());
        if (!value)
        {
            return nullptr;
        }

        return winrt::get_self<RegexState>(value)->get_strong();
    }

    bool TextBoxExtensions::IsEmail(std::wstring_view text)
    {
        static const std::wregex emailRegex(
            L"^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
            L"(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$");
        return std::regex_match(text.begin(), text.end(), emailRegex);
    }

    bool TextBoxExtensions::IsDecimal(std::wstring_view text)
    {
        static const std::wregex decimalRegex(L"^-?\\d+([.,]\\d+)?$");
        return std::regex_match(text.begin(), text.end(), decimalRegex);
    }

    bool TextBoxExtensions::IsNumeric(std::wstring_view text)
    {
        static const std::wregex numericRegex(L"^[+-]?\\d+$");
        return std::regex_match(text.begin(), text.end(), numericRegex);
    }

    bool TextBoxExtensions::IsPhoneNumber(std::wstring_view text)
    {
        static const std::wregex phoneRegex(L"^[+]?\\d{1,4}?[-.\\s]?\\(?\\d{1,3}?\\)?[-.\\s]?\\d{1,4}[-.\\s]?\\d{1,4}[-.\\s]?\\d{1,9}$");
        return std::regex_match(text.begin(), text.end(), phoneRegex);
    }

    bool TextBoxExtensions::IsCharacterString(std::wstring_view text)
    {
        static const std::wregex charRegex(L"^[A-Za-z]+$");
        return std::regex_match(text.begin(), text.end(), charRegex);
    }

    void TextBoxExtensions::TextBoxRegexPropertyOnChange(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto textBox = sender.try_as<winrt::TextBox>();
        if (!textBox)
        {
            return;
        }

        ValidateTextBox(textBox, false);

        auto state = GetRegexState(textBox);
        if (!state)
        {
            state = winrt::make_self<RegexState>();
            textBox.SetValue(RegexStateProperty(), *state);
        }

        if (state->LoadedToken)
        {
            textBox.Loaded(state->LoadedToken);
        }

        if (state->LostFocusToken)
        {
            textBox.LostFocus(state->LostFocusToken);
        }

        if (state->TextChangedToken)
        {
            textBox.TextChanged(state->TextChangedToken);
        }

        state->LoadedToken = textBox.Loaded(&TextBoxExtensions::TextBox_Loaded);
        state->LostFocusToken = textBox.LostFocus(&TextBoxExtensions::TextBox_LostFocus);
        state->TextChangedToken = textBox.TextChanged(&TextBoxExtensions::TextBox_TextChanged);
    }

    void TextBoxExtensions::TextBox_TextChanged(winrt::IInspectable const& sender, [[maybe_unused]] winrt::TextChangedEventArgs const& e)
    {
        const auto textBox = sender.as<winrt::TextBox>();
        auto validationMode = GetValidationMode(textBox);
        ValidateTextBox(textBox, validationMode == winrt::XamlToolkit::WinUI::ValidationMode::Dynamic);
    }

    void TextBoxExtensions::TextBox_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        const auto textBox = sender.as<winrt::TextBox>();
        ValidateTextBox(textBox);
    }

    void TextBoxExtensions::TextBox_LostFocus(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        const auto textBox = sender.as<winrt::TextBox>();
        ValidateTextBox(textBox);
    }

    void TextBoxExtensions::ValidateTextBox(winrt::TextBox const& textBox, bool force)
    {
        auto validationType = GetValidationType(textBox);
        bool regexMatch;
        const auto text = textBox.Text();
        switch (validationType)
        {
        default:
        case winrt::XamlToolkit::WinUI::ValidationType::Custom:
        {
            const auto regex = GetRegex(textBox);
            if (regex.empty() || std::all_of(regex.begin(), regex.end(), [](wchar_t c) { return std::iswspace(c); }))
            {
                return;
            }
            std::wregex pattern(regex.data(), regex.size());
            regexMatch = std::regex_search(text.begin(), text.end(), pattern);
            break;
        }
        case winrt::XamlToolkit::WinUI::ValidationType::Decimal:
            regexMatch = IsDecimal(text);
            break;
        case winrt::XamlToolkit::WinUI::ValidationType::Email:
            regexMatch = IsEmail(text);
            break;
        case winrt::XamlToolkit::WinUI::ValidationType::Number:
            regexMatch = IsNumeric(text);
            break;
        case winrt::XamlToolkit::WinUI::ValidationType::PhoneNumber:
            regexMatch = IsPhoneNumber(text);
            break;
        case winrt::XamlToolkit::WinUI::ValidationType::Characters:
            regexMatch = IsCharacterString(text);
            break;
        }

        if (!regexMatch && force)
        {
            if (!text.empty())
            {
                const auto validationMode = GetValidationMode(textBox);
                if (validationMode == winrt::XamlToolkit::WinUI::ValidationMode::Forced)
                {
                    textBox.Text(L"");
                }
                else if (validationType != winrt::XamlToolkit::WinUI::ValidationType::Email && validationType != winrt::XamlToolkit::WinUI::ValidationType::PhoneNumber)
                {
                    if (validationMode == winrt::XamlToolkit::WinUI::ValidationMode::Dynamic)
                    {
                        int selectionStart = textBox.SelectionStart();
                        if (selectionStart != 0) selectionStart = selectionStart - 1;
                        auto currentText = std::wstring(textBox.Text());
                        currentText.erase(selectionStart, 1);
                        textBox.Text(currentText);
                        textBox.SelectionStart(selectionStart);
                    }
                }
            }
        }

        SetIsValid(textBox, regexMatch);
    }
}
