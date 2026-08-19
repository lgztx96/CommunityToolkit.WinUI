#pragma once

#include "RibbonGroup.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct RibbonGroup : RibbonGroupT<RibbonGroup>
    {
        RibbonGroup();

        virtual void OnApplyTemplate();

        static const wil::single_threaded_property<winrt::DependencyProperty> ContentProperty;

        winrt::UIElement Content() const
        {
            return GetValue(ContentProperty()).try_as<winrt::UIElement>();
        }

        void Content(winrt::UIElement const& value)
        {
            SetValue(ContentProperty(), value);
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> LabelProperty;

        winrt::hstring Label() const
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(LabelProperty()));
        }

        void Label(winrt::hstring const& value)
        {
            SetValue(LabelProperty(), winrt::box_value(value));
        }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct RibbonGroup : RibbonGroupT<RibbonGroup, implementation::RibbonGroup>
    {
    };
}
