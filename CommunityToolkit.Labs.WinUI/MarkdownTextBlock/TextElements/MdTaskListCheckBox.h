// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.Media3D.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media::Media3D;

    class MdTaskListCheckBox final : public IAddChild
    {
    private:
        Microsoft::UI::Xaml::Documents::TextElement _textElement{nullptr};

    public:

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override 
        {
            return _textElement;
        }

        MdTaskListCheckBox(wchar_t task_mark)
        {
            Grid grid;
            CompositeTransform3D transform;
            transform.TranslateY(2);
            grid.Transform3D(transform);
            grid.Width(16);
            grid.Height(16);
            grid.Margin(Thickness(2, 0, 2, 0));
            grid.BorderThickness(Thickness(1, 1, 1, 1));
            grid.BorderBrush(SolidColorBrush(Microsoft::UI::Colors::Gray()));
            FontIcon icon;
            icon.FontSize(16);
            icon.HorizontalAlignment(HorizontalAlignment::Center);
            icon.VerticalAlignment(VerticalAlignment::Center);
            icon.Glyph(L"\uE73E");
            grid.Children().Append(
				task_mark == 'x' || task_mark == 'X' ?  icon : TextBlock().as<UIElement>());
            grid.Padding(Thickness(0));
            grid.CornerRadius(CornerRadius(2,2,2,2));
            InlineUIContainer inlineUIContainer;
            inlineUIContainer.Child(grid);
            _textElement = inlineUIContainer;
        }
    };
}


