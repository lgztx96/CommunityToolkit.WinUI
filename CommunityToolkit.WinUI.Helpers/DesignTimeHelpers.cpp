#include "pch.h"
#include "DesignTimeHelpers.h"
#if __has_include("DesignTimeHelpers.g.cpp")
#include "DesignTimeHelpers.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Helpers::implementation
{
    bool DesignTimeHelpers::IsRunningInLegacyDesignerMode()
    {
        static bool legacy = winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled() &&
                             !winrt::Windows::ApplicationModel::DesignMode::DesignMode2Enabled();
        return legacy;
    }

    bool DesignTimeHelpers::IsRunningInEnhancedDesignerMode()
    {
        static bool enhanced = winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled() &&
                               winrt::Windows::ApplicationModel::DesignMode::DesignMode2Enabled();
        return enhanced;
    }

    bool DesignTimeHelpers::IsRunningInApplicationRuntimeMode()
    {
        static bool runtime = !winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled();
        return runtime;
    }
}
