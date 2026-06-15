#include "pch.h"
#include "winrt_module_imports.h"
#include "DesignTimeHelpers.h"
#if __has_include("DesignTimeHelpers.g.cpp")
#include "DesignTimeHelpers.g.cpp"
#endif

namespace winrt
{
	using namespace Windows::ApplicationModel;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    bool DesignTimeHelpers::IsRunningInLegacyDesignerMode()
    {
        static bool legacy = winrt::DesignMode::DesignModeEnabled() && !winrt::DesignMode::DesignMode2Enabled();
        return legacy;
    }

    bool DesignTimeHelpers::IsRunningInEnhancedDesignerMode()
    {
        static bool enhanced = winrt::DesignMode::DesignModeEnabled() && winrt::DesignMode::DesignMode2Enabled();
        return enhanced;
    }

    bool DesignTimeHelpers::IsRunningInApplicationRuntimeMode()
    {
        static bool runtime = !winrt::DesignMode::DesignModeEnabled();
        return runtime;
    }
}
