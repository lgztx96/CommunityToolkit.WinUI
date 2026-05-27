#pragma once

#include "SizerAutomationPeer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Automation;
    using namespace Microsoft::UI::Xaml::Automation::Peers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SizerAutomationPeer : SizerAutomationPeerT<SizerAutomationPeer>
    {
        SizerAutomationPeer(XamlToolkit::WinUI::Controls::SizerBase owner)
            : SizerAutomationPeerT<SizerAutomationPeer>(owner)
        {
        }

        XamlToolkit::WinUI::Controls::SizerBase OwningSizer();

        winrt::hstring GetClassNameCore();

        winrt::hstring GetNameCore();
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SizerAutomationPeer : SizerAutomationPeerT<SizerAutomationPeer, implementation::SizerAutomationPeer>
    {
    };
}
