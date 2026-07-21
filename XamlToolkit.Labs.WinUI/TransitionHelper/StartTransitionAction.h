#pragma once

#include "StartTransitionAction.g.h"
#include "TransitionHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct StartTransitionAction : StartTransitionActionT<StartTransitionAction>
	{
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TransitionProperty;
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SourceProperty;
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TargetProperty;

		winrt::XamlToolkit::Labs::WinUI::TransitionHelper Transition() const
		{
			return GetValue(TransitionProperty()).try_as<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>();
		}
		void Transition(winrt::XamlToolkit::Labs::WinUI::TransitionHelper const& value)
		{
			SetValue(TransitionProperty(), value);
		}

		winrt::Microsoft::UI::Xaml::FrameworkElement Source() const
		{
			return GetValue(SourceProperty()).try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
		}
		void Source(winrt::Microsoft::UI::Xaml::FrameworkElement const& value)
		{
			SetValue(SourceProperty(), value);
		}

		winrt::Microsoft::UI::Xaml::FrameworkElement Target() const
		{
			return GetValue(TargetProperty()).try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
		}
		void Target(winrt::Microsoft::UI::Xaml::FrameworkElement const& value)
		{
			SetValue(TargetProperty(), value);
		}

		winrt::Windows::Foundation::IInspectable Execute(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Foundation::IInspectable const&);
	};
}
namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct StartTransitionAction : StartTransitionActionT<StartTransitionAction, implementation::StartTransitionAction>
	{
	};
}
