#pragma once

#include "UIElementExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Composition.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Input;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct UIElementExtensions
    {
        static bool GetClipToBounds(winrt::UIElement const& element);

        static void SetClipToBounds(winrt::UIElement const& element, bool value);

        static void OnClipToBoundsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static const wil::single_threaded_property<winrt::DependencyProperty> ClipToBoundsProperty;

        static winrt::Point CoordinatesFrom(winrt::UIElement const& target, winrt::UIElement const& parent);

        static winrt::Point CoordinatesTo(winrt::UIElement const& parent, winrt::UIElement const& target);

#pragma region Mouse
        static winrt::InputSystemCursorShape GetCursor(winrt::FrameworkElement const& obj);

        static void SetCursor(winrt::FrameworkElement const& obj, winrt::InputSystemCursorShape const& value);

        static void CursorChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static const wil::single_threaded_property<winrt::DependencyProperty> CursorProperty;
#pragma endregion 
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct UIElementExtensions : UIElementExtensionsT<UIElementExtensions, implementation::UIElementExtensions>
    {
    };
}
