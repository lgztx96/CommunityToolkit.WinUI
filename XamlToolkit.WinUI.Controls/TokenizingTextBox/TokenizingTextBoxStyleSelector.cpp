#include "pch.h"
#include "winrt_module_imports.h"
#include "TokenizingTextBoxStyleSelector.h"
#if __has_include("TokenizingTextBoxStyleSelector.g.cpp")
#include "TokenizingTextBoxStyleSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    Style TokenizingTextBoxStyleSelector::SelectStyleCore(IInspectable const& item, [[maybe_unused]] DependencyObject const& container)
    {
        if (item.try_as<ITokenStringContainer>())
        {
            return TextStyle;
        }

        return TokenStyle;
    }
}
