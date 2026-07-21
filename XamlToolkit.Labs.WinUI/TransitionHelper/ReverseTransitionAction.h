#pragma once

#include "ReverseTransitionAction.g.h"
#include "TransitionHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct ReverseTransitionAction : ReverseTransitionActionT<ReverseTransitionAction>
	{
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TransitionProperty;
		winrt::XamlToolkit::Labs::WinUI::TransitionHelper Transition() const
		{
			return GetValue(TransitionProperty()).try_as<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>();
		}
		void Transition(winrt::XamlToolkit::Labs::WinUI::TransitionHelper const& value)
		{
			SetValue(TransitionProperty(), value);
		}

		winrt::Windows::Foundation::IInspectable Execute(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Foundation::IInspectable const&);
	};
}
namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct ReverseTransitionAction : ReverseTransitionActionT<ReverseTransitionAction, implementation::ReverseTransitionAction>
	{
	};
}
