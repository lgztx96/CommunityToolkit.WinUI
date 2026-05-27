#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Internal class for connected animation list property.
    /// </summary>
    struct ConnectedAnimationListProperty
    {
        winrt::hstring ElementName;
        Microsoft::UI::Xaml::Controls::ListViewBase ListViewBase{ nullptr };
    };
}
