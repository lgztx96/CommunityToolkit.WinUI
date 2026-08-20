#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"

#ifdef __INTELLISENSE__
#include <regex>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    static bool IsNullOrWhiteSpace(std::wstring_view s)
    {
        return s.empty() || std::all_of(s.begin(), s.end(), [](wchar_t c) { return std::iswspace(c); });
    }

    // Per-textbox mask state storage
    struct MaskState
    {
        std::vector<int> EscapedChars;
        std::wstring EscapedMask;
        std::unordered_map<wchar_t, std::wstring> RepresentationDictionary;
        std::wstring OldText;
        std::wstring DefaultDisplayText;
        int OldSelectionStart = 0;
        int OldSelectionLength = 0;
        winrt::event_token SelectionChangedToken;
        winrt::event_token TextChangingToken;
        winrt::event_token PasteToken;
        winrt::event_token LoadedToken;
        winrt::event_token GotFocusToken;
    };

    struct WeakTextBoxHash
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

    static std::unordered_map<winrt::weak_ref<winrt::TextBox>, std::unique_ptr<MaskState>, WeakTextBoxHash> _maskStates;

    static MaskState* GetOrCreateMaskState(winrt::TextBox const& textbox)
    {
        if (auto it = _maskStates.find(textbox); it != _maskStates.end())
        {
            // Verify the weak_ref is still alive; clean up stale entry if not
            if (auto ref = it->first.get())
            {
                return it->second.get();
            }
            _maskStates.erase(it);
        }
        auto state = std::make_unique<MaskState>();
        auto ptr = state.get();
        _maskStates.emplace(textbox, std::move(state));
        return ptr;
    }

    void TextBoxExtensions::InitTextBoxMask(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // TODO: We should think about if we want to adopt this pattern, but have a DiagnosticSupressor to prevent warning for non-braced if that we should set in .editorconfig.
        auto textbox = d.try_as<winrt::TextBox>();
        if (!textbox) return;

        auto state = GetOrCreateMaskState(textbox);

        // Remove old event handlers
        textbox.SelectionChanged(state->SelectionChangedToken);
        textbox.TextChanging(state->TextChangingToken);
        textbox.Paste(state->PasteToken);
        textbox.Loaded(state->LoadedToken);
        textbox.GotFocus(state->GotFocusToken);

        // Subscribe to Loaded to initialize mask
        state->LoadedToken = textbox.Loaded(&TextBoxExtensions::Textbox_Loaded);
    }

    void TextBoxExtensions::Textbox_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) return;

        auto maskValue = GetMask(textbox);
        std::wstring mask(maskValue);

        // In case no value is provided, use it as normal textbox
        if (IsNullOrWhiteSpace(mask))
        {
            return;
        }

        auto placeHolderValue = GetMaskPlaceholder(textbox);
        std::wstring placeHolderStr(placeHolderValue);
        if (placeHolderStr.empty())
        {
            throw winrt::hresult_invalid_argument(L"PlaceHolder can't be null or empty");
        }

        auto state = GetOrCreateMaskState(textbox);
        auto& escapedChars = state->EscapedChars;

        // Process escape characters
        std::wstring builder = mask;
        for (int i = 0; i < static_cast<int>(builder.length()) - 1; i++)
        {
            if (builder[i] == EscapeChar)
            {
                escapedChars.push_back(i);
                builder.erase(i, 1);
            }
        }

        state->EscapedMask = builder;

        wchar_t placeHolder = placeHolderStr[0];

        // Build representation dictionary
        std::unordered_map<wchar_t, std::wstring> representationDictionary
        {
            { AlphaCharacterRepresentation.first, std::wstring(AlphaCharacterRepresentation.second) },
            { NumericCharacterRepresentation.first, std::wstring(NumericCharacterRepresentation.second) },
            { AlphaNumericRepresentation.first, std::wstring(AlphaNumericRepresentation.second) }
        };

        auto customMaskValue = GetCustomMask(textbox);
        std::wstring_view customMaskStr(customMaskValue);
        if (!customMaskStr.empty())
        {
            // Split by comma for roles
            size_t start = 0;
            size_t end;
            while ((end = customMaskStr.find(L',', start)) != std::wstring::npos)
            {
                auto role = customMaskStr.substr(start, end - start);
                auto colonPos = role.find(L':');
                if (colonPos == std::wstring::npos)
                {
                    throw winrt::hresult_invalid_argument(L"Invalid CustomMask property");
                }

                auto keyValue = role.substr(0, colonPos);
                auto value = role.substr(colonPos + 1);

                // validate regex
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
                start = end + 1;
            }

            // Last role
            if (start < customMaskStr.size())
            {
                auto role = customMaskStr.substr(start);
                auto colonPos = role.find(L':');
                if (colonPos == std::wstring::npos)
                {
                    throw winrt::hresult_invalid_argument(L"Invalid CustomMask property");
                }

                auto keyValue = role.substr(0, colonPos);
                auto value = role.substr(colonPos + 1);

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
        std::wstring displayTextBuilder = state->EscapedMask;
        for (size_t i = 0; i < displayTextBuilder.length(); i++)
        {
            if (std::find(escapedChars.begin(), escapedChars.end(), static_cast<int>(i)) != escapedChars.end())
            {
                continue;
            }

            for (const auto& pair : representationDictionary)
            {
                if (displayTextBuilder[i] == pair.first)
                {
                    displayTextBuilder[i] = placeHolder;
                }
            }
        }

        std::wstring displayText = displayTextBuilder;

        if (textbox.Text().empty())
        {
            textbox.Text(displayText);
        }
        else
        {
            auto textboxInitialValue = textbox.Text();
            textbox.Text(displayText);
            int oldSelectionStart = state->OldSelectionStart;
            SetTextBoxValue(std::wstring_view(textboxInitialValue), textbox, state->EscapedMask, escapedChars, representationDictionary, placeHolder, oldSelectionStart);
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
        auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) return;

        auto maskValue = GetMask(textbox);
        auto placeHolderValue = GetMaskPlaceholder(textbox);

        auto it = _maskStates.find(winrt::weak_ref<winrt::TextBox>(textbox));
        if (it == _maskStates.end()) return;
        auto state = it->second.get();

        if (IsNullOrWhiteSpace(maskValue) || state->RepresentationDictionary.empty() || placeHolderValue.empty())
        {
            return;
        }

        wchar_t placeHolder = std::wstring(placeHolderValue)[0];

        // if the textbox got focus and the textbox is empty (contains only mask) set the textbox cursor at the beginning to simulate normal TextBox behavior if it is empty.
        // if the textbox has value set the cursor to the first empty mask character
        auto textboxText = textbox.Text();
        for (int i = 0; i < static_cast<int>(textboxText.size()); i++)
        {
            if (placeHolder == textboxText[i])
            {
                textbox.SelectionStart(i);
                break;
            }
        }
    }

    winrt::fire_and_forget TextBoxExtensions::Textbox_Paste(winrt::IInspectable sender, winrt::TextControlPasteEventArgs const& e)
    {
        e.Handled(true);

        auto dataPackageView = winrt::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
        if (!dataPackageView.Contains(winrt::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text()))
        {
            co_return;
        }

        auto pasteText = co_await dataPackageView.GetTextAsync();
        if (pasteText.empty())
        {
            co_return;
        }

        auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) co_return;

        auto maskValue = GetMask(textbox);
        auto placeHolderValue = GetMaskPlaceholder(textbox);

        auto it = _maskStates.find(textbox);
        if (it == _maskStates.end()) co_return;
        auto state = it->second.get();

        if (IsNullOrWhiteSpace(maskValue) || state->RepresentationDictionary.empty() || placeHolderValue.empty())
        {
            co_return;
        }

        if (state->EscapedMask.empty())
        {
            co_return;
        }

        int oldSelectionStart = state->OldSelectionStart;
        textbox.TextChanging(state->TextChangingToken); // Remove handler temporarily
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
            auto dictIt = representationDictionary.find(maskChar);
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
        auto textbox = sender.try_as<winrt::TextBox>();
        if (!textbox) return;

        auto it = _maskStates.find(textbox);
        if (it == _maskStates.end()) return;
        auto state = it->second.get();

        state->OldSelectionStart = textbox.SelectionStart();
        state->OldSelectionLength = textbox.SelectionLength();
    }

    void TextBoxExtensions::Textbox_TextChanging(winrt::TextBox const& textbox, [[maybe_unused]] winrt::TextBoxTextChangingEventArgs const& args)
    {
        auto it = _maskStates.find(textbox);
        if (it == _maskStates.end()) return;
        auto state = it->second.get();

        auto escapedMask = state->EscapedMask;
        auto& escapedChars = state->EscapedChars;

        auto& representationDictionary = state->RepresentationDictionary;
        auto placeHolderValue = GetMaskPlaceholder(textbox);
        auto& oldText = state->OldText;
        int oldSelectionStart = state->OldSelectionStart;
        int oldSelectionLength = state->OldSelectionLength;

        if (escapedMask.empty() || representationDictionary.empty() || placeHolderValue.empty() || oldText.empty())
        {
            return;
        }

        wchar_t placeHolder = std::wstring(placeHolderValue)[0];
        bool isDeleteOrBackspace = false;
        int deleteBackspaceIndex = 0;

        // Delete or backspace is triggered
        // if the new length is less than or equal the old text - the old selection length then a delete or backspace is triggered with or without selection and no characters is added
        auto text = textbox.Text();
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
            auto displayText = state->DefaultDisplayText;
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

            auto dictIt = representationDictionary.find(maskChar);
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
            auto dictIt = representationDictionary.find(maskChar);
            if (dictIt != representationDictionary.end() && std::find(escapedChars.begin(), escapedChars.end(), i) == escapedChars.end())
            {
                return i;
            }
        }

        return selectionIndex;
    }
}
