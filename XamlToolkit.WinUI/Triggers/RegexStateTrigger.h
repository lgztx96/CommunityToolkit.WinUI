#pragma once

#include "RegexStateTrigger.g.h"

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
    struct RegexStateTrigger : RegexStateTriggerT<RegexStateTrigger>
    {
        RegexStateTrigger();

        winrt::hstring Value() const { return winrt::unbox_value<winrt::hstring>(GetValue(ValueProperty)); }
        void Value(winrt::hstring const& value) const { SetValue(ValueProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

        winrt::hstring Expression() const { return winrt::unbox_value<winrt::hstring>(GetValue(ExpressionProperty)); }
        void Expression(winrt::hstring const& value) const { SetValue(ExpressionProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ExpressionProperty;

        winrt::XamlToolkit::WinUI::RegexOptions Options() const { return winrt::unbox_value<winrt::XamlToolkit::WinUI::RegexOptions>(GetValue(OptionsProperty)); }
        void Options(winrt::XamlToolkit::WinUI::RegexOptions const& value) const { SetValue(OptionsProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> OptionsProperty;

    private:
        static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        void UpdateTrigger();

        static std::regex_constants::syntax_option_type ToStdRegexOptions(winrt::XamlToolkit::WinUI::RegexOptions options);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct RegexStateTrigger : RegexStateTriggerT<RegexStateTrigger, implementation::RegexStateTrigger>
    {
    };
}
