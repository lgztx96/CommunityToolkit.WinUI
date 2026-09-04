#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> TransitionHelper::IdProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"Id",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> TransitionHelper::IsIndependentProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"IsIndependent",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(false) });

	const wil::single_threaded_property<winrt::DependencyProperty> TransitionHelper::IndependentTranslationProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"IndependentTranslation",
			winrt::xaml_typename<winrt::IReference<winrt::Point>>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> TransitionHelper::CoordinatedTargetProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"CoordinatedTarget",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	winrt::hstring TransitionHelper::GetId(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(IdProperty()), L"");
	}

	void TransitionHelper::SetId(winrt::DependencyObject const& obj, winrt::hstring const& value)
	{
		obj.SetValue(IdProperty(), winrt::box_value(value));
	}

	bool TransitionHelper::GetIsIndependent(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value<bool>(obj.GetValue(IsIndependentProperty()));
	}

	void TransitionHelper::SetIsIndependent(winrt::DependencyObject const& obj, bool value)
	{
		obj.SetValue(IsIndependentProperty(), winrt::box_value(value));
	}

	winrt::IReference<winrt::Point> TransitionHelper::GetIndependentTranslation(winrt::DependencyObject const& obj)
	{
		return obj.GetValue(IndependentTranslationProperty()).try_as<winrt::IReference<winrt::Point>>();
	}

	void TransitionHelper::SetIndependentTranslation(winrt::DependencyObject const& obj, winrt::IReference<winrt::Point> const& value)
	{
		obj.SetValue(IndependentTranslationProperty(), value);
	}

	winrt::hstring TransitionHelper::GetCoordinatedTarget(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(CoordinatedTargetProperty()), L"");
	}

	void TransitionHelper::SetCoordinatedTarget(winrt::DependencyObject const& obj, winrt::hstring const& value)
	{
		obj.SetValue(CoordinatedTargetProperty(), winrt::box_value(value));
	}
}
