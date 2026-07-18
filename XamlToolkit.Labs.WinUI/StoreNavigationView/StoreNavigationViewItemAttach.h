#pragma once

#include "StoreNavigationViewItemAttach.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach>
    {
        StoreNavigationViewItemAttach() = default;

        static inline wil::single_threaded_property<DependencyProperty> SelectedIconProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"SelectedIcon",
                winrt::xaml_typename<IInspectable>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr));

        static inline wil::single_threaded_property<DependencyProperty> ShowNotificationDotProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"ShowNotificationDot",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(false)));

        static inline wil::single_threaded_property<DependencyProperty> UnselectedIconProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"UnselectedIcon",
                winrt::xaml_typename<IInspectable>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr));

        static inline wil::single_threaded_property<DependencyProperty> StaticIconVisibilityProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"StaticIconVisibility",
                winrt::xaml_typename<Visibility>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(box_value(Visibility::Collapsed)));

        [[nodiscard]] static winrt::IInspectable GetSelectedIcon(winrt::DependencyObject const& obj);
        static void SetSelectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value);

        [[nodiscard]] static bool GetShowNotificationDot(winrt::DependencyObject const& obj);
        static void SetShowNotificationDot(winrt::DependencyObject const& obj, bool value);

        [[nodiscard]] static winrt::IInspectable GetUnselectedIcon(winrt::DependencyObject const& obj);
        static void SetUnselectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value);

        [[nodiscard]] static winrt::Visibility GetStaticIconVisibility(DependencyObject const& obj);
        static void SetStaticIconVisibility(winrt::DependencyObject const& obj, winrt::Visibility const& value);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach, implementation::StoreNavigationViewItemAttach>
    {};
}
