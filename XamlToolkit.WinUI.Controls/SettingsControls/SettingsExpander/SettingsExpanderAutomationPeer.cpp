#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsExpanderAutomationPeer.h"
#if __has_include("SettingsExpanderAutomationPeer.g.cpp")
#include "SettingsExpanderAutomationPeer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    SettingsExpanderAutomationPeer::SettingsExpanderAutomationPeer(owner_type const& owner)
        : SettingsExpanderAutomationPeerT<SettingsExpanderAutomationPeer>(owner)
    {
    }

    winrt::AutomationControlType SettingsExpanderAutomationPeer::GetAutomationControlTypeCore()
    {
        return winrt::AutomationControlType::Group;
    }

    winrt::hstring SettingsExpanderAutomationPeer::GetClassNameCore()
    {
        return winrt::xaml_typename<owner_type>().Name;
    }

    winrt::hstring SettingsExpanderAutomationPeer::GetNameCore()
    {
        winrt::hstring name = base_type::GetNameCore();

        if (auto owner = Owner().try_as<owner_type>())
        {
            if (!winrt::AutomationProperties::GetName(owner).empty())
            {
                name = winrt::AutomationProperties::GetName(owner);
            }
            else
            {
                if (auto headerString = owner.Header().try_as<winrt::hstring>(); headerString && !headerString->empty())
                {
                    name = headerString.value();
                }
            }
        }
        return name;
    }

    void SettingsExpanderAutomationPeer::RaiseExpandedChangedEvent(bool newValue)
    {
        winrt::ExpandCollapseState newState = newValue ? winrt::ExpandCollapseState::Expanded : winrt::ExpandCollapseState::Collapsed;

        winrt::ExpandCollapseState oldState = (newState == winrt::ExpandCollapseState::Expanded) ?
            winrt::ExpandCollapseState::Collapsed :
            winrt::ExpandCollapseState::Expanded;

        RaisePropertyChangedEvent(winrt::ExpandCollapsePatternIdentifiers::ExpandCollapseStateProperty(), winrt::box_value(oldState), winrt::box_value(newState));
    }
}
