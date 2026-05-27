#include "pch.h"
#include "winrt_module_imports.h"
#include "SizerAutomationPeer.h"
#if __has_include("SizerAutomationPeer.g.cpp")
#include "SizerAutomationPeer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    using owner_type = XamlToolkit::WinUI::Controls::SizerBase;

    owner_type SizerAutomationPeer::OwningSizer()
    {
        return Owner().try_as<owner_type>();
    }

    winrt::hstring SizerAutomationPeer::GetClassNameCore()
    {
        return winrt::get_class_name(OwningSizer());
    }

    winrt::hstring SizerAutomationPeer::GetNameCore()
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
