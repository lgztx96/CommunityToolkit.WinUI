#include "pch.h"
#include "winrt_module_imports.h"
#include "FocusTarget.h"
#if __has_include("FocusTarget.g.cpp")
#include "FocusTarget.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> FocusTarget::ControlProperty =
        winrt::DependencyProperty::Register(
            L"Control",
            winrt::xaml_typename<winrt::Control>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &FocusTarget::OnControlChanged));

    void FocusTarget::OnControlChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto target = d.try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusTarget>())
        {
            if (auto impl = winrt::get_self<FocusTarget>(target))
            {
                impl->ControlChanged.invoke(target, nullptr);
            }
        }
    }
}
