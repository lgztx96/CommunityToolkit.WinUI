#pragma once

#include "IsNullOrEmptyStateTrigger.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;

    struct IsNullOrEmptyStateTrigger : IsNullOrEmptyStateTriggerT<IsNullOrEmptyStateTrigger>
    {
        IsNullOrEmptyStateTrigger();

        IInspectable Value() const { return GetValue(ValueProperty); }
        void Value(IInspectable const& value) const { SetValue(ValueProperty, value); }

        static const wil::single_threaded_property<DependencyProperty> ValueProperty;

    private:

        static void OnValuePropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        void UpdateTrigger();

        static bool IsNullOrEmpty(IInspectable const& val);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct IsNullOrEmptyStateTrigger : IsNullOrEmptyStateTriggerT<IsNullOrEmptyStateTrigger, implementation::IsNullOrEmptyStateTrigger>
    {
    };
}
