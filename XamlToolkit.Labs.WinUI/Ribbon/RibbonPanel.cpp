#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <vector>
#endif
#include "RibbonPanel.h"
#if __has_include("RibbonPanel.g.cpp")
#include "RibbonPanel.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	Size RibbonPanel::MeasureOverride(Size availableSize)
	{
		// We try to limit the layout changes if the parent scrollviewer is sending values with small changes.
		availableSize.Width = std::floor(availableSize.Width);

		auto children = Children();
		std::vector<UIElement> orderedChildren(children.Size(), { nullptr });
		children.GetMany(0, orderedChildren);

		std::ranges::sort(orderedChildren, [](UIElement const& a, UIElement const& b)
			{
				auto groupA = a.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
				auto groupB = b.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
				int priA = groupA ? groupA.Priority() : 0;
				int priB = groupB ? groupB.Priority() : 0;
				return priA < priB;
			});

		Size desiredSize{};

		for (auto const& child : orderedChildren)
		{
			auto collapsibleGroup = child.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
			auto requestedWidths = collapsibleGroup ? collapsibleGroup.RequestedWidths() : nullptr;

			if (!requestedWidths || collapsibleGroup && collapsibleGroup.State() == Visibility::Collapsed)
			{
				child.Measure(GroupAvailableSize);
			}
			else
			{
				// Get the closest match to remainingWidth or use infinite size if we do not have any match.
				double remainingWidth = availableSize.Width - desiredSize.Width;
				std::vector<double> matchingWidths;

				for (auto const& w : requestedWidths)
				{
					if (w <= remainingWidth)
					{
						matchingWidths.push_back(w);
					}
				}

				double requestedWidth = matchingWidths.empty() ? std::numeric_limits<double>::infinity() : matchingWidths.back();
				Size fixedSize{ static_cast<float>(requestedWidth), availableSize.Height };
				child.Measure(fixedSize);
			}

			desiredSize.Width += child.DesiredSize().Width;
			desiredSize.Height = std::max(desiredSize.Height, child.DesiredSize().Height);
		}

		if (desiredSize.Width > availableSize.Width)
		{
			// We need to collapse some groups.
			 // If there is no priority order we assume that the last items are the one which should collapse first.
			std::vector<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup> groups;
			for (auto const& item : children)
			{
				auto group = item.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
				if (group && group.State() == Visibility::Visible)
					groups.emplace_back(group);
			}

			std::ranges::reverse(groups);
			std::ranges::sort(groups, [](auto const& a, auto const& b)
				{
					return a.Priority() > b.Priority();
				});

			for (auto const& group : groups)
			{
				group.State(Visibility::Collapsed);
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
					group.State(Visibility::Visible);
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
			std::vector<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup> groups;
			for (auto const& item : children)
			{
				auto group = item.try_as<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>();
				if (group && group.State() == Visibility::Collapsed)
					groups.emplace_back(group);
			}

			std::ranges::sort(groups, [](auto const& a, auto const& b)
				{
					return a.Priority() < b.Priority();
				});

			for (auto const& group : groups)
			{
				auto previousSize = group.DesiredSize();
				group.State(Visibility::Visible);

				auto requestedWidths = group.RequestedWidths();
				if (!requestedWidths)
				{
					group.Measure(GroupAvailableSize);
				}
				else
				{
					// Get the closest match to remainingWidth or use infinite size if we do not have any match.
					double remainingWidth = availableSize.Width + previousSize.Width - desiredSize.Width;
					std::vector<double> matchingWidths;
					for (auto const& w : requestedWidths)
					{
						if (w <= remainingWidth)
							matchingWidths.push_back(w);
					}

					double requestedWidth = matchingWidths.empty() ? std::numeric_limits<double>::infinity() : matchingWidths.back();
					Size fixedSize{ static_cast<float>(requestedWidth), availableSize.Height };
					group.Measure(fixedSize);
				}

				auto newSize = group.DesiredSize();
				double widthIncrease = newSize.Width - previousSize.Width;
				if (desiredSize.Width + widthIncrease > availableSize.Width)
				{
					// Too wide, we revert the change
					group.State(Visibility::Collapsed);
					group.Measure(GroupAvailableSize);
					break;
				}

				desiredSize.Width += static_cast<float>(widthIncrease);
				desiredSize.Height = std::max(desiredSize.Height, newSize.Height);
			}
		}

		return desiredSize;
	}

	Size RibbonPanel::ArrangeOverride(Size finalSize)
	{
		Rect position{ 0, 0, 0, finalSize.Height };

		for (auto const& child : Children())
		{
			position.Width = child.DesiredSize().Width;
			child.Arrange(position);
			position.X += position.Width;
		}

		return { position.X, position.Height };
	}
}
