#pragma once

#include "TabbedCommandBarItemTemplateSelector.g.h"

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
    struct TabbedCommandBarItemTemplateSelector : TabbedCommandBarItemTemplateSelectorT<TabbedCommandBarItemTemplateSelector>
    {
        wil::single_threaded_rw_property<winrt::DataTemplate> Normal{ nullptr };

        wil::single_threaded_rw_property<winrt::DataTemplate> Contextual{ nullptr };

        winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item) const;

        winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item, winrt::DependencyObject const& container);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct TabbedCommandBarItemTemplateSelector : TabbedCommandBarItemTemplateSelectorT<TabbedCommandBarItemTemplateSelector, implementation::TabbedCommandBarItemTemplateSelector>
    {
    };
}
