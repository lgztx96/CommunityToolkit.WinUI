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
        SizerAutomationPeer(winrt::XamlToolkit::WinUI::Controls::SizerBase const& owner)
            : SizerAutomationPeerT<SizerAutomationPeer>(owner)
        {
        }

        winrt::XamlToolkit::WinUI::Controls::SizerBase OwningSizer() const;

        winrt::hstring GetClassNameCore() const;

        winrt::hstring GetNameCore() const;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SizerAutomationPeer : SizerAutomationPeerT<SizerAutomationPeer, implementation::SizerAutomationPeer>
    {
    };
}
