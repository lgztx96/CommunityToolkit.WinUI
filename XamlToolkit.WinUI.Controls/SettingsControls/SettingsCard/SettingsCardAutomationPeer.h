#pragma once

#include "SettingsCardAutomationPeer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Automation;
	using namespace Microsoft::UI::Xaml::Automation::Peers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SettingsCardAutomationPeer : SettingsCardAutomationPeerT<SettingsCardAutomationPeer>
    {
        using owner_type = winrt::XamlToolkit::WinUI::Controls::SettingsCard;

        SettingsCardAutomationPeer(owner_type const& owner);

        winrt::AutomationControlType GetAutomationControlTypeCore();

        winrt::hstring GetClassNameCore();

        winrt::hstring GetNameCore();

        winrt::IInspectable GetPatternCore(winrt::PatternInterface const& patternInterface);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsCardAutomationPeer : SettingsCardAutomationPeerT<SettingsCardAutomationPeer, implementation::SettingsCardAutomationPeer>
    {
    };
}
