#pragma once

#include "FrameworkElementExtensions.g.h"

#ifdef __INTELLISENSE__
#include <unordered_map>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct FrameworkElementExtensions
    {
    private:
        struct HandlerState
        {
			winrt::event_token _loadedToken;
            winrt::event_token _unloadedToken;
        };

        struct WeakFrameworkElementHash
        {
            size_t operator()(winrt::weak_ref<FrameworkElement> const& element) const noexcept
            {
                return std::hash<void*>{}(winrt::get_abi(element).get());
            }
        };

        static void RemoveHandlers() noexcept;

        static thread_local inline std::unordered_map<winrt::weak_ref<FrameworkElement>, HandlerState, WeakFrameworkElementHash> _handlerStates;

    public:
        static void AncestorType_PropertyChanged(DependencyObject const& obj, DependencyPropertyChangedEventArgs const& args);

        static DependencyObject FindAscendant(DependencyObject element, TypeName const& name);

        static void FrameworkElement_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

        static void FrameworkElement_Unloaded(IInspectable const& sender, RoutedEventArgs const& e);

        static IInspectable GetAncestor(DependencyObject const& obj);

        static void SetAncestor(DependencyObject const& obj, IInspectable const& value);

        static TypeName GetAncestorType(FrameworkElement const& obj);

        static void SetAncestorType(FrameworkElement const& obj, TypeName const& value);

        static inline const wil::single_threaded_property<DependencyProperty> AncestorProperty =
            DependencyProperty::RegisterAttached(
                L"Ancestor", 
                winrt::xaml_typename<IInspectable>(), 
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::FrameworkElementExtensions>(), 
                PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> AncestorTypeProperty =
            DependencyProperty::RegisterAttached(
                L"AncestorType", 
                winrt::xaml_typename<TypeName>(), 
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::FrameworkElementExtensions>(), 
                PropertyMetadata(nullptr, &FrameworkElementExtensions::AncestorType_PropertyChanged));
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct FrameworkElementExtensions : FrameworkElementExtensionsT<FrameworkElementExtensions, implementation::FrameworkElementExtensions>
    {
    };
}