// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdTableCell.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    class MdTableRow final : public IAddChild
    {
    private:
        StackPanel _stackPanel;
        Paragraph _paragraph;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdTableRow()
        {
            _stackPanel.Orientation(Orientation::Horizontal);
            InlineUIContainer inlineUIContainer;
            inlineUIContainer.Child(_stackPanel);
            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto cellChild = dynamic_cast<const MdTableCell*>(child))
            {
                auto richTextBlock = RichTextBlock();
                richTextBlock.Blocks().Append(cellChild->TextElement().as<Paragraph>());
                _stackPanel.Children().Append(richTextBlock);
            }
        }
    };
}

