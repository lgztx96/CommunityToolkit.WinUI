#include "pch.h"
#include "SettingsCardAutomationPeer.h"
#if __has_include("SettingsCardAutomationPeer.g.cpp")
#include "SettingsCardAutomationPeer.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    SettingsCardAutomationPeer::SettingsCardAutomationPeer(owner_type const& owner)
        : SettingsCardAutomationPeerT<SettingsCardAutomationPeer>(owner)
    {

    }

    AutomationControlType SettingsCardAutomationPeer::GetAutomationControlTypeCore()
    {
        if (auto settingsCard = Owner().try_as<owner_type>(); settingsCard && settingsCard.IsClickEnabled())
        {
            return AutomationControlType::Button;
        }
        else
        {
            return AutomationControlType::Group;
        }
    }

    winrt::hstring SettingsCardAutomationPeer::GetClassNameCore()
    {
        return winrt::xaml_typename<owner_type>().Name;
    }

    winrt::hstring SettingsCardAutomationPeer::GetNameCore()
    {
        // We only want to announce the button card name if it is clickable, else it's just a regular card that does not receive focus
        if (auto owner = Owner().try_as<owner_type>(); owner && owner.IsClickEnabled())
        {
            winrt::hstring name = AutomationProperties::GetName(owner);
            if (!name.empty())
            {
                return name;
            }
            else
            {
                if (auto headerString = owner.Header().try_as<winrt::hstring>(); headerString && !headerString->empty())
                {
                    return headerString.value();
                }
            }
        }

        return base_type::GetNameCore();
    }

    Windows::Foundation::IInspectable SettingsCardAutomationPeer::GetPatternCore(PatternInterface const& patternInterface)
    {
        if (patternInterface == PatternInterface::Invoke)
        {
            if (auto owner = Owner().try_as<owner_type>(); owner && owner.IsClickEnabled())
            {
                // Only provide Invoke pattern if the card is clickable
                return *this;
            }
            else
            {
                // Not clickable, do not provide Invoke pattern
                return nullptr;
            }
        }

        return base_type::GetPatternCore(patternInterface);
    }
}
