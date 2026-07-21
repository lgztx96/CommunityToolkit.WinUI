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
            size_t operator()(winrt::weak_ref< winrt::FrameworkElement> const& element) const noexcept
            {
                return std::hash<void*>{}(winrt::get_abi(element).get());
            }
        };

        static void RemoveHandlers() noexcept;

        static thread_local inline std::unordered_map<winrt::weak_ref<winrt::FrameworkElement>, HandlerState, WeakFrameworkElementHash> _handlerStates;

    public:
        static void AncestorType_PropertyChanged(winrt::DependencyObject const& obj, winrt::DependencyPropertyChangedEventArgs const& args);

        static winrt::DependencyObject FindAscendant(winrt::DependencyObject element, winrt::TypeName const& name);

        static void FrameworkElement_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void FrameworkElement_Unloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static winrt::IInspectable GetAncestor(winrt::DependencyObject const& obj);

        static void SetAncestor(winrt::DependencyObject const& obj, winrt::IInspectable const& value);

        static winrt::TypeName GetAncestorType(winrt::FrameworkElement const& obj);

        static void SetAncestorType(winrt::FrameworkElement const& obj, winrt::TypeName const& value);

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
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct FrameworkElementExtensions : FrameworkElementExtensionsT<FrameworkElementExtensions, implementation::FrameworkElementExtensions>
    {
    };
}