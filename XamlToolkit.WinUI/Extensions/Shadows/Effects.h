#pragma once

#include "Effects.g.h"

#include "AttachedShadowBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct Effects
    {
        static XamlToolkit::WinUI::AttachedShadowBase GetShadow(winrt::FrameworkElement const& obj)
        {
            return obj.GetValue(ShadowProperty()).try_as<winrt::XamlToolkit::WinUI::AttachedShadowBase>();
        }

        static void SetShadow(winrt::FrameworkElement const& obj, winrt::XamlToolkit::WinUI::AttachedShadowBase const& value)
        {
            obj.SetValue(ShadowProperty(), value);
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ShadowProperty;

    private:
        static void OnShadowChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct Effects : EffectsT<Effects, implementation::Effects>
    {
    };
}
