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
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SwitchConverter : SwitchConverterT<SwitchConverter>
	{
		SwitchConverter();

		winrt::XamlToolkit::WinUI::Controls::CaseCollection SwitchCases() const
		{
			return GetValue(SwitchCasesProperty())
				.try_as<winrt::XamlToolkit::WinUI::Controls::CaseCollection>();
		}

		void SwitchCases(winrt::XamlToolkit::WinUI::Controls::CaseCollection const& value)
		{
			SetValue(SwitchCasesProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> SwitchCasesProperty =
			winrt::DependencyProperty::Register(
				L"SwitchCases",
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::CaseCollection>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		//winrt::IReference<winrt::TypeName> TargetType() const
		//{
		//	return GetValue(TargetTypeProperty()).try_as<winrt::TypeName>();
		//}

		//void TargetType(winrt::IReference<winrt::TypeName> const& value)
		//{
		//	SetValue(TargetTypeProperty(), value);
		//}

		//static inline const wil::single_threaded_property<winrt::DependencyProperty> TargetTypeProperty =
		//	winrt::DependencyProperty::Register(
		//		L"TargetType",
		//		winrt::xaml_typename<winrt::TypeName>(),
		//		winrt::xaml_typename<class_type>(),
		//		winrt::PropertyMetadata{ nullptr });

		winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

		winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SwitchConverter : SwitchConverterT<SwitchConverter, implementation::SwitchConverter>
	{	
	};
}
