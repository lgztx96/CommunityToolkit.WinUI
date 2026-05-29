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
        static XamlToolkit::WinUI::AttachedShadowBase GetShadow(FrameworkElement const& obj)
        {
            return winrt::unbox_value<XamlToolkit::WinUI::AttachedShadowBase>(obj.GetValue(ShadowProperty));
        }

        static void SetShadow(FrameworkElement const& obj, XamlToolkit::WinUI::AttachedShadowBase const& value)
        {
            obj.SetValue(ShadowProperty, value);
        }

    private:

        static void OnShadowChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

    public:
        static inline const wil::single_threaded_property<DependencyProperty> ShadowProperty =
          DependencyProperty::RegisterAttached(L"Shadow",
                winrt::xaml_typename<XamlToolkit::WinUI::AttachedShadowBase>(),
                winrt::xaml_typename<XamlToolkit::WinUI::Effects>(),
                PropertyMetadata(nullptr, &Effects::OnShadowChanged));
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct Effects : EffectsT<Effects, implementation::Effects>
    {
    };
}
