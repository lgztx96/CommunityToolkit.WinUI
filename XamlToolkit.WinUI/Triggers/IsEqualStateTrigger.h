#pragma once

#include "IsEqualStateTrigger.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct IsEqualStateTrigger : IsEqualStateTriggerT<IsEqualStateTrigger>
    {
        IsEqualStateTrigger() = default;

        void UpdateTrigger();

        static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static bool AreValuesEqual(winrt::IInspectable const& value1, winrt::IInspectable const& value2, bool convertType);

        static bool ConvertTypeEquals(winrt::IInspectable const& value1, winrt::IInspectable const& value2);

        winrt::IInspectable Value() const { return GetValue(ValueProperty); }
        void Value(winrt::IInspectable const& value) const { SetValue(ValueProperty, value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

        winrt::IInspectable To() const { return GetValue(ToProperty); }
        void To(winrt::IInspectable const& value) const { SetValue(ToProperty, value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ToProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct IsEqualStateTrigger : IsEqualStateTriggerT<IsEqualStateTrigger, implementation::IsEqualStateTrigger>
    {
    };
}
