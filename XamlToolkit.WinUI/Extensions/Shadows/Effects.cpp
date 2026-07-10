#include "pch.h"
#include "winrt_module_imports.h"
#include "Effects.h"
#if __has_include("Effects.g.cpp")
#include "Effects.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Effects::ShadowProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Shadow",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::AttachedShadowBase>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Effects>(),
            winrt::PropertyMetadata(nullptr, &Effects::OnShadowChanged));

    void Effects::OnShadowChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<winrt::FrameworkElement>())
        {
            if (auto oldShadow = e.OldValue().try_as<winrt::XamlToolkit::WinUI::AttachedShadowBase>())
            {
                oldShadow.DisconnectElement(element);
            }

            if (auto newShadow = e.NewValue().try_as<winrt::XamlToolkit::WinUI::AttachedShadowBase>())
            {
                newShadow.ConnectElement(element);
            }
        }
    }
}
