#include "pch.h"
#include "ThemeListener.h"
#if __has_include("ThemeListener.g.cpp")
#include "ThemeListener.g.cpp"
#endif
#include <winrt/Windows.Foundation.Metadata.h>

namespace winrt::CommunityToolkit::WinUI::Helpers::implementation
{
    using namespace winrt::Windows::Foundation::Metadata;

    ThemeListener::ThemeListener(struct DispatcherQueue const& dispatcherQueue)
    {
        CurrentTheme = Application::Current().RequestedTheme();

        if (ApiInformation::IsPropertyPresent(L"Windows.UI.ViewManagement.AccessibilitySettings", L"HighContrast"))
        {
            IsHighContrast = _accessible.HighContrast();
        }

        DispatcherQueue = dispatcherQueue ? dispatcherQueue : DispatcherQueue::GetForCurrentThread();
    }

    void ThemeListener::Accessible_HighContrastChanged([[maybe_unused]] AccessibilitySettings const& sender, [[maybe_unused]] IInspectable const& args)
    {
        OnThemePropertyChangedAsync();
    }

    // Note: This can get called multiple times during HighContrast switch, do we care?
    void ThemeListener::Settings_ColorValuesChanged([[maybe_unused]] UISettings const& sender, [[maybe_unused]] IInspectable const& args)
    {
        OnThemePropertyChangedAsync();
    }

    void ThemeListener::OnThemePropertyChangedAsync()
    {
        DispatcherQueue().TryEnqueue(DispatcherQueuePriority::Normal, [this]()
        {
            // TODO: This doesn't stop the multiple calls if we're in our faked 'White' HighContrast Mode below.
            if (CurrentTheme != Application::Current().RequestedTheme() || IsHighContrast() != _accessible.HighContrast())
            {
                UpdateProperties();
            }
        });
    }

    void ThemeListener::UpdateProperties()
    {
        static auto contains = [](winrt::hstring scheme, std::wstring_view target) -> bool
        {
            std::wstring str{ scheme.c_str(), scheme.size() };
            std::transform(str.begin(), str.end(), str.begin(), ::towlower);
            return str.find(target) != std::wstring::npos;
        };

        // TODO: Not sure if HighContrastScheme names are localized?
        if (_accessible.HighContrast() && contains(_accessible.HighContrastScheme(), L"white"))
        {
            // If our HighContrastScheme is ON & a lighter one, then we should remain in 'Light' theme mode for Monaco Themes Perspective
            IsHighContrast = false;
            CurrentTheme = ApplicationTheme::Light;
        }
        else
        {
            // Otherwise, we just set to what's in the system as we'd expect.
            IsHighContrast = _accessible.HighContrast();
            CurrentTheme = Application::Current().RequestedTheme();
        }

        ThemeChanged.invoke(*this, nullptr);
    }

    winrt::hstring ThemeListener::CurrentThemeName()
    {
        switch (CurrentTheme)
        {
        case ApplicationTheme::Light:
            return L"Light";
        case ApplicationTheme::Dark:
            return L"Dark";
        default:
            throw winrt::hresult_invalid_argument(L"Unknown ApplicationTheme value");
        }
    }
}
