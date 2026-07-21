#include "pch.h"
#include "winrt_module_imports.h"
#include "Activity.h"
#if __has_include("Activity.g.cpp")
#include "Activity.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Activity::DelayProperty = 
        winrt::DependencyProperty::Register(
                L"Delay",
                winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr });

    winrt::IReference<winrt::TimeSpan> Activity::Delay() const
    {
        return GetValue(DelayProperty()).try_as<winrt::IReference<winrt::TimeSpan>>();
    }

    void Activity::Delay(winrt::IReference<winrt::TimeSpan> const& value)
    {
        SetValue(DelayProperty(), value);
    }

    winrt::IAsyncAction Activity::InvokeAsync(winrt::UIElement const& element)
    {
        if (auto delay = Delay())
        {
            co_await delay.Value();
		}
    }
}
