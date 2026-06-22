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
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
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

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty =
            winrt::DependencyProperty::Register(L"Value", winrt::xaml_typename<winrt::IInspectable>(), winrt::xaml_typename<class_type>(), winrt::PropertyMetadata(nullptr, OnValuePropertyChanged));

        winrt::IInspectable To() const { return GetValue(ToProperty); }
        void To(winrt::IInspectable const& value) const { SetValue(ToProperty, value); }

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ToProperty =
            winrt::DependencyProperty::Register(L"To", winrt::xaml_typename<winrt::IInspectable>(), winrt::xaml_typename<class_type>(), winrt::PropertyMetadata(nullptr, OnValuePropertyChanged));
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct IsEqualStateTrigger : IsEqualStateTriggerT<IsEqualStateTrigger, implementation::IsEqualStateTrigger>
    {
    };
}
