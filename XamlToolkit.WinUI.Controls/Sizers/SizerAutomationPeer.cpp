#include "pch.h"
#include "winrt_module_imports.h"
#include "SizerAutomationPeer.h"
#if __has_include("SizerAutomationPeer.g.cpp")
#include "SizerAutomationPeer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    using owner_type = winrt::XamlToolkit::WinUI::Controls::SizerBase;

    owner_type SizerAutomationPeer::OwningSizer() const
    {
        return Owner().try_as<owner_type>();
    }

    winrt::hstring SizerAutomationPeer::GetClassNameCore() const
    {
        return winrt::get_class_name(OwningSizer());
    }

    winrt::hstring SizerAutomationPeer::GetNameCore() const
    {
        winrt::hstring name = winrt::AutomationProperties::GetName(OwningSizer());
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
