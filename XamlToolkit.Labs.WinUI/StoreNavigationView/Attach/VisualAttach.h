#pragma once

#include "VisualAttach.g.h"

#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct VisualAttach : VisualAttachT<VisualAttach>
    {
        VisualAttach() = default;

        static void OnNormalizedCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnIsBindCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnScaleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<winrt::DependencyProperty> IsBindCenterPointProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"IsBindCenterPoint",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(false), VisualAttach::OnIsBindCenterPointChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NormalizedCenterPointProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"NormalizedCenterPoint",
                winrt::xaml_typename<hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr, &VisualAttach::OnNormalizedCenterPointChanged));

        static inline const wil::single_threaded_property<DependencyProperty> ScaleProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"Scale",
                winrt::xaml_typename<hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr, &VisualAttach::OnScaleChanged));

        [[nodiscard]] static bool GetIsBindCenterPoint(winrt::DependencyObject const& obj) noexcept;
        static void SetIsBindCenterPoint(winrt::DependencyObject const& obj, bool value);

        [[nodiscard]] static hstring GetNormalizedCenterPoint(winrt::DependencyObject const& obj) noexcept;
        static void SetNormalizedCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value);

        [[nodiscard]] static hstring GetScale(winrt::DependencyObject const& obj) noexcept;
        static void SetScale(winrt::DependencyObject const& obj, winrt::hstring const& value);

        [[nodiscard]] static hstring GetScaleForElement(winrt::UIElement const& element) noexcept;
        static void SetScaleForElement(winrt::hstring const& value, winrt::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct VisualAttach : VisualAttachT<VisualAttach, implementation::VisualAttach>
    {};
}
