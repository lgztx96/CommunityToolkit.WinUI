#pragma once

#include "ThemeListener.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::ViewManagement;
    using namespace Microsoft::UI::Dispatching;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct ThemeListener : ThemeListenerT<ThemeListener>
    {
        wil::single_threaded_rw_property<winrt::ApplicationTheme> CurrentTheme;

        wil::single_threaded_rw_property<bool> IsHighContrast{ false };

        wil::single_threaded_rw_property<winrt::DispatcherQueue> DispatcherQueue{ nullptr };

        winrt::event<winrt::XamlToolkit::WinUI::Helpers::ThemeChangedHandler> _themeChanged;

        winrt::event_token ThemeChanged(winrt::XamlToolkit::WinUI::Helpers::ThemeChangedHandler const& handler);

        void ThemeChanged(winrt::event_token const& token) noexcept;

        ThemeListener() : ThemeListener(nullptr) {};

        ThemeListener(winrt::DispatcherQueue const& dispatcherQueue);

        void OnThemePropertyChangedAsync();

        winrt::hstring CurrentThemeName();

    private:
        winrt::AccessibilitySettings _accessible;
        winrt::UISettings _settings;

        void Accessible_HighContrastChanged(winrt::AccessibilitySettings const& sender, winrt::IInspectable const& args);

        // Note: This can get called multiple times during HighContrast switch, do we care?
        void Settings_ColorValuesChanged(winrt::UISettings const& sender, winrt::IInspectable const& args);

        void UpdateProperties();
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct ThemeListener : ThemeListenerT<ThemeListener, implementation::ThemeListener>
    {
    };
}
