#pragma once

#include "StartTransitionAction.g.h"
#include "TransitionHelper.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace ::winrt::Microsoft::UI::Xaml;
    using namespace ::winrt::Windows::Foundation;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct StartTransitionAction : StartTransitionActionT<StartTransitionAction>
    {
        StartTransitionAction() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> TransitionProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetProperty;

        winrt::XamlToolkit::Labs::WinUI::TransitionHelper Transition() const
        {
            return GetValue(TransitionProperty()).try_as<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>();
        }

        void Transition(winrt::XamlToolkit::Labs::WinUI::TransitionHelper const& value)
        {
            SetValue(TransitionProperty(), value);
        }

        winrt::FrameworkElement Source() const
        {
            return GetValue(SourceProperty()).try_as<winrt::FrameworkElement>();
        }

        void Source(winrt::FrameworkElement const& value)
        {
            SetValue(SourceProperty(), value);
        }

        winrt::FrameworkElement Target() const
        {
            return GetValue(TargetProperty()).try_as<winrt::FrameworkElement>();
        }

        void Target(winrt::FrameworkElement const& value)
        {
            SetValue(TargetProperty(), value);
        }

        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter) const;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct StartTransitionAction : StartTransitionActionT<StartTransitionAction, implementation::StartTransitionAction>
    {
    };
}
