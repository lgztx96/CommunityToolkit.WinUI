#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

#ifdef __INTELLISENSE__
#include <regex>
#include <ranges>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    static bool IsNullOrWhiteSpace(std::wstring_view s)
    {
        return s.empty() || std::all_of(s.begin(), s.end(), [](wchar_t c) { return std::iswspace(c); });
    }

    static constexpr auto Split(std::wstring_view view, std::wstring_view delim)
    {
        return std::views::split(view, delim) 
            | std::views::transform([](auto&& range) { return std::wstring_view(range); }) 
            | std::ranges::to<std::vector>();
    }

    winrt::com_ptr<MaskState> TextBoxExtensions::GetMaskState(winrt::TextBox const& textbox)
    {
        const auto value = textbox.GetValue(MaskStateProperty());
        if (!value)
        {
            return nullptr;
        }
        return winrt::get_self<MaskState>(value)->get_strong();
    }

    void TextBoxExtensions::InitTextBoxMask(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // TODO: We should think about if we want to adopt this pattern, but have a DiagnosticSupressor to prevent warning for non-braced if that we should set in .editorconfig.
        const auto textbox = d.try_as<winrt::TextBox>();
        if (!textbox)
        {
            return;
        }

        auto state = GetMaskState(textbox);
        if (!state)
        {
            state = winrt::make_self<MaskState>();
            textbox.SetValue(MaskStateProperty(), *state);
        }

        if (state->SelectionChangedToken)
        {
            textbox.SelectionChanged(state->SelectionChangedToken);
            state->SelectionChangedToken = { 0 };
        }

        if (state->TextChangingToken)
        {
            textbox.TextChanging(state->TextChangingToken);
            state->TextChangingToken = { 0 };
        }

        if (state->PasteToken)
        {
            textbox.Paste(state->PasteToken);
            state->PasteToken = { 0 };
        }

        if (state->LoadedToken)
        {
            textbox.Loaded(state->LoadedToken);
        }

        if (state->GotFocusToken)
        {
            textbox.GotFocus(state->GotFocusToken);
            state->GotFocusToken = { 0 };
        }

        // Subscribe to Loaded to initialize mask
        state->LoadedToken = textbox.Loaded(&TextBoxExtensions::Textbox_Loaded);
    }

    void TextBoxExtensions::Textbox_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        const auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) return;

        const auto mask = GetMask(textbox);

        // In case no value is provided, use it as normal textbox
        if (IsNullOrWhiteSpace(mask))
        {
            return;
        }

        auto placeHolderValue = GetMaskPlaceholder(textbox);
        if (placeHolderValue.empty())
        {
            throw winrt::hresult_invalid_argument(L"PlaceHolder can't be null or empty");
        }

        auto state = GetMaskState(textbox);
        if (!state)
        {
            state = winrt::make_self<MaskState>();
            textbox.SetValue(MaskStateProperty(), *state);
        }
        auto& escapedChars = state->EscapedChars;

        // Process escape characters
        std::wstring builder{ mask };
        for (int i = 0; i < static_cast<int>(builder.length()) - 1; i++)
        {
            if (builder[i] == EscapeChar)
            {
                escapedChars.push_back(i);
                builder.erase(i, 1);
            }
        }

        state->EscapedMask = builder;

        wchar_t placeHolder = placeHolderValue[0];

        // Build representation dictionary
        std::unordered_map<wchar_t, std::wstring> representationDictionary
        {
            { AlphaCharacterRepresentation.first, std::wstring(AlphaCharacterRepresentation.second) },
            { NumericCharacterRepresentation.first, std::wstring(NumericCharacterRepresentation.second) },
            { AlphaNumericRepresentation.first, std::wstring(AlphaNumericRepresentation.second) }
        };

        if (auto customMaskValue = GetCustomMask(textbox); !IsNullOrWhiteSpace(customMaskValue))
        {
            const auto customRoles = Split(customMaskValue, L",");
            for (const auto role : customRoles)
            {
                const auto roleValues = Split(role, L":");
                if (roleValues.size() != 2)
                {
                    throw winrt::hresult_invalid_argument(L"Invalid CustomMask property");
                }

                const auto keyValue = roleValues[0];
                const auto value = roleValues[1];

                // an exception should be throw if the regex is not valid
                try
                {
                    std::wregex testRegex(value.begin(), value.end());
                }
                catch (...)
                {
                    throw winrt::hresult_invalid_argument(L"Invalid CustomMask property, regex is not valid");
                }

                if (keyValue.size() != 1)
                {
                    throw winrt::hresult_invalid_argument(L"Invalid CustomMask property, please validate the mask key");
                }

                representationDictionary[keyValue[0]] = value;
            }
        }

        state->RepresentationDictionary = representationDictionary;

        // Build display text with placeholders
        std::wstring displayText = state->EscapedMask;
        for (size_t i = 0; i < displayText.length(); i++)
        {
            if (std::find(escapedChars.begin(), escapedChars.end(), static_cast<int>(i)) != escapedChars.end())
            {
                continue;
            }

            for (const auto& [key, _] : representationDictionary)
            {
                if (displayText[i] == key)
                {
                    displayText[i] = placeHolder;
                }
            }
        }

        if (textbox.Text().empty())
        {
            textbox.Text(displayText);
        }
        else
        {
            const auto textboxInitialValue = textbox.Text();
            textbox.Text(displayText);
            int oldSelectionStart = state->OldSelectionStart;
            SetTextBoxValue(textboxInitialValue, textbox, state->EscapedMask, escapedChars, representationDictionary, placeHolder, oldSelectionStart);
        }

        if (state->TextChangingToken)
        {
            textbox.TextChanging(state->TextChangingToken);
        }

        if (state->SelectionChangedToken)
        { 
            textbox.SelectionChanged(state->SelectionChangedToken);
        }

        if (state->PasteToken)
        {
            textbox.Paste(state->PasteToken);
        }

        if (state->GotFocusToken)
        {
            textbox.GotFocus(state->GotFocusToken);
        }

        state->TextChangingToken = textbox.TextChanging(&TextBoxExtensions::Textbox_TextChanging);
        state->SelectionChangedToken = textbox.SelectionChanged(&TextBoxExtensions::Textbox_SelectionChanged);
        state->PasteToken = textbox.Paste(&TextBoxExtensions::Textbox_Paste);
        state->GotFocusToken = textbox.GotFocus(&TextBoxExtensions::Textbox_GotFocus_Mask);
        state->OldText = textbox.Text();
        state->DefaultDisplayText = displayText;
        textbox.SelectionStart(0);
    }

    void TextBoxExtensions::Textbox_GotFocus_Mask(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (const auto textbox = sender.try_as<winrt::TextBox>())
        {
            const auto maskValue = GetMask(textbox);
            const auto placeHolderValue = GetMaskPlaceholder(textbox);

            const auto state = GetMaskState(textbox);
            if (!state)
            { 
                return;
            }

            if (IsNullOrWhiteSpace(maskValue) || state->RepresentationDictionary.empty() || placeHolderValue.empty())
            {
                return;
            }

            wchar_t placeHolder = placeHolderValue[0];

            // if the textbox got focus and the textbox is empty (contains only mask) set the textbox cursor at the beginning to simulate normal TextBox behavior if it is empty.
            // if the textbox has value set the cursor to the first empty mask character
            const auto textboxText = textbox.Text();
            for (int i = 0; i < static_cast<int>(textboxText.size()); i++)
            {
                if (placeHolder == textboxText[i])
                {
                    textbox.SelectionStart(i);
                    break;
                }
            }
        }
    }

    winrt::fire_and_forget TextBoxExtensions::Textbox_Paste(winrt::IInspectable sender, winrt::TextControlPasteEventArgs const& e)
    {
        e.Handled(true);

        const auto dataPackageView = winrt::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
        if (!dataPackageView.Contains(winrt::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text()))
        {
            co_return;
        }

        const auto pasteText = co_await dataPackageView.GetTextAsync();
        if (pasteText.empty())
        {
            co_return;
        }

        const auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) co_return;

        const auto maskValue = GetMask(textbox);
        const auto placeHolderValue = GetMaskPlaceholder(textbox);

        const auto state = GetMaskState(textbox);
        if (!state)
        {
            co_return;
        }

        if (IsNullOrWhiteSpace(maskValue) || state->RepresentationDictionary.empty() || placeHolderValue.empty())
        {
            co_return;
        }

        if (state->EscapedMask.empty())
        {
            co_return;
        }

        // to update the textbox text without triggering TextChanging text
        int oldSelectionStart = state->OldSelectionStart;
        textbox.TextChanging(state->TextChangingToken);
        SetTextBoxValue(pasteText, textbox, state->EscapedMask, state->EscapedChars, state->RepresentationDictionary, placeHolderValue[0], oldSelectionStart);
        state->OldText = textbox.Text();
        state->TextChangingToken = textbox.TextChanging(&TextBoxExtensions::Textbox_TextChanging);
    }

    void TextBoxExtensions::SetTextBoxValue(
        std::wstring_view newValue,
        winrt::TextBox const& textbox,
        std::wstring_view mask,
        const std::vector<int>& escapedChars,
        const std::unordered_map<wchar_t, std::wstring>& representationDictionary,
        wchar_t placeholder,
        int oldSelectionStart)
    {
        int maxLength = (static_cast<int>(newValue.length()) + oldSelectionStart) < static_cast<int>(mask.length())
            ? (static_cast<int>(newValue.length()) + oldSelectionStart)
            : static_cast<int>(mask.length());

        std::wstring textArray{ textbox.Text() };

        for (int i = oldSelectionStart; i < maxLength; i++)
        {
            wchar_t maskChar = mask[i];
            wchar_t selectedChar = newValue[i - oldSelectionStart];

            // If dynamic character a,9,* or custom
            const auto dictIt = representationDictionary.find(maskChar);
            if (dictIt != representationDictionary.end() && std::find(escapedChars.begin(), escapedChars.end(), i) == escapedChars.end())
            {
                std::wregex pattern(dictIt->second);
                if (std::regex_match(&selectedChar, &selectedChar + 1, pattern))
                {
                    textArray[i] = selectedChar;
                }
                else
                {
                    textArray[i] = placeholder;
                }
            }
        }

        textbox.Text(textArray);
        textbox.SelectionStart(maxLength);
    }

    void TextBoxExtensions::Textbox_SelectionChanged(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        const auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) return;

        const auto state = GetMaskState(textbox);
        if (!state) return;

        state->OldSelectionStart = textbox.SelectionStart();
        state->OldSelectionLength = textbox.SelectionLength();
    }

    void TextBoxExtensions::Textbox_TextChanging(winrt::TextBox const& textbox, [[maybe_unused]] winrt::TextBoxTextChangingEventArgs const& args)
    {
        const auto state = GetMaskState(textbox);
        if (!state)
        { 
            return;
        }

        const auto& escapedMask = state->EscapedMask;
        const auto& escapedChars = state->EscapedChars;

        const auto& representationDictionary = state->RepresentationDictionary;
        const auto placeHolderValue = GetMaskPlaceholder(textbox);
        const auto& oldText = state->OldText;
        int oldSelectionStart = state->OldSelectionStart;
        int oldSelectionLength = state->OldSelectionLength;

        if (escapedMask.empty() || representationDictionary.empty() || placeHolderValue.empty() || oldText.empty())
        {
            return;
        }

        wchar_t placeHolder = placeHolderValue[0];
        bool isDeleteOrBackspace = false;
        int deleteBackspaceIndex = 0;

        // Delete or backspace is triggered
        // if the new length is less than or equal the old text - the old selection length then a delete or backspace is triggered with or without selection and no characters is added
        const auto text = textbox.Text();
        if (text.size() < oldText.size()
            && text.size() <= oldText.size() - oldSelectionLength)
        {
            isDeleteOrBackspace = true;
            if (oldSelectionLength == 0)
            {
                // backspace else delete
                if (oldSelectionStart != textbox.SelectionStart())
                {
                    deleteBackspaceIndex++;
                }
            }
        }

        // case adding data at the end of the textbox
        if (oldSelectionStart >= static_cast<int>(oldText.size()) && !isDeleteOrBackspace)
        {
            // ignore change(s) if oldtext is a substring of new text value
            if (std::wstring_view{ text }.contains(oldText))
            {
                textbox.Text(oldText);

                if (oldText.size() >= 0)
                {
                    textbox.SelectionStart(static_cast<int>(oldText.size()));
                }

                return;
            }
        }

        std::wstring textArray = oldText;

        // detect if backspace or delete is triggered to handle the right removed character
        int newSelectionIndex = oldSelectionStart - deleteBackspaceIndex;

        // check if single selection
        bool isSingleSelection = oldSelectionLength != 0 && oldSelectionLength != 1;

        // for handling single key click add +1 to match length for selection = 1
        int singleOrMultiSelectionIndex = oldSelectionLength == 0 ? oldSelectionLength + 1 : oldSelectionLength;

        // Case change due to Text property is assigned a value (Ex Textbox.Text="value")
        if (textbox.SelectionStart() == 0 && textbox.FocusState() == winrt::FocusState::Unfocused)
        {
            const auto& displayText = state->DefaultDisplayText;
            if (text.empty())
            {
                state->OldText = displayText;
                state->OldSelectionStart = 0;
                state->OldSelectionLength = 0;
                textbox.Text(displayText);
                return;
            }
            else
            {
                auto textboxInitialValue = text;
                textbox.Text(displayText);
                SetTextBoxValue(std::wstring_view(textboxInitialValue), textbox, escapedMask, escapedChars, representationDictionary, placeHolderValue[0], 0);
                state->OldText = textbox.Text();
                return;
            }
        }

        if (!isDeleteOrBackspace)
        {
            // In case the change happened due to user input
            wchar_t selectedChar = textbox.SelectionStart() > 0 ?
                textbox.Text()[textbox.SelectionStart() - 1] :
                placeHolder;

            wchar_t maskChar = escapedMask[newSelectionIndex];

            const auto dictIt = representationDictionary.find(maskChar);
            // If dynamic character a,9,* or custom
            if (dictIt != representationDictionary.end() && std::find(escapedChars.begin(), escapedChars.end(), newSelectionIndex) == escapedChars.end())
            {
                std::wregex pattern(dictIt->second);
                if (std::regex_match(&selectedChar, &selectedChar + 1, pattern))
                {
                    textArray[newSelectionIndex] = selectedChar;
                    newSelectionIndex++;
                }
                else
                {
                    // if single press don't change
                    if (oldSelectionLength == 0)
                    {
                        textArray[newSelectionIndex] = oldText[newSelectionIndex];
                    }
                    else
                    {
                        // if change in selection reset to default place holder instead of keeping the old valid to be clear for the user
                        textArray[newSelectionIndex] = placeHolder;
                    }
                }
            }
            else
            {
                // if fixed character
                textArray[newSelectionIndex] = oldText[newSelectionIndex];

                // updating text box new index
                newSelectionIndex++;
            }
        }

        if (isSingleSelection || isDeleteOrBackspace)
        {
            for (int i = newSelectionIndex;
                i < (oldSelectionStart - deleteBackspaceIndex + singleOrMultiSelectionIndex);
                i++)
            {
                // TODO: Should have bounds check above?
                wchar_t maskChar = escapedMask[i];

                // If dynamic character a,9,* or custom
                auto dictIt = representationDictionary.find(maskChar);
                if (dictIt != representationDictionary.end() && std::find(escapedChars.begin(), escapedChars.end(), i) == escapedChars.end())
                {
                    textArray[i] = placeHolder;
                }
                else
                {
                    // if fixed character
                    textArray[i] = oldText[i];
                }
            }
        }

        textbox.Text(textArray);
        state->OldText = textbox.Text();
        textbox.SelectionStart(isDeleteOrBackspace ? newSelectionIndex : GetSelectionStart(escapedMask, escapedChars, newSelectionIndex, representationDictionary));
    }

    int TextBoxExtensions::GetSelectionStart(
        std::wstring_view mask,
        const std::vector<int>& escapedChars,
        int selectionIndex,
        const std::unordered_map<wchar_t, std::wstring>& representationDictionary)
    {
        for (int i = selectionIndex; i < static_cast<int>(mask.length()); i++)
        {
            wchar_t maskChar = mask[i];

            // If dynamic character a,9,* or custom
            const auto dictIt = representationDictionary.find(maskChar);
            if (dictIt != representationDictionary.end() && std::find(escapedChars.begin(), escapedChars.end(), i) == escapedChars.end())
            {
                return i;
            }
        }

        return selectionIndex;
    }
}
