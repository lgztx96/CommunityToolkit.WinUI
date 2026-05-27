#include "pch.h"
#include "winrt_module_imports.h"
#include "TabbedCommandBarItemTemplateSelector.h"
#if __has_include("TabbedCommandBarItemTemplateSelector.g.cpp")
#include "TabbedCommandBarItemTemplateSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	DataTemplate TabbedCommandBarItemTemplateSelector::SelectTemplateCore(IInspectable const& item)
	{
		if (auto t = item.try_as<winrt::XamlToolkit::WinUI::Controls::TabbedCommandBarItem>()) 
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
