#pragma once

#include "MdTableUIElement.g.h"
#include <execution>
#include <vector>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Shapes;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;

    struct MdTableUIElement : MdTableUIElementT<MdTableUIElement>
    {
        MdTableUIElement() : _columnCount(0), _rowCount(0), _borderThickness(0.0f) {}

    private:

        int _columnCount;
        int _rowCount;
        float _borderThickness;
        std::vector<float> _columnWidths;
        std::vector<float> _rowHeights;

    public:
        MdTableUIElement(int columnCount, int rowCount, float borderThickness, Brush borderBrush, Brush headingBrush, CornerRadius cornerRadius, Thickness tableMargin);

        // Helper method to enumerate FrameworkElements instead of UIElements.
        IVectorView<FrameworkElement> ContentChildren() const;

        // Helper method to get table vertical edges.
        IVectorView<Rectangle> VerticalLines();

        // Helper method to get table horizontal edges.
        IVectorView<Rectangle> HorizontalLines();

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct MdTableUIElement : MdTableUIElementT<MdTableUIElement, implementation::MdTableUIElement>
    {
    };
}
