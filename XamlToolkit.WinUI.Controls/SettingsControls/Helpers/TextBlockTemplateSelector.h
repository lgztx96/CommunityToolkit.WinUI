#pragma once

#include "TextBlockTemplateSelector.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct TextBlockTemplateSelector : TextBlockTemplateSelectorT<TextBlockTemplateSelector>
    {
        TextBlockTemplateSelector() = default;

        [[nodiscard]] winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item, winrt::DependencyObject const& container);
        [[nodiscard]] winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item);

        wil::single_threaded_rw_property<winrt::DataTemplate> TextBlockTemplate;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct TextBlockTemplateSelector : TextBlockTemplateSelectorT<TextBlockTemplateSelector, implementation::TextBlockTemplateSelector>
    {
    };
}
