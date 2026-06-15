#pragma once

#include "DesignTimeHelpers.g.h"

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct DesignTimeHelpers
    {
        static bool IsRunningInLegacyDesignerMode();

        static bool IsRunningInEnhancedDesignerMode();

        static bool IsRunningInApplicationRuntimeMode();
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct DesignTimeHelpers : DesignTimeHelpersT<DesignTimeHelpers, implementation::DesignTimeHelpers>
    {
    };
}
