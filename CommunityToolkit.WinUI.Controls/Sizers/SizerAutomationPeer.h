#pragma once

#include "SizerAutomationPeer.g.h"
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace winrt::Microsoft::UI::Xaml::Automation;
    using namespace winrt::Microsoft::UI::Xaml::Automation::Peers;

    struct SizerAutomationPeer : SizerAutomationPeerT<SizerAutomationPeer>
    {
        SizerAutomationPeer(CommunityToolkit::WinUI::Controls::SizerBase owner)
            : SizerAutomationPeerT<SizerAutomationPeer>(owner)
        {
        }

        CommunityToolkit::WinUI::Controls::SizerBase OwningSizer();

        hstring GetClassNameCore();

        hstring GetNameCore();
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct SizerAutomationPeer : SizerAutomationPeerT<SizerAutomationPeer, implementation::SizerAutomationPeer>
    {
    };
}
