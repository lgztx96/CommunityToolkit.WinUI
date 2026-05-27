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

    private:

        int _columnCount;
        int _rowCount;
        float _borderThickness;
        std::vector<float> _columnWidths;
        std::vector<float> _rowHeights;

    public:
        MdTableUIElement(int columnCount, int rowCount, float borderThickness, Brush const& borderBrush, Brush const& headingBrush, CornerRadius const& cornerRadius, Thickness const& tableMargin);

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

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct MdTableUIElement : MdTableUIElementT<MdTableUIElement, implementation::MdTableUIElement>
    {
    };
}
