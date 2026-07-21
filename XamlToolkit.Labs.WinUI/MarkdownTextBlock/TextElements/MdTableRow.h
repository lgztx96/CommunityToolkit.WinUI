// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdTableCell.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdTableRow final : public IAddChild
    {
    private:
        winrt::StackPanel _stackPanel;
        winrt::Paragraph _paragraph;

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdTableRow()
        {
            _stackPanel.Orientation(winrt::Orientation::Horizontal);
            winrt::InlineUIContainer inlineUIContainer;
            inlineUIContainer.Child(_stackPanel);
            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(IAddChild* child) override
        {
            if (auto cellChild = dynamic_cast<const MdTableCell*>(child))
            {
                winrt::RichTextBlock richTextBlock;
                richTextBlock.Blocks().Append(cellChild->TextElement().as<winrt::Paragraph>());
                _stackPanel.Children().Append(richTextBlock);
            }
        }
    };
}

