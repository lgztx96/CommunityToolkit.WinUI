#include "pch.h"
#include "winrt_module_imports.h"
#include "TabbedCommandBarItemTemplateSelector.h"
#if __has_include("TabbedCommandBarItemTemplateSelector.g.cpp")
#include "TabbedCommandBarItemTemplateSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::DataTemplate TabbedCommandBarItemTemplateSelector::SelectTemplateCore(winrt::IInspectable const& item) const
	{
		if (auto t = item.try_as<winrt::XamlToolkit::WinUI::Controls::TabbedCommandBarItem>(); t && t.IsContextual())
		{
			return Contextual();
		}

		return Normal();
	}

	winrt::DataTemplate TabbedCommandBarItemTemplateSelector::SelectTemplateCore(winrt::IInspectable const& item, [[maybe_unused]] winrt::DependencyObject const& container)
	{
		return SelectTemplateCore(item);
	}
}
