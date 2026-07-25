#pragma once

#include "ReverseTransitionAction.g.h"
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
    struct ReverseTransitionAction : ReverseTransitionActionT<ReverseTransitionAction>
    {
        ReverseTransitionAction() = default;

        static const wil::single_threaded_property<winrt::DependencyProperty> TransitionProperty;

        winrt::XamlToolkit::Labs::WinUI::TransitionHelper Transition() const
        {
            return GetValue(TransitionProperty()).try_as<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>();
        }

        void Transition(winrt::XamlToolkit::Labs::WinUI::TransitionHelper const& value)
        {
            SetValue(TransitionProperty(), value);
        }

        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const&) const;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ReverseTransitionAction : ReverseTransitionActionT<ReverseTransitionAction, implementation::ReverseTransitionAction>
    {
    };
}
