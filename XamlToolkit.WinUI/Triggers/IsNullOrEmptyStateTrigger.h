#pragma once

#include "IsNullOrEmptyStateTrigger.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct IsNullOrEmptyStateTrigger : IsNullOrEmptyStateTriggerT<IsNullOrEmptyStateTrigger>
	{
		IsNullOrEmptyStateTrigger();

		winrt::IInspectable Value() const { return GetValue(ValueProperty()); }
		void Value(winrt::IInspectable const& value) const { SetValue(ValueProperty(), value); }

		static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

	private:
		static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		void UpdateTrigger();

		static bool IsNullOrEmpty(winrt::IInspectable const& val);

		winrt::event_token _containerChangedToken{ 0 };
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct IsNullOrEmptyStateTrigger : IsNullOrEmptyStateTriggerT<IsNullOrEmptyStateTrigger, implementation::IsNullOrEmptyStateTrigger>
	{
	};
}
