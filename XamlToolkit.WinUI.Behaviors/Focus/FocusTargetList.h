#pragma once

#include "FocusTargetList.g.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// A collection of FocusTarget.
    /// </summary>
    struct FocusTargetList : FocusTargetListT<FocusTargetList>
    {
        FocusTargetList() = default;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct FocusTargetList : FocusTargetListT<FocusTargetList, implementation::FocusTargetList>
    {
    };
}
