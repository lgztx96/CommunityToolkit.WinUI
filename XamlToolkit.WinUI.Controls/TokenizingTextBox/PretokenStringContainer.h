#pragma once

#include "PretokenStringContainer.g.h"

#ifdef __INTELLISENSE__
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
		winrt::hstring Text() const { return winrt::unbox_value<winrt::hstring>(GetValue(TextProperty)); }
		void Text(winrt::hstring const& value) { SetValue(TextProperty, winrt::box_value(value)); }

		// Using a DependencyProperty as the backing store for Text.  This enables animation, styling, binding, etc...
		static inline const wil::single_threaded_property<DependencyProperty> TextProperty =
			DependencyProperty::Register(L"Text", winrt::xaml_typename<hstring>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(L"")));

		wil::single_threaded_property<bool> IsLast;

		PretokenStringContainer(bool isLast = false) : IsLast(isLast) { }

		winrt::hstring ToString() const
		{
			return Text();
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct PretokenStringContainer : PretokenStringContainerT<PretokenStringContainer, implementation::PretokenStringContainer>
	{
	};
}
