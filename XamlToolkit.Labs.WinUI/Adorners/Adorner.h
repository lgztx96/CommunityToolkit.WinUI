#pragma once

#include "Adorner.g.h"

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
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct Adorner : AdornerT<Adorner>
    {
        friend struct AdornerLayer;

        Adorner();

        ~Adorner();

        winrt::UIElement AdornedElement() const noexcept;

        void AdornedElement(winrt::UIElement const& value);

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> AdornerLayer;

        virtual void OnApplyTemplate() {}

        virtual void OnAttached() {}

        virtual void OnDetaching() {}

        void UpdateLayout();

        void OnLayoutUpdated(winrt::IInspectable const& sender, winrt::IInspectable const& e);

        static winrt::fire_and_forget final_release(std::unique_ptr<Adorner> self);

    private:
        void OnAdornedElementChanged(winrt::UIElement const& oldvalue, winrt::UIElement const& newvalue);

        void OnSizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        void OnAdornedElementLoaded(winrt::IInspectable const& source, winrt::RoutedEventArgs const& eventArgs);

        void OnAdornedElementUnloaded(winrt::IInspectable const& source, winrt::RoutedEventArgs const& eventArgs);

        winrt::UIElement _adornedElement{ nullptr };

        winrt::event_token _adornedElementSizeChangedToken;
        winrt::event_token _adornedElementLoadedToken;
        winrt::event_token _adornedElementUnloadedToken;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct Adorner : AdornerT<Adorner, implementation::Adorner>
    {
    };
}
