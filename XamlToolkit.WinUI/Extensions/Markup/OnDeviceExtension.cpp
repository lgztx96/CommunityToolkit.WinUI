#include "pch.h"
#include "winrt_module_imports.h"
#include "OnDeviceExtension.h"
#if __has_include("OnDeviceExtension.g.cpp")
#include "OnDeviceExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::IInspectable OnDeviceExtension::ProvideValue() const
    {
        auto deviceFamily = winrt::Windows::System::Profile::AnalyticsInfo::VersionInfo().DeviceFamily();

        if (deviceFamily == L"Windows.Desktop")
        {
            return Desktop() ? Desktop() : Default();
        }
        else if (deviceFamily == L"Windows.Holographic")
        {
            return Holographic() ? Holographic() : Default();
        }
        else if (deviceFamily == L"Windows.IoT")
        {
            return IoT() ? IoT() : Default();
        }
        else if (deviceFamily == L"Windows.Team")
        {
            return Team() ? Team() : Default();
        }
        else if (deviceFamily == L"Windows.Xbox")
        {
            return Xbox() ? Xbox() : Default();
        }

        return Default();
    }

    winrt::IInspectable OnDeviceExtension::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider) const
    {
        return ProvideValue();
    }
}
