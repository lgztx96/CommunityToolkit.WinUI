#include "pch.h"
#include "SettingsExpanderAutomationPeer.h"
#if __has_include("SettingsExpanderAutomationPeer.g.cpp")
#include "SettingsExpanderAutomationPeer.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    SettingsExpanderAutomationPeer::SettingsExpanderAutomationPeer(owner_type const& owner)
        : SettingsExpanderAutomationPeerT<SettingsExpanderAutomationPeer>(owner)
    {
    }

    AutomationControlType SettingsExpanderAutomationPeer::GetAutomationControlTypeCore()
    {
        return AutomationControlType::Group;
    }

    hstring SettingsExpanderAutomationPeer::GetClassNameCore()
    {
        return winrt::xaml_typename<owner_type>().Name;
    }

    hstring SettingsExpanderAutomationPeer::GetNameCore()
    {
        hstring name = base_type::GetNameCore();

        if (auto owner = Owner().try_as<owner_type>())
        {
            if (!AutomationProperties::GetName(owner).empty())
            {
                name = AutomationProperties::GetName(owner);
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
        ExpandCollapseState newState = (newValue == true) ?
            ExpandCollapseState::Expanded :
            ExpandCollapseState::Collapsed;

        ExpandCollapseState oldState = (newState == ExpandCollapseState::Expanded) ?
            ExpandCollapseState::Collapsed :
            ExpandCollapseState::Expanded;

        RaisePropertyChangedEvent(ExpandCollapsePatternIdentifiers::ExpandCollapseStateProperty(), winrt::box_value(oldState), winrt::box_value(newState));
    }
}
