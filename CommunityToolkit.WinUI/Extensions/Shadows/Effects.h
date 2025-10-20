#pragma once

#include "Effects.g.h"

#include "AttachedShadowBase.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;

    struct Effects
    {
        static CommunityToolkit::WinUI::AttachedShadowBase GetShadow(FrameworkElement const& obj)
        {
            return winrt::unbox_value<CommunityToolkit::WinUI::AttachedShadowBase>(obj.GetValue(ShadowProperty));
        }

        static void SetShadow(FrameworkElement const& obj, CommunityToolkit::WinUI::AttachedShadowBase const& value)
        {
            obj.SetValue(ShadowProperty, value);
        }

    private:

        static void OnShadowChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

    public:
        static inline const wil::single_threaded_property<DependencyProperty> ShadowProperty =
          DependencyProperty::RegisterAttached(L"Shadow",
                winrt::xaml_typename<CommunityToolkit::WinUI::AttachedShadowBase>(),
                winrt::xaml_typename<CommunityToolkit::WinUI::Effects>(),
                PropertyMetadata(nullptr, &Effects::OnShadowChanged));
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct Effects : EffectsT<Effects, implementation::Effects>
    {
    };
}
