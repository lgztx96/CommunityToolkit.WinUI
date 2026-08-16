#include "pch.h"
#include "winrt_module_imports.h"
#include "IsEqualStateTrigger.h"
#if __has_include("IsEqualStateTrigger.g.cpp")
#include "IsEqualStateTrigger.g.cpp"
#endif

#include "../Extensions/Foundation/EqualsHelper.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> IsEqualStateTrigger::ValueProperty =
		winrt::DependencyProperty::Register(
			L"Value",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IsEqualStateTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> IsEqualStateTrigger::ToProperty =
		winrt::DependencyProperty::Register(
			L"To",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IsEqualStateTrigger::OnValuePropertyChanged));

	void IsEqualStateTrigger::UpdateTrigger()
	{
		SetActive(AreValuesEqual(Value(), To(), true));
	}

	void IsEqualStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		winrt::get_self<IsEqualStateTrigger>(d.as<class_type>())->UpdateTrigger();
	}

	bool IsEqualStateTrigger::AreValuesEqual(winrt::IInspectable const& value1, winrt::IInspectable const& value2, bool convertType)
	{
		if (convertType)
		{
			return EqualsHelper::ConvertibleEquals(value1, value2);
		}

		return EqualsHelper::AreEqual(value1, value2);
	}
}
