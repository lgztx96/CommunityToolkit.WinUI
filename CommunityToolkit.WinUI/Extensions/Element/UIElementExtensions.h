#pragma once

#include "UIElementExtensions.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

#include <winrt/Microsoft.UI.Xaml.Hosting.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;

    struct UIElementExtensions
    {
    private:
        static void OnClipToBoundsPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

    public:
        static inline const wil::single_threaded_property<DependencyProperty> ClipToBoundsProperty =
            DependencyProperty::RegisterAttached(L"ClipToBounds",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<CommunityToolkit::WinUI::UIElementExtensions>(),
                PropertyMetadata(winrt::box_value(false), &UIElementExtensions::OnClipToBoundsPropertyChanged));

        static bool GetClipToBounds(UIElement const& element);

        static void SetClipToBounds(UIElement const& element, bool value);

        static Windows::Foundation::Point CoordinatesFrom(UIElement const& target, UIElement const& parent);

        static Windows::Foundation::Point CoordinatesTo(UIElement const& parent, UIElement const& target);
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct UIElementExtensions : UIElementExtensionsT<UIElementExtensions, implementation::UIElementExtensions>
    {
    };
}
