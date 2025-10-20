// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../IAddChild.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements::Html 
{
    using namespace Windows::UI::Text;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    class MdBlock : public IAddChild
    {
    private:
        Paragraph _paragraph;
        std::vector<RichTextBlock> _richTextBlocks;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdBlock()
        {
            //_htmlNode = block;
            auto align = "";// _htmlNode.GetAttributeValue("align", "left");

            TextAlignment textAlignment;

            if (align == "left")
                textAlignment = TextAlignment::Left;
            else if (align == "right")
                textAlignment = TextAlignment::Right;
            else if (align == "center")
                textAlignment = TextAlignment::Center;
            else if (align == "justify")
                textAlignment = TextAlignment::Justify;
            else
                textAlignment = TextAlignment::Left;

            _paragraph.TextAlignment(textAlignment);
            StyleBlock();
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto blockChild = child->TextElement().try_as<Block>())
            {
                _paragraph.Inlines().Append(LineBreak());
                auto inlineUIContainer = InlineUIContainer();
                auto richTextBlock = RichTextBlock();
                richTextBlock.Blocks().Append(blockChild);
                inlineUIContainer.Child(richTextBlock);
                _richTextBlocks.push_back(richTextBlock);
                _paragraph.Inlines().Append(inlineUIContainer);
                _paragraph.Inlines().Append(LineBreak());
            }
            else if (auto  inlineChild = child->TextElement().try_as<Inline>())
            {
                _paragraph.Inlines().Append(inlineChild);
            }
        }

        void StyleBlock()
        {
            auto name = "";// _htmlNode.Name.ToLower();

            if (name == "address")
            {
                _paragraph.FontStyle(FontStyle::Italic);
            }

            for (const auto& richTextBlock : _richTextBlocks)
            {
                richTextBlock.FontStyle(FontStyle::Italic);
            }
        }
    };
}

