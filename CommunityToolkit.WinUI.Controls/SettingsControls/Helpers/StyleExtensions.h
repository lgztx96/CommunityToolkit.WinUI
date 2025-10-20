#pragma once

#include "StyleExtensions.g.h"
#include "ResourceDictionaryExtensions.h"
#include "StyleExtensionResourceDictionary.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;

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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct StyleExtensions : StyleExtensionsT<StyleExtensions, implementation::StyleExtensions>
	{
	};
}
