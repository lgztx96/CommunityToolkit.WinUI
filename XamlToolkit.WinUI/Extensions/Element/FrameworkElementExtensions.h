#pragma once

#include "FrameworkElementExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct AncestorTracker : winrt::implements<AncestorTracker, winrt::IInspectable>
    {
        winrt::event_token loadedToken{ 0 };
        winrt::event_token unloadedToken{ 0 };
    };

    struct FrameworkElementExtensions
    {
        static void AncestorType_PropertyChanged(winrt::DependencyObject const& obj, winrt::DependencyPropertyChangedEventArgs const& args);

        static void FrameworkElement_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void FrameworkElement_Unloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static winrt::IInspectable GetAncestor(winrt::DependencyObject const& obj);

        static void SetAncestor(winrt::DependencyObject const& obj, winrt::IInspectable const& value);

        static winrt::TypeName GetAncestorType(winrt::FrameworkElement const& obj);

        static void SetAncestorType(winrt::FrameworkElement const& obj, winrt::TypeName const& value);

        static winrt::com_ptr<AncestorTracker> GetAncestorTracker(winrt::FrameworkElement const& element);

        static inline const wil::single_threaded_property<winrt::DependencyProperty> AncestorProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"Ancestor", 
                winrt::xaml_typename<winrt::IInspectable>(),
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::FrameworkElementExtensions>(), 
                winrt::PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> AncestorTypeProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"AncestorType", 
                winrt::xaml_typename<winrt::TypeName>(),
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::FrameworkElementExtensions>(), 
                winrt::PropertyMetadata(nullptr, &FrameworkElementExtensions::AncestorType_PropertyChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> AncestorTrackerProperty =
            winrt::DependencyProperty::RegisterAttached(
                L"AncestorTracker",
                winrt::xaml_typename<winrt::IInspectable>(),
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::FrameworkElementExtensions>(),
                winrt::PropertyMetadata(nullptr));
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct FrameworkElementExtensions : FrameworkElementExtensionsT<FrameworkElementExtensions, implementation::FrameworkElementExtensions>
    {
    };
}