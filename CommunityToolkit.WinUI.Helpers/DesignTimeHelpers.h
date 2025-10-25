#pragma once

#include "DesignTimeHelpers.g.h"

namespace winrt::CommunityToolkit::WinUI::Helpers::implementation
{
    struct DesignTimeHelpers : DesignTimeHelpersT<DesignTimeHelpers>
    {
        static bool IsRunningInLegacyDesignerMode();

        static bool IsRunningInEnhancedDesignerMode();

        static bool IsRunningInApplicationRuntimeMode();
    };
}

namespace winrt::CommunityToolkit::WinUI::Helpers::factory_implementation
{
    struct DesignTimeHelpers : DesignTimeHelpersT<DesignTimeHelpers, implementation::DesignTimeHelpers>
    {
    };
}
