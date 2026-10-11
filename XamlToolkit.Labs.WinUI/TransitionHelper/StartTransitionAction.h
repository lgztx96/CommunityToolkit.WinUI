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
            return _source.get();
        }

        void Source(winrt::FrameworkElement const& value)
        {
            _source = value;
        }

        winrt::FrameworkElement Target() const
        {
            return _target.get();
        }

        void Target(winrt::FrameworkElement const& value)
        {
            _target = value;
        }

        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter) const;

    private:
        winrt::weak_ref<winrt::FrameworkElement> _source{ nullptr };
        winrt::weak_ref<winrt::FrameworkElement> _target{ nullptr };
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct StartTransitionAction : StartTransitionActionT<StartTransitionAction, implementation::StartTransitionAction>
    {
    };
}
