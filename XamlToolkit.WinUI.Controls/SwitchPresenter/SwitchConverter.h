#pragma once

#include "SwitchConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SwitchConverter : SwitchConverterT<SwitchConverter>
	{
		SwitchConverter();

		Controls::CaseCollection SwitchCases() const
		{
			return GetValue(SwitchCasesProperty)
				.try_as<Controls::CaseCollection>();
		}

		void SwitchCases(Controls::CaseCollection const& value)
		{
			SetValue(SwitchCasesProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SwitchCasesProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"SwitchCases",
				winrt::xaml_typename<Controls::CaseCollection>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		//IReference<TypeName> TargetType() const
		//{
		//	return GetValue(TargetTypeProperty).try_as<TypeName>();
		//}

		//void TargetType(IReference<TypeName> const& value)
		//{
		//	SetValue(TargetTypeProperty, value);
		//}

		//static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TargetTypeProperty =
		//	winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
		//		L"TargetType",
		//		winrt::xaml_typename<TypeName>(),
		//		winrt::xaml_typename<class_type>(),
		//		winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, winrt::hstring const& language);

		IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, winrt::hstring const& language);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SwitchConverter : SwitchConverterT<SwitchConverter, implementation::SwitchConverter>
	{
	};
}
