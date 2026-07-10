#pragma once

#include "AdornerDecorator.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct AdornerDecorator : AdornerDecoratorT<AdornerDecorator>
    {
    private:
        static constexpr std::wstring_view PartAdornerLayer = L"AdornerLayer";

    public:
        winrt::UIElement Child() const
        {
            return GetValue(ChildProperty()).try_as<winrt::UIElement>();
        }

        void Child(winrt::UIElement const& value)
        {
            SetValue(ChildProperty(), value);
        }

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ChildProperty =
            winrt::DependencyProperty::Register(
                L"Child",
                winrt::xaml_typename<winrt::UIElement>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr });

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> AdornerLayer;

        AdornerDecorator();

        void OnApplyTemplate();
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct AdornerDecorator : AdornerDecoratorT<AdornerDecorator, implementation::AdornerDecorator>
    {
    };
}
