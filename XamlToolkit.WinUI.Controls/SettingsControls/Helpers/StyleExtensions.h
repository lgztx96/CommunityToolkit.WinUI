#pragma once

#include "StyleExtensions.g.h"
#include "ResourceDictionaryExtensions.h"
#include "StyleExtensionResourceDictionary.h"

#ifdef __INTELLISENSE__
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
	struct StyleExtensions : StyleExtensionsT<StyleExtensions>
	{
		static ResourceDictionary GetResources(Microsoft::UI::Xaml::Style obj);

		static void SetResources(Microsoft::UI::Xaml::Style obj, ResourceDictionary value);

		static void ResourcesChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e);

		static void ForceControlToReloadThemeResources(FrameworkElement frameworkElement);

		static inline const wil::single_threaded_property<DependencyProperty> ResourcesProperty =
			DependencyProperty::RegisterAttached(L"Resources",
				winrt::xaml_typename<ResourceDictionary>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(nullptr, ResourcesChanged));
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct StyleExtensions : StyleExtensionsT<StyleExtensions, implementation::StyleExtensions>
	{
	};
}
