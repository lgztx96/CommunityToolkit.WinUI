#pragma once

#include "PretokenStringContainer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt 
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct PretokenStringContainer : PretokenStringContainerT<PretokenStringContainer>
	{
		winrt::hstring Text() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextProperty())); }
		void Text(winrt::hstring const& value) { SetValue(TextProperty(), winrt::box_value(value)); }

		// Using a DependencyProperty as the backing store for Text.  This enables animation, styling, binding, etc...
		static const wil::single_threaded_property<winrt::DependencyProperty> TextProperty;

		wil::single_threaded_property<bool> IsLast;

		PretokenStringContainer(bool isLast = false) : IsLast(isLast) { }

		winrt::hstring ToString() const;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct PretokenStringContainer : PretokenStringContainerT<PretokenStringContainer, implementation::PretokenStringContainer>
	{
	};
}
