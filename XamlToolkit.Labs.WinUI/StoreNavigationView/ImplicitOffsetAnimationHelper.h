#pragma once

#include "ImplicitOffsetAnimationHelper.g.h"

#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ImplicitOffsetAnimationHelper : ImplicitOffsetAnimationHelperT<ImplicitOffsetAnimationHelper>
    {
        ImplicitOffsetAnimationHelper() = default;

        static void OnEnableOffsetAnimationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> EnableOffsetAnimationProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"EnableOffsetAnimation",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr, &ImplicitOffsetAnimationHelper::OnEnableOffsetAnimationChanged));

        [[nodiscard]] static bool GetEnableOffsetAnimation(winrt::UIElement const& element) noexcept;
        static void SetEnableOffsetAnimation(winrt::UIElement const& element, bool value);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ImplicitOffsetAnimationHelper : ImplicitOffsetAnimationHelperT<ImplicitOffsetAnimationHelper, implementation::ImplicitOffsetAnimationHelper>
    {};
}
