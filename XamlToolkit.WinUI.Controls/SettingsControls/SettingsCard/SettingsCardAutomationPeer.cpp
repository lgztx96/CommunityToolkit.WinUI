#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsCardAutomationPeer.h"
#if __has_include("SettingsCardAutomationPeer.g.cpp")
#include "SettingsCardAutomationPeer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    SettingsCardAutomationPeer::SettingsCardAutomationPeer(owner_type const& owner)
        : SettingsCardAutomationPeerT<SettingsCardAutomationPeer>(owner)
    {

    }

    winrt::AutomationControlType SettingsCardAutomationPeer::GetAutomationControlTypeCore()
    {
        if (auto settingsCard = Owner().try_as<owner_type>(); settingsCard && settingsCard.IsClickEnabled())
        {
            return winrt::AutomationControlType::Button;
        }
        else
        {
            return winrt::AutomationControlType::Group;
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
            winrt::hstring name = winrt::AutomationProperties::GetName(owner);
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

    winrt::IInspectable SettingsCardAutomationPeer::GetPatternCore(winrt::PatternInterface const& patternInterface)
    {
        if (patternInterface == winrt::PatternInterface::Invoke)
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
