#include "pch.h"
#include "winrt_module_imports.h"
#include "ReverseTransitionAction.h"
#include "TransitionHelper.h"
#if __has_include("ReverseTransitionAction.g.cpp")
#include "ReverseTransitionAction.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<DependencyProperty> ReverseTransitionAction::TransitionProperty = DependencyProperty::Register(L"Transition", xaml_typename<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>(), xaml_typename<class_type>(), nullptr);
	Windows::Foundation::IInspectable ReverseTransitionAction::Execute(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&)
	{
		if (!Transition()) throw hresult_invalid_argument(L"Transition is required."); Transition().ReverseAsync(); return nullptr;
	}
}
