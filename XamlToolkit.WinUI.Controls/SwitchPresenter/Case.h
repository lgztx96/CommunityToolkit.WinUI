#pragma once

#include "Case.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct Case : CaseT<Case>
	{
		Case() = default;

		winrt::Windows::Foundation::IInspectable Content() const
		{
			return GetValue(ContentProperty).as<winrt::Windows::Foundation::IInspectable>();
		}

		void Content(winrt::Windows::Foundation::IInspectable const& value)
		{
			SetValue(ContentProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ContentProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Content",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		bool IsDefault() const
		{
			return winrt::unbox_value<bool>(GetValue(IsDefaultProperty));
		}

		void IsDefault(bool value)
		{
			SetValue(IsDefaultProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsDefaultProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsDefault",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(false) });

		winrt::Windows::Foundation::IInspectable Value() const
		{
			return GetValue(ValueProperty).as<winrt::Windows::Foundation::IInspectable>();
		}

		void Value(winrt::Windows::Foundation::IInspectable const& value)
		{
			SetValue(ValueProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ValueProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Value",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct Case : CaseT<Case, implementation::Case>
	{
	};
}
