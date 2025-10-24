#pragma once

#include "TabbedCommandBarItemTemplateSelector.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct TabbedCommandBarItemTemplateSelector : TabbedCommandBarItemTemplateSelectorT<TabbedCommandBarItemTemplateSelector>
    {
        wil::single_threaded_rw_property<DataTemplate> Normal{ nullptr };

        wil::single_threaded_rw_property<DataTemplate> Contextual{ nullptr };

        DataTemplate SelectTemplateCore(IInspectable const& item);

        DataTemplate SelectTemplateCore(IInspectable const& item, DependencyObject const& container);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct TabbedCommandBarItemTemplateSelector : TabbedCommandBarItemTemplateSelectorT<TabbedCommandBarItemTemplateSelector, implementation::TabbedCommandBarItemTemplateSelector>
    {
    };
}
