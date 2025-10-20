#pragma once

#include "SettingsCardAutomationPeer.g.h"
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace winrt::Microsoft::UI::Xaml::Automation;
    using namespace winrt::Microsoft::UI::Xaml::Automation::Peers;

    struct SettingsCardAutomationPeer : SettingsCardAutomationPeerT<SettingsCardAutomationPeer>
    {
        using owner_type = CommunityToolkit::WinUI::Controls::SettingsCard;

        SettingsCardAutomationPeer(owner_type const& owner);

        AutomationControlType GetAutomationControlTypeCore();

        winrt::hstring GetClassNameCore();

        winrt::hstring GetNameCore();

        Windows::Foundation::IInspectable GetPatternCore(PatternInterface const& patternInterface);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsCardAutomationPeer : SettingsCardAutomationPeerT<SettingsCardAutomationPeer, implementation::SettingsCardAutomationPeer>
    {
    };
}
