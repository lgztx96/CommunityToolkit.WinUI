#pragma once

#include "StyleExtensions.g.h"
#include "ResourceDictionaryExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <rpcndr.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    MIDL_INTERFACE("BE6759D0-57BA-4F6E-A853-7833006BC60D") IStyleExtensionResourceDictionary : ::IUnknown { };

    struct StyleExtensionResourceDictionary : winrt::ResourceDictionaryT<StyleExtensionResourceDictionary, IStyleExtensionResourceDictionary>
    {

    };

    struct StyleExtensions
    {
        static winrt::ResourceDictionary GetResources(winrt::Style const& obj);

        static void SetResources(winrt::Style const& obj, winrt::ResourceDictionary const& value);

        static void ResourcesChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

        static void ForceControlToReloadThemeResources(winrt::FrameworkElement const& frameworkElement);

        static const wil::single_threaded_property<winrt::DependencyProperty> ResourcesProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct StyleExtensions : StyleExtensionsT<StyleExtensions, implementation::StyleExtensions>
    {
    };
}
