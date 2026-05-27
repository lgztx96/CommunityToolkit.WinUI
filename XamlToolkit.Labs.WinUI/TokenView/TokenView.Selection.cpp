#include "pch.h"
#include "winrt_module_imports.h"
#include "TokenView.h"
#include "../XamlToolkit.WinUI/common.h"

namespace winrt
{
    using namespace XamlToolkit::WinUI;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    bool TokenView::MoveFocus(MoveDirection direction)
    {
        bool retVal = false;

        if (auto currentItem = ItemFromContainer(GetCurrentContainerItem()).try_as<DependencyObject>())
        {
            uint32_t previousIndex;

            if (!Items().IndexOf(currentItem, previousIndex))
            {
                return false;
            }

            auto index = previousIndex;

            if (direction == MoveDirection::Previous)
            {
                if (previousIndex > 0)
                {
                    index -= 1;
                }
                else
                {
                    retVal = true;
                }
            }
            else if (direction == MoveDirection::Next)
            {
                if (previousIndex < Items().Size() - 1)
                {
                    index += 1;
                }
            }

            // Only do stuff if the index is actually changing
            if (index != previousIndex)
            {
                if (auto newItem = ContainerFromIndex(index).try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
                {
                    newItem.Focus(FocusState::Keyboard);
                }
                retVal = true;
            }
        }
        return retVal;
    }
}
