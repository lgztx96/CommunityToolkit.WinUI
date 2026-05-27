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

        static inline const wil::single_threaded_property<DependencyProperty> ContentProperty =
            DependencyProperty::Register(
                L"Content",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::UIElement>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

        winrt::Microsoft::UI::Xaml::UIElement Content() const
        {
            return GetValue(ContentProperty).try_as<winrt::Microsoft::UI::Xaml::UIElement>();
        }

        void Content(winrt::Microsoft::UI::Xaml::UIElement const& value)
        {
            SetValue(ContentProperty, value);
        }

        static inline const wil::single_threaded_property<DependencyProperty> LabelProperty =
            DependencyProperty::Register(
                L"Label",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L"") });

        winrt::hstring Label() const
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(LabelProperty));
        }

        void Label(winrt::hstring const& value)
        {
            SetValue(LabelProperty, winrt::box_value(value));
        }
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct RibbonGroup : RibbonGroupT<RibbonGroup, implementation::RibbonGroup>
    {
    };
}
