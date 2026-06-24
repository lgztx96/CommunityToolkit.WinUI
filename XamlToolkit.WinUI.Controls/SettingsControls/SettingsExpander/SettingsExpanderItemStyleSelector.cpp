#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsExpanderItemStyleSelector.h"
#if __has_include("SettingsExpanderItemStyleSelector.g.cpp")
#include "SettingsExpanderItemStyleSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    winrt::Style SettingsExpanderItemStyleSelector::SelectStyleCore(
        [[maybe_unused]] winrt::IInspectable const& item, winrt::DependencyObject const& container)
    {
        if (auto card = container.try_as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(); card && card.IsClickEnabled())
        {
            return ClickableStyle;
        }
        else
        {
            return DefaultStyle;
        }
    }
}
