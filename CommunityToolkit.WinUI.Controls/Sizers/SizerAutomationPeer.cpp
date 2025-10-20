#include "pch.h"
#include "SizerAutomationPeer.h"
#if __has_include("SizerAutomationPeer.g.cpp")
#include "SizerAutomationPeer.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using owner_type = CommunityToolkit::WinUI::Controls::SizerBase;

    owner_type SizerAutomationPeer::OwningSizer()
    {
        return Owner().try_as<owner_type>();
    }

    hstring SizerAutomationPeer::GetClassNameCore()
    {
        return winrt::get_class_name(OwningSizer());
    }

    hstring SizerAutomationPeer::GetNameCore()
    {
        hstring name = AutomationProperties::GetName(OwningSizer());
        if (!name.empty())
        {
            return name;
        }

        name = winrt::get_class_name(OwningSizer());
        if (!name.empty())
        {
            return name;
        }

        name = base_type::GetNameCore();
        if (!name.empty())
        {
            return name;
        }

        return {};
    }
}
