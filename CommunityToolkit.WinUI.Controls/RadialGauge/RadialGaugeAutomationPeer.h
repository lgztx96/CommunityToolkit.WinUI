#pragma once

#include "RadialGaugeAutomationPeer.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using owner_type = winrt::CommunityToolkit::WinUI::Controls::RadialGauge;

    struct RadialGaugeAutomationPeer : RadialGaugeAutomationPeerT<RadialGaugeAutomationPeer>
    {
        RadialGaugeAutomationPeer(owner_type const& owner)
            : RadialGaugeAutomationPeerT<RadialGaugeAutomationPeer>(owner)
        {
        }

        bool IsReadOnly() { return !Owner().as<owner_type>().IsInteractive(); }

        double LargeChange() { return Owner().as<owner_type>().StepSize(); }

        double Maximum() { return Owner().as<owner_type>().Maximum(); }

        double Minimum() { return Owner().as<owner_type>().Minimum(); }

        double SmallChange() { return Owner().as<owner_type>().StepSize(); }

        double Value() { return Owner().as<owner_type>().Value(); }

        void SetValue(double value);

        IVector<AutomationPeer> GetChildrenCore();

        winrt::hstring GetNameCore();

        IInspectable GetPatternCore(PatternInterface patternInterface);

        AutomationControlType GetAutomationControlTypeCore();

        void RaiseValueChangedEvent(double oldValue, double newValue);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct RadialGaugeAutomationPeer : RadialGaugeAutomationPeerT<RadialGaugeAutomationPeer, implementation::RadialGaugeAutomationPeer>
    {
    };
}
