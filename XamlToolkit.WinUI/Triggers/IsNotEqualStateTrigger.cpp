#include "pch.h"
#include "winrt_module_imports.h"
#include "IsNotEqualStateTrigger.h"
#include "IsEqualStateTrigger.h"
#if __has_include("IsNotEqualStateTrigger.g.cpp")
#include "IsNotEqualStateTrigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> IsNotEqualStateTrigger::ValueProperty =
		winrt::DependencyProperty::Register(
			L"Value",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IsNotEqualStateTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> IsNotEqualStateTrigger::ToProperty =
		winrt::DependencyProperty::Register(
			L"To",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IsNotEqualStateTrigger::OnValuePropertyChanged));

	void IsNotEqualStateTrigger::UpdateTrigger()
	{
		SetActive(!IsEqualStateTrigger::AreValuesEqual(Value(), To(), true));
	}

	void IsNotEqualStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		winrt::get_self<IsNotEqualStateTrigger>(d.as<class_type>())->UpdateTrigger();
	}
}
