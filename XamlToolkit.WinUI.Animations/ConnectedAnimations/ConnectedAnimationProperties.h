#pragma once

#include "ConnectedAnimationListProperty.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <vector>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Internal class for connected animation properties.
    /// </summary>
    struct ConnectedAnimationProperties
    {
        winrt::hstring Key;
        winrt::Microsoft::UI::Xaml::UIElement Element{ nullptr };
        std::vector<ConnectedAnimationListProperty> ListAnimProperties;

        bool IsListAnimation() const
        {
            return !ListAnimProperties.empty();
        }
    };
}
