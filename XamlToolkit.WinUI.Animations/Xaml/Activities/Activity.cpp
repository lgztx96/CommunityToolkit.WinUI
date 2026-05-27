#include "pch.h"
#include "winrt_module_imports.h"
#include "Activity.h"
#if __has_include("Activity.g.cpp")
#include "Activity.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> Activity::DelayProperty = 
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Delay",
                winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

    winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> Activity::Delay()
    {
        return GetValue(DelayProperty()).try_as<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>();
    }

    void Activity::Delay(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const& value)
    {
        SetValue(DelayProperty(), value);
    }

    winrt::Windows::Foundation::IAsyncAction Activity::InvokeAsync(Microsoft::UI::Xaml::UIElement const& element)
    {
        if (auto delay = Delay())
        {
            co_await delay.Value();
		}
    }
}
