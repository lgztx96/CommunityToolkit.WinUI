#pragma once

#include "MdTableUIElement.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <vector>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Shapes;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct MdTableUIElement : MdTableUIElementT<MdTableUIElement>
    {
        MdTableUIElement() : _columnCount(0), _rowCount(0), _borderThickness(0.0f) {}

        MdTableUIElement(int columnCount, int rowCount, float borderThickness, winrt::Brush const& borderBrush, winrt::Brush const& headingBrush, winrt::CornerRadius const& cornerRadius, winrt::Thickness const& tableMargin);

        // Helper method to enumerate FrameworkElements instead of UIElements.
        winrt::IVectorView<winrt::FrameworkElement> ContentChildren() const;

        // Helper method to get table vertical edges.
        winrt::IVectorView<winrt::Rectangle> VerticalLines();

        // Helper method to get table horizontal edges.
        winrt::IVectorView<winrt::Rectangle> HorizontalLines();

        winrt::Size MeasureOverride(winrt::Size availableSize);

        winrt::Size ArrangeOverride(winrt::Size finalSize);

	private:
        int _columnCount;
        int _rowCount;
        float _borderThickness;
        std::vector<float> _columnWidths;
        std::vector<float> _rowHeights;
    };
}
