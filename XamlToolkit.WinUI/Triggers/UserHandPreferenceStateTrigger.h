#pragma once

#include "UserHandPreferenceStateTrigger.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::ViewManagement;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct UserHandPreferenceStateTrigger : UserHandPreferenceStateTriggerT<UserHandPreferenceStateTrigger>
    {
        UserHandPreferenceStateTrigger();

        winrt::HandPreference HandPreference() const { return winrt::unbox_value<winrt::HandPreference>(GetValue(HandPreferenceProperty)); }
        void HandPreference(winrt::HandPreference const& value) const { SetValue(HandPreferenceProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> HandPreferenceProperty;

    private:
        static void OnHandPreferencePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        winrt::HandPreference _handPreference;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct UserHandPreferenceStateTrigger : UserHandPreferenceStateTriggerT<UserHandPreferenceStateTrigger, implementation::UserHandPreferenceStateTrigger>
    {
    };
}
