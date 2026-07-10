#pragma once

#include "SwitchPresenter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SwitchPresenter : SwitchPresenterT<SwitchPresenter>
	{
		winrt::XamlToolkit::WinUI::Controls::Case CurrentCase() const
		{
			return GetValue(CurrentCaseProperty()).try_as<winrt::XamlToolkit::WinUI::Controls::Case>();
		}

		void CurrentCase(winrt::XamlToolkit::WinUI::Controls::Case const& value)
		{
			SetValue(CurrentCaseProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CurrentCaseProperty =
			winrt::DependencyProperty::Register(
				L"CurrentCase",
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::Case>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr));

		winrt::XamlToolkit::WinUI::Controls::CaseCollection SwitchCases() const
		{
			return GetValue(SwitchCasesProperty()).try_as<winrt::XamlToolkit::WinUI::Controls::CaseCollection>();
		}

		void SwitchCases(winrt::XamlToolkit::WinUI::Controls::CaseCollection const& value)
		{
			SetValue(SwitchCasesProperty(), value);
		}

		static void OnSwitchCasesPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> SwitchCasesProperty =
			winrt::DependencyProperty::Register(
				L"SwitchCases",
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::CaseCollection>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr, &SwitchPresenter::OnSwitchCasesPropertyChanged));

		winrt::IInspectable Value() const
		{
			return GetValue(ValueProperty());
		}

		void Value(winrt::IInspectable const& value)
		{
			SetValue(ValueProperty(), value);
		}

		static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty =
			winrt::DependencyProperty::Register(
				L"Value",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr, &SwitchPresenter::OnValuePropertyChanged));

		/*winrt::IReference<winrt::TypeName> TargetType()
		{
			return GetValue(TargetTypeProperty()).try_as<winrt::IReference<winrt::TypeName>>();
		}

		void TargetType(winrt::IReference<winrt::TypeName> const& value)
		{
			SetValue(TargetTypeProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> TargetTypeProperty =
			winrt::DependencyProperty::Register(
				L"TargetType",
				winrt::xaml_typename<winrt::TypeName>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr));*/

		SwitchPresenter();

		void OnApplyTemplate();

	private:
		void SwitchPresenter_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void EvaluateCases();
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SwitchPresenter : SwitchPresenterT<SwitchPresenter, implementation::SwitchPresenter>
	{
	};
}
