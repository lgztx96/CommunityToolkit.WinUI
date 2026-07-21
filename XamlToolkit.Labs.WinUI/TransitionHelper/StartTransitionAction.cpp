#include "pch.h"
#include "winrt_module_imports.h"
#include "StartTransitionAction.h"
#include "TransitionHelper.h"
#if __has_include("StartTransitionAction.g.cpp")
#include "StartTransitionAction.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<DependencyProperty> StartTransitionAction::TransitionProperty = DependencyProperty::Register(L"Transition", xaml_typename<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>(), xaml_typename<class_type>(), nullptr);
	const wil::single_threaded_property<DependencyProperty> StartTransitionAction::SourceProperty = DependencyProperty::Register(L"Source", xaml_typename<FrameworkElement>(), xaml_typename<class_type>(), nullptr);
	const wil::single_threaded_property<DependencyProperty> StartTransitionAction::TargetProperty = DependencyProperty::Register(L"Target", xaml_typename<FrameworkElement>(), xaml_typename<class_type>(), nullptr);

	Windows::Foundation::IInspectable StartTransitionAction::Execute(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&)
	{
		if (!Transition() || !Source() || !Target()) throw hresult_invalid_argument(L"Transition, Source and Target are required."); Transition().Source(Source()); Transition().Target(Target()); Transition().StartAsync(); return nullptr;
	}
}
