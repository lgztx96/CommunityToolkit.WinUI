#pragma once

#include "FrameworkElementExtensions.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::Xaml::Interop;

    struct FrameworkElementExtensions : FrameworkElementExtensionsT<FrameworkElementExtensions>
    {
        static inline FrameworkElement::Unloaded_revoker _elementUnloadRevoker;
        static inline FrameworkElement::Loaded_revoker _ancestorLoadRevoker;

        static void AncestorType_PropertyChanged(DependencyObject const& obj, DependencyPropertyChangedEventArgs const& args);

        static DependencyObject FindAscendant(DependencyObject element, TypeName const& name);

        static void FrameworkElement_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

        static void FrameworkElement_Unloaded(IInspectable const& sender, RoutedEventArgs const& e);

        static IInspectable GetAncestor(DependencyObject const& obj);

        static void SetAncestor(DependencyObject const& obj, IInspectable const& value);

        static TypeName GetAncestorType(FrameworkElement const& obj);

        static void SetAncestorType(FrameworkElement const& obj, TypeName const& value);

        static inline const wil::single_threaded_property<DependencyProperty> AncestorProperty =
            DependencyProperty::RegisterAttached(L"Ancestor", winrt::xaml_typename<IInspectable>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> AncestorTypeProperty =
            DependencyProperty::RegisterAttached(L"AncestorType", winrt::xaml_typename<TypeName>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, AncestorType_PropertyChanged));
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct FrameworkElementExtensions : FrameworkElementExtensionsT<FrameworkElementExtensions, implementation::FrameworkElementExtensions>
    {
    };
}
