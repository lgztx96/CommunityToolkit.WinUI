#pragma once

#include "RibbonPanel.g.h"

#ifdef __INTELLISENSE__
#include <limits>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct RibbonPanel : RibbonPanelT<RibbonPanel>
    {
        static constexpr Size GroupAvailableSize{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };

        RibbonPanel() = default;

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct RibbonPanel : RibbonPanelT<RibbonPanel, implementation::RibbonPanel>
    {
    };
}
