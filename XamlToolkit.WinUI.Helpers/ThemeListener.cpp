#include "pch.h"
#include "winrt_module_imports.h"
#include "ThemeListener.h"
#if __has_include("ThemeListener.g.cpp")
#include "ThemeListener.g.cpp"
#endif

namespace winrt
{
    using namespace Windows::Foundation::Metadata;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    ThemeListener::ThemeListener(winrt::DispatcherQueue const& dispatcherQueue)
    {
        CurrentTheme = winrt::Application::Current().RequestedTheme();

        if (winrt::ApiInformation::IsPropertyPresent(L"Windows.UI.ViewManagement.AccessibilitySettings", L"HighContrast"))
        {
            IsHighContrast = _accessible.HighContrast();
        }

        DispatcherQueue = dispatcherQueue ? dispatcherQueue : winrt::DispatcherQueue::GetForCurrentThread();
    }

    void ThemeListener::Accessible_HighContrastChanged([[maybe_unused]] winrt::AccessibilitySettings const& sender, [[maybe_unused]] winrt::IInspectable const& args)
    {
        OnThemePropertyChangedAsync();
    }

    // Note: This can get called multiple times during HighContrast switch, do we care?
    void ThemeListener::Settings_ColorValuesChanged([[maybe_unused]] winrt::UISettings const& sender, [[maybe_unused]] winrt::IInspectable const& args)
    {
        OnThemePropertyChangedAsync();
    }

    void ThemeListener::OnThemePropertyChangedAsync()
    {
        DispatcherQueue().TryEnqueue(winrt::DispatcherQueuePriority::Normal, [weakThis = get_weak()]()
        {
            if (auto self = weakThis.get())
            {
                // TODO: This doesn't stop the multiple calls if we're in our faked 'White' HighContrast Mode below.
                if (self->CurrentTheme != winrt::Application::Current().RequestedTheme() || self->IsHighContrast() != self->_accessible.HighContrast())
                {
                    self->UpdateProperties();
                }
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
            CurrentTheme = winrt::ApplicationTheme::Light;
        }
        else
        {
            // Otherwise, we just set to what's in the system as we'd expect.
            IsHighContrast = _accessible.HighContrast();
            CurrentTheme = winrt::Application::Current().RequestedTheme();
        }

        _themeChanged(*this);
    }

    winrt::hstring ThemeListener::CurrentThemeName()
    {
        switch (CurrentTheme)
        {
        case winrt::ApplicationTheme::Light:
            return L"Light";
        case winrt::ApplicationTheme::Dark:
            return L"Dark";
        default:
            throw winrt::hresult_invalid_argument(L"Unknown ApplicationTheme value");
        }
    }

    winrt::event_token ThemeListener::ThemeChanged(winrt::XamlToolkit::WinUI::Helpers::ThemeChangedHandler const& handler)
    {
        return _themeChanged.add(handler);
    }

    void ThemeListener::ThemeChanged(winrt::event_token const& token) noexcept
    {
        _themeChanged.remove(token);
    }
}
