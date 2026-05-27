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
        Adorner();

        UIElement AdornedElement() const noexcept;

        void AdornedElement(UIElement const& value);

        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> AdornerLayer;

        virtual void OnApplyTemplate() {}

        virtual void OnAttached() {}

        virtual void OnDetaching() {}

        void UpdateLayout();

        void OnLayoutUpdated(IInspectable const& sender, IInspectable const& e);

    private:
        void OnAdornedElementChanged(UIElement const& oldvalue, UIElement const& newvalue);

        void OnSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

        void OnAdornedElementLoaded(IInspectable const& source, RoutedEventArgs const& eventArgs);

        void OnAdornedElementUnloaded(IInspectable const& source, RoutedEventArgs const& eventArgs);

        UIElement _adornedElement;

        FrameworkElement::SizeChanged_revoker _adornedElementSizeChangedRevoker;
        FrameworkElement::Loaded_revoker _adornedElementLoadedRevoker;
        FrameworkElement::Unloaded_revoker _adornedElementUnloadedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct Adorner : AdornerT<Adorner, implementation::Adorner>
    {
    };
}
