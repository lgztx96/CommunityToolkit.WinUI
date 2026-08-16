#pragma once

#include "IsNotEqualStateTrigger.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct IsNotEqualStateTrigger : IsNotEqualStateTriggerT<IsNotEqualStateTrigger>
    {
        IsNotEqualStateTrigger() = default;

        void UpdateTrigger();

        winrt::IInspectable Value() const { return GetValue(ValueProperty()); }
        void Value(winrt::IInspectable const& value) const { SetValue(ValueProperty(), value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

        winrt::IInspectable To() const { return GetValue(ToProperty()); }
        void To(winrt::IInspectable const& value) const { SetValue(ToProperty(), value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ToProperty;

    private:
        static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct IsNotEqualStateTrigger : IsNotEqualStateTriggerT<IsNotEqualStateTrigger, implementation::IsNotEqualStateTrigger>
    {
    };
}
