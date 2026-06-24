#pragma once

#include "SettingsExpanderAutomationPeer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml::Automation;
    using namespace Microsoft::UI::Xaml::Automation::Peers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SettingsExpanderAutomationPeer : SettingsExpanderAutomationPeerT<SettingsExpanderAutomationPeer>
    {
        using owner_type = winrt::XamlToolkit::WinUI::Controls::SettingsExpander;

        SettingsExpanderAutomationPeer() = default;

        SettingsExpanderAutomationPeer(owner_type const& owner);

        winrt::AutomationControlType GetAutomationControlTypeCore();

        winrt::hstring GetClassNameCore();

        winrt::hstring GetNameCore();

        void RaiseExpandedChangedEvent(bool newValue);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsExpanderAutomationPeer : SettingsExpanderAutomationPeerT<SettingsExpanderAutomationPeer, implementation::SettingsExpanderAutomationPeer>
    {
    };
}
