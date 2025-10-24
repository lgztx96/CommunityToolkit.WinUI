#include "pch.h"
#include "TabbedCommandBarItemTemplateSelector.h"
#if __has_include("TabbedCommandBarItemTemplateSelector.g.cpp")
#include "TabbedCommandBarItemTemplateSelector.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	DataTemplate TabbedCommandBarItemTemplateSelector::SelectTemplateCore(IInspectable const& item)
	{
		if (auto t = item.try_as<winrt::CommunityToolkit::WinUI::Controls::TabbedCommandBarItem>()) 
		{
			if (t.IsContextual())
			{
				return Contextual();
			}
		}

		return Normal();
	}

	DataTemplate TabbedCommandBarItemTemplateSelector::SelectTemplateCore(IInspectable const& item, [[maybe_unused]] DependencyObject const& container)
	{
		return SelectTemplateCore(item);
	}
}
