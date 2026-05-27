#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    struct IVisualTreeHelper
    {
        virtual ~IVisualTreeHelper() = default;
        virtual winrt::DependencyObject GetParent(winrt::DependencyObject const& reference) const = 0;
    };
}
