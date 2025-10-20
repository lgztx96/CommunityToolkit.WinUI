// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Documents;

    class MdFlowDocument final : public IAddChild
    {
    private:
        //HtmlNode _htmlNode;
        RichTextBlock _richTextBlock;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            throw winrt::hresult_not_implemented();
        }

        RichTextBlock RichTextBlock()
        {
            return _richTextBlock;
        }

        bool IsHtml()
        {
            // return _htmlNode != nullptr;
        };

        MdFlowDocument() = default;

        //MdFlowDocument(HtmlNode node)
        //{
        //    _htmlNode = node;
        //}

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto element = child->TextElement())
            {
                if (auto block = element.try_as<Block>())
                {
                    _richTextBlock.Blocks().Append(block);
                }
                else if (auto inlineElement = element.try_as<Inline>())
                {
                    Paragraph paragraph;
                    paragraph.Inlines().Append(inlineElement);
                    _richTextBlock.Blocks().Append(paragraph);
                }
            }
        }
    };
}


