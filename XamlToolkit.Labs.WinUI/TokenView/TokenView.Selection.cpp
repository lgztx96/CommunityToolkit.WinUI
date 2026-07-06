#include "pch.h"
#include "winrt_module_imports.h"
#include "TokenView.h"

namespace winrt
{
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    bool TokenView::MoveFocus(MoveDirection direction)
    {
        bool retVal = false;

        auto currentContainerItem = GetCurrentContainerItem();
        if (!currentContainerItem)
        {
            return false;
        }

        if (auto currentItem = ItemFromContainer(currentContainerItem))
        {
            auto items = Items();
            uint32_t previousIndex;
            if (!items.IndexOf(currentItem, previousIndex))
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
                if (previousIndex + 1 < items.Size())
                {
                    index += 1;
                }
            }

            // Only do stuff if the index is actually changing
            if (index != previousIndex)
            {
                if (auto newItem = ContainerFromIndex(index).try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
                {
                    newItem.Focus(winrt::FocusState::Keyboard);
                }
                retVal = true;
            }
        }
        return retVal;
    }
}
