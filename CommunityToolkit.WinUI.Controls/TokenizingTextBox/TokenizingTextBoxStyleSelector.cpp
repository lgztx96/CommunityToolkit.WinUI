#include "pch.h"
#include "TokenizingTextBoxStyleSelector.h"
#if __has_include("TokenizingTextBoxStyleSelector.g.cpp")
#include "TokenizingTextBoxStyleSelector.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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
