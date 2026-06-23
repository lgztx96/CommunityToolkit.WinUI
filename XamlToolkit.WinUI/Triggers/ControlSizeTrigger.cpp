#include "pch.h"
#include "winrt_module_imports.h"
#include "ControlSizeTrigger.h"
#if __has_include("ControlSizeTrigger.g.cpp")
#include "ControlSizeTrigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::CanTriggerProperty =
		winrt::DependencyProperty::Register(
			L"CanTrigger",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true), &ControlSizeTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::MaxWidthProperty =
		winrt::DependencyProperty::Register(
			L"MaxWidth",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(std::numeric_limits<double>::infinity()), &ControlSizeTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::MinWidthProperty =
		winrt::DependencyProperty::Register(
			L"MinWidth",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(0.0), &ControlSizeTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::MaxHeightProperty =
		winrt::DependencyProperty::Register(
			L"MaxHeight",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(std::numeric_limits<double>::infinity()), &ControlSizeTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::MinHeightProperty =
		winrt::DependencyProperty::Register(
			L"MinHeight",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(0.0), &ControlSizeTrigger::OnValuePropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> ControlSizeTrigger::TargetElementProperty =
		winrt::DependencyProperty::Register(
			L"TargetElement",
			winrt::xaml_typename<winrt::FrameworkElement>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &ControlSizeTrigger::OnTargetElementPropertyChanged));

	void ControlSizeTrigger::OnTargetElementPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto self = winrt::get_self<ControlSizeTrigger>(d.as<class_type>())->get_strong();
		self->UpdateTargetElement(e.OldValue().as<winrt::FrameworkElement>(), e.NewValue().as<winrt::FrameworkElement>());
	}

	void ControlSizeTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto self = winrt::get_self<ControlSizeTrigger>(d.as<class_type>())->get_strong();
		self->UpdateTrigger();
	}

	// Handle event to get current values
	void ControlSizeTrigger::OnTargetElementSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
	{
		UpdateTrigger();
	}

	void ControlSizeTrigger::UpdateTargetElement(winrt::FrameworkElement const& oldValue, winrt::FrameworkElement const& newValue)
	{
		if (oldValue)
		{
			_sizeChangedRevoker.revoke();
		}

		if (newValue)
		{
			_sizeChangedRevoker = newValue.SizeChanged(winrt::auto_revoke, { this, &ControlSizeTrigger::OnTargetElementSizeChanged });
		}

		UpdateTrigger();
	}

	// Logic to evaluate and apply trigger value
	void ControlSizeTrigger::UpdateTrigger()
	{
		auto targetElement = TargetElement();
		if (targetElement == nullptr || !CanTrigger())
		{
			SetActive(false);
			return;
		}

		bool activate = MinWidth() <= targetElement.ActualWidth() &&
			targetElement.ActualWidth() < MaxWidth() &&
			MinHeight() <= targetElement.ActualHeight() &&
			targetElement.ActualHeight() < MaxHeight();

		IsActive = activate;
		SetActive(activate);
	}
}
