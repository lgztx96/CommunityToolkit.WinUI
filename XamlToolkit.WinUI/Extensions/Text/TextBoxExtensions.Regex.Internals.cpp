#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

#ifdef __INTELLISENSE__
#include <regex>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    // Per-textbox regex state storage
    struct RegexState
    {
        winrt::event_token LoadedToken;
        winrt::event_token LostFocusToken;
        winrt::event_token TextChangedToken;
    };

    struct WeakRegexTextBoxHash
    {
        size_t operator()(winrt::weak_ref<winrt::TextBox> const& wref) const noexcept
        {
            if (auto ref = wref.get())
            {
                return std::hash<void*>{}(winrt::get_abi(ref));
            }

            return 0;
        }
    };

    static std::unordered_map<winrt::weak_ref<winrt::TextBox>, std::unique_ptr<RegexState>, WeakRegexTextBoxHash> _regexStates;

    static RegexState* GetOrCreateRegexState(winrt::TextBox const& textbox)
    {
        if (auto it = _regexStates.find(textbox); it != _regexStates.end())
        {
            // Verify the weak_ref is still alive; clean up stale entry if not
            if (auto ref = it->first.get())
            {
                return it->second.get();
            }
            _regexStates.erase(it);
        }
        auto state = std::make_unique<RegexState>();
        auto ptr = state.get();
        _regexStates.emplace(textbox, std::move(state));
        return ptr;
    }

    // Validation helper patterns matching CommunityToolkit.Common behavior
    bool TextBoxExtensions::IsEmail(std::wstring_view text)
    {
        // Standard email regex pattern
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

        auto state = GetOrCreateRegexState(textBox);

        // Remove old handlers
        textBox.Loaded(state->LoadedToken);
        textBox.LostFocus(state->LostFocusToken);
        textBox.TextChanged(state->TextChangedToken);

        // Add new handlers
        state->LoadedToken = textBox.Loaded(&TextBoxExtensions::TextBox_Loaded_Regex);
        state->LostFocusToken = textBox.LostFocus(&TextBoxExtensions::TextBox_LostFocus_Regex);
        state->TextChangedToken = textBox.TextChanged(&TextBoxExtensions::TextBox_TextChanged_Regex);
    }

    void TextBoxExtensions::TextBox_TextChanged_Regex(winrt::IInspectable const& sender, [[maybe_unused]] winrt::TextChangedEventArgs const& e)
    {
        auto textBox = sender.try_as<winrt::TextBox>();
        if (!textBox) return;

        auto validationMode = GetValidationMode(textBox);
        ValidateTextBox(textBox, validationMode == winrt::XamlToolkit::WinUI::ValidationMode::Dynamic);
    }

    void TextBoxExtensions::TextBox_Loaded_Regex(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        auto textBox = sender.try_as<winrt::TextBox>();
        if (!textBox) return;

        ValidateTextBox(textBox);
    }

    void TextBoxExtensions::TextBox_LostFocus_Regex(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        auto textBox = sender.try_as<winrt::TextBox>();
        if (!textBox) return;

        ValidateTextBox(textBox);
    }

    void TextBoxExtensions::ValidateTextBox(winrt::TextBox const& textBox, bool force)
    {
        auto validationType = GetValidationType(textBox);
        bool regexMatch;
        auto text = textBox.Text();
        switch (validationType)
        {
        default:
        case winrt::XamlToolkit::WinUI::ValidationType::Custom:
        {
            auto regexStr = GetRegex(textBox);
            std::wstring regex(regexStr);
            if (regex.empty() || std::all_of(regex.begin(), regex.end(), [](wchar_t c) { return std::iswspace(c); }))
            {
                return;
            }
            std::wregex pattern(regex);
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
                auto validationMode = GetValidationMode(textBox);
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
