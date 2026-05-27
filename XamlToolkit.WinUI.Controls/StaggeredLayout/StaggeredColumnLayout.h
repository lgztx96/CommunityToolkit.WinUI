#pragma once
#include "StaggeredItem.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <vector>
#endif

namespace winrt::XamlToolkit::WinUI::Controls
{
    class StaggeredColumnLayout final : public std::vector<StaggeredItem*>
    {
        using base = std::vector<StaggeredItem*>;
    public:
        StaggeredColumnLayout() noexcept : height(0.0) {}

        double Height() const noexcept { return height; };

        void push_back(StaggeredItem* item)
        {
            height = item->Top() + item->Height();
            base::push_back(item);
        }

        void clear()
        {
            height = 0.0;
            base::clear();
        }

    private:
        double height;
    };
}
