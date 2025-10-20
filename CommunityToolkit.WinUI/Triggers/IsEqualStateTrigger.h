#pragma once

#include "IsEqualStateTrigger.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt;
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;

    struct IsEqualStateTrigger : IsEqualStateTriggerT<IsEqualStateTrigger>
    {
        IsEqualStateTrigger() = default;

        void UpdateTrigger();

        static void OnValuePropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static bool AreValuesEqual(IInspectable const& value1, IInspectable const& value2, bool convertType);

        static bool ConvertTypeEquals(IInspectable const& value1, IInspectable const& value2);

        IInspectable Value() const { return GetValue(ValueProperty); }
        void Value(IInspectable const& value) const { SetValue(ValueProperty, value); }

        static inline const wil::single_threaded_property<DependencyProperty> ValueProperty =
            DependencyProperty::Register(L"Value", winrt::xaml_typename<IInspectable>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnValuePropertyChanged));

        IInspectable To() const { return GetValue(ToProperty); }
        void To(IInspectable const& value) const { SetValue(ToProperty, value); }

        static inline const wil::single_threaded_property<DependencyProperty> ToProperty =
            DependencyProperty::Register(L"To", winrt::xaml_typename<IInspectable>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnValuePropertyChanged));
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct IsEqualStateTrigger : IsEqualStateTriggerT<IsEqualStateTrigger, implementation::IsEqualStateTrigger>
    {
    };
}
