#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>
#endif
#include "StaggeredLayoutState.h"
#undef max

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void StaggeredLayoutState::AddItemToColumn(StaggeredItem* item, int columnIndex)
    {
        auto [itr, _] = _columnLayout.try_emplace(columnIndex);
        auto& columnLayout = (*itr).second;

        if (std::find(columnLayout.begin(), columnLayout.end(), item) == columnLayout.end())
        {
            columnLayout.push_back(item);
        }
    }

    void StaggeredLayoutState::RemoveFromIndex(int index)
    {
        if (index >= static_cast<int>(_items.size()))
        {
            // Item was added/removed but we haven't realized that far yet
            return;
        }

        int numToRemove = (int)_items.size() - index;

        for (auto iter = _items.begin() + index;
            iter != _items.begin() + index + numToRemove;
            iter++)
        {
            delete* iter;
        }

        _items.erase(_items.begin() + index, _items.begin() + index + numToRemove);

        for (auto& [_, layout] : _columnLayout)
        {
            for (size_t i = 0; i < layout.size(); i++)
            {
                if (layout[i]->Index() >= index)
                {
                    numToRemove = static_cast<int>(layout.size() - i);
                    layout.erase(layout.begin() + i, layout.begin() + i + numToRemove);
                    break;
                }
            }
        }
    }

    double StaggeredLayoutState::GetHeight()
    {
        auto values = std::views::values(_columnLayout);
        double desiredHeight = std::ranges::max(std::views::transform(values,
            [](const auto& layout) { return layout.Height(); }));

        int itemCount = std::accumulate(values.begin(), values.end(), 0,
            [](int sum, const auto& layout) { return static_cast<int>(sum + layout.size()); });

        if (itemCount == _context.ItemCount())
        {
            return desiredHeight;
        }

        double averageHeight = 0;
        for (const auto& kvp : _columnLayout)
        {
            averageHeight += kvp.second.Height() / kvp.second.size();
        }

        averageHeight /= _columnLayout.size();
        double estimatedHeight = (averageHeight * _context.ItemCount()) / _columnLayout.size();
        if (estimatedHeight > desiredHeight)
        {
            desiredHeight = estimatedHeight;
        }

        if (std::abs(desiredHeight - _lastAverageHeight) < 5)
        {
            return _lastAverageHeight;
        }

        _lastAverageHeight = desiredHeight;
        return desiredHeight;
    }

    StaggeredItem* StaggeredLayoutState::GetItemAt(int index)
    {
        if (index <= (static_cast<int>(_items.size()) - 1))
        {
            return _items[index];
        }
        else
        {
            auto item = new StaggeredItem(index);
            _items.push_back(item);
            return item;
        }
    }

    void StaggeredLayoutState::RecycleElementAt(int index)
    {
        const auto element = _context.GetOrCreateElementAt(index);
        _context.RecycleElement(element);
    }

    void StaggeredLayoutState::RemoveRange(int startIndex, int endIndex)
    {
        for (int i = startIndex; i <= endIndex; i++)
        {
            if (i > static_cast<int>(_items.size()))
            {
                break;
            }

            auto item = _items[i];
            item->Height(0);
            item->Top(0);

            // We must recycle all elements to ensure that it gets the correct context
            RecycleElementAt(i);
        }

        for (auto& [_, layout] : _columnLayout)
        {
            for (size_t i = 0; i < layout.size(); i++)
            {
                if ((startIndex <= layout[i]->Index()) && (layout[i]->Index() <= endIndex))
                {
                    int numToRemove = static_cast<int>(layout.size() - i);
                    layout.erase(layout.begin() + i, layout.begin() + i + numToRemove);
                    break;
                }
            }
        }
    }

    void StaggeredLayoutState::Clear()
    {
        _columnLayout.clear();
        for (auto item : _items)
        {
            delete item;
        }
        _items.clear();
    }

    void StaggeredLayoutState::InitializeForContext(
        winrt::Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context)
    {
        _context = context;
    }
}
