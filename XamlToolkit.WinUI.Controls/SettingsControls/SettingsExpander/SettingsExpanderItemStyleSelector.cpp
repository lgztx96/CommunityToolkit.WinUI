#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsExpanderItemStyleSelector.h"
#if __has_include("SettingsExpanderItemStyleSelector.g.cpp")
#include "SettingsExpanderItemStyleSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	Style SettingsExpanderItemStyleSelector::SelectStyleCore(
		[[maybe_unused]] winrt::Windows::Foundation::IInspectable const& item, DependencyObject const& container)
	{
		if (auto card = container.try_as<XamlToolkit::WinUI::Controls::SettingsCard>(); card && card.IsClickEnabled())
		{
			return ClickableStyle;
		}
		else
		{
			return DefaultStyle;
		}
	}
}
