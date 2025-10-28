#include "pch.h"
#include "ListViewExtensions.h"
#if __has_include("ListViewExtensions.g.cpp")
#include "ListViewExtensions.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::CommunityToolkit::WinUI::implementation
{
    void ListViewExtensions::DeselectAll(ListViewBase const& list)
    {
        switch (list.SelectionMode())
        {
        case ListViewSelectionMode::Single:
            list.SelectedItem(nullptr);
            break;
        case ListViewSelectionMode::Multiple:
        case ListViewSelectionMode::Extended:
            list.DeselectRange(ItemIndexRange(0, list.Items().Size()));
            break;
        }
    }

    void ListViewExtensions::SelectAllSafe(ListViewBase const& list)
    {
        switch (list.SelectionMode())
        {
        case ListViewSelectionMode::Single:
            list.SelectedItem(list.Items().Size() > 0 ? list.Items().GetAt(0) : nullptr);
            break;
        case ListViewSelectionMode::Multiple:
        case ListViewSelectionMode::Extended:
            list.SelectRange(ItemIndexRange(0, list.Items().Size()));
            break;
        }
    }
}
