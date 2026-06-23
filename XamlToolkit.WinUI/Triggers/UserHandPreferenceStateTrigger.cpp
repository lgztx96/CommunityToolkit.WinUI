#include "pch.h"
#include "winrt_module_imports.h"
#include "UserHandPreferenceStateTrigger.h"
#if __has_include("UserHandPreferenceStateTrigger.g.cpp")
#include "UserHandPreferenceStateTrigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UserHandPreferenceStateTrigger::HandPreferenceProperty =
        winrt::DependencyProperty::Register(
            L"HandPreference",
            winrt::xaml_typename<winrt::HandPreference>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::HandPreference::RightHanded), &UserHandPreferenceStateTrigger::OnHandPreferencePropertyChanged));

    UserHandPreferenceStateTrigger::UserHandPreferenceStateTrigger()
    {
        winrt::UISettings settings;
        _handPreference = settings.HandPreference();
    }

    void UserHandPreferenceStateTrigger::OnHandPreferencePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto obj = d.as<class_type>();
        auto self = winrt::get_self<UserHandPreferenceStateTrigger>(obj);
        auto val = winrt::unbox_value<winrt::HandPreference>(e.NewValue());

        self->SetActive(self->_handPreference == val);
    }
}
