#pragma once

#include "ListViewExtensions.g.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    struct ListViewExtensions : ListViewExtensionsT<ListViewExtensions>
    {
        static void DeselectAll(ListViewBase const& list);

        static void SelectAllSafe(ListViewBase const& list);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct ListViewExtensions : ListViewExtensionsT<ListViewExtensions, implementation::ListViewExtensions>
    {
    };
}
