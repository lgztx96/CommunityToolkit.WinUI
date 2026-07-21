// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Media3D.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media::Media3D;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdTaskListCheckBox final : public IAddChild
    {
    private:
        winrt::TextElement _textElement{nullptr};

    public:

        winrt::TextElement TextElement() const override 
        {
            return _textElement;
        }

        MdTaskListCheckBox(wchar_t task_mark)
        {
            winrt::Grid grid;
            winrt::CompositeTransform3D transform;
            transform.TranslateY(2);
            grid.Transform3D(transform);
            grid.Width(16);
            grid.Height(16);
            grid.Margin(winrt::Thickness(2, 0, 2, 0));
            grid.BorderThickness(winrt::Thickness(1, 1, 1, 1));
            grid.BorderBrush(winrt::SolidColorBrush(winrt::Microsoft::UI::Colors::Gray()));
            winrt::FontIcon icon;
            icon.FontSize(16);
            icon.HorizontalAlignment(winrt::HorizontalAlignment::Center);
            icon.VerticalAlignment(winrt::VerticalAlignment::Center);
            icon.Glyph(L"\uE73E");
            grid.Children().Append(
				task_mark == 'x' || task_mark == 'X' ?  icon : TextBlock().as<winrt::UIElement>());
            grid.Padding(winrt::Thickness(0));
            grid.CornerRadius(winrt::CornerRadius(2,2,2,2));
            winrt::InlineUIContainer inlineUIContainer;
            inlineUIContainer.Child(grid);
            _textElement = inlineUIContainer;
        }
    };
}


