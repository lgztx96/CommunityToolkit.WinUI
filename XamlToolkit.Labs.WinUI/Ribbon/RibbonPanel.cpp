#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <ranges>
#include <cmath>
#include <vector>
#endif
#include "RibbonPanel.h"
#if __has_include("RibbonPanel.g.cpp")
#include "RibbonPanel.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::Size RibbonPanel::MeasureOverride(winrt::Size availableSize)
    {
        // We try to limit the layout changes if the parent scrollviewer is sending values with small changes.
        availableSize.Width = std::floor(availableSize.Width);

        auto children = Children();
        std::vector<winrt::UIElement> orderedChildren(children.Size(), { nullptr });
        children.GetMany(0, orderedChildren);

        std::ranges::stable_sort(orderedChildren, [](winrt::UIElement const& a, winrt::UIElement const& b)
        {
            auto groupA = a.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
            auto groupB = b.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
            int priA = groupA ? groupA.Priority() : 0;
            int priB = groupB ? groupB.Priority() : 0;
            return priA < priB;
        });

        winrt::Size desiredSize{};

        for (auto const& child : orderedChildren)
        {
            auto collapsibleGroup = child.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
            auto requestedWidths = collapsibleGroup ? collapsibleGroup.RequestedWidths() : nullptr;

            if (!requestedWidths || (collapsibleGroup && collapsibleGroup.State() == winrt::Visibility::Collapsed))
            {
                child.Measure(GroupAvailableSize);
            }
            else
            {
                // Get the closest match to remainingWidth or use infinite size if we do not have any match.
                double remainingWidth = availableSize.Width - desiredSize.Width;

                double requestedWidth = std::numeric_limits<double>::infinity();
                for (auto const& w : requestedWidths)
                {
                    if (w <= remainingWidth)
                    {
                        requestedWidth = w;
                    }
                }

                winrt::Size fixedSize{ static_cast<float>(requestedWidth), availableSize.Height };
                child.Measure(fixedSize);
            }

            desiredSize.Width += child.DesiredSize().Width;
            desiredSize.Height = std::max(desiredSize.Height, child.DesiredSize().Height);
        }

        if (desiredSize.Width > availableSize.Width)
        {
            // We need to collapse some groups.
            // If there is no priority order we assume that the last items are the one which should collapse first.
            for (auto const& item : orderedChildren | std::views::reverse)
            {
                auto group = item.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();

                if (!group || group.State() != winrt::Visibility::Visible)
                {
                    continue;
                }

                group.State(winrt::Visibility::Collapsed);
                auto previousSize = group.DesiredSize();
                group.Measure(GroupAvailableSize);
                auto newSize = group.DesiredSize();

                if (newSize.Width < previousSize.Width)
                {
                    desiredSize.Width -= previousSize.Width - newSize.Width;
                    desiredSize.Height = std::max(desiredSize.Height, newSize.Height);
                }
                else
                {
                    group.State(winrt::Visibility::Visible);
                    group.Measure(GroupAvailableSize);
                }

                if (desiredSize.Width < availableSize.Width)
                {
                    break;
                }
            }
        }
        else if (desiredSize.Width < availableSize.Width)
        {
            // We have more space than needed, we check if we can expand some groups
            for (auto const& item : orderedChildren)
            {
                auto group = item.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();

                if (!group || group.State() != winrt::Visibility::Collapsed)
                {
                    continue;
                }

                auto previousSize = group.DesiredSize();
                group.State(winrt::Visibility::Visible);

                auto requestedWidths = group.RequestedWidths();
                if (!requestedWidths)
                {
                    group.Measure(GroupAvailableSize);
                }
                else
                {
                    // Get the closest match to remainingWidth or use infinite size if we do not have any match.
                    double remainingWidth = availableSize.Width + previousSize.Width - desiredSize.Width;

                    double requestedWidth = std::numeric_limits<double>::infinity();
                    for (auto const& w : requestedWidths)
                    {
                        if (w <= remainingWidth)
                        {
                            requestedWidth = w;
                        }
                    }

                    winrt::Size fixedSize{ static_cast<float>(requestedWidth), availableSize.Height };
                    group.Measure(fixedSize);
                }

                auto newSize = group.DesiredSize();
                double widthIncrease = newSize.Width - previousSize.Width;
                if (desiredSize.Width + widthIncrease > availableSize.Width)
                {
                    // Too wide, we revert the change
                    group.State(winrt::Visibility::Collapsed);
                    group.Measure(GroupAvailableSize);
                    break;
                }

                desiredSize.Width += static_cast<float>(widthIncrease);
                desiredSize.Height = std::max(desiredSize.Height, newSize.Height);
            }
        }

        return desiredSize;
    }

    winrt::Size RibbonPanel::ArrangeOverride(winrt::Size finalSize)
    {
        winrt::Rect position{ 0, 0, 0, finalSize.Height };

        for (auto const& child : Children())
        {
            position.Width = child.DesiredSize().Width;
            child.Arrange(position);
            position.X += position.Width;
        }

        return { position.X, position.Height };
    }
}
