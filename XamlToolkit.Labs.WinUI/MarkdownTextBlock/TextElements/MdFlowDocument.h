// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Documents;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdFlowDocument final : public IAddChild
    {
    private:
        //HtmlNode _htmlNode;
        RichTextBlock _richTextBlock;

    public:
        winrt::TextElement TextElement() const override
        {
            throw winrt::hresult_not_implemented();
        }

        winrt::RichTextBlock RichTextBlock()
        {
            return _richTextBlock;
        }

        bool IsHtml()
        {
            // return _htmlNode;
        };

        MdFlowDocument() = default;

        //MdFlowDocument(HtmlNode node)
        //{
        //    _htmlNode = node;
        //}

        void AddChild(IAddChild* child) override
        {
            if (auto element = child->TextElement())
            {
                if (auto block = element.try_as<winrt::Block>())
                {
                    _richTextBlock.Blocks().Append(block);
                }
                else if (auto inlineElement = element.try_as<winrt::Inline>())
                {
                    winrt::Paragraph paragraph;
                    paragraph.Inlines().Append(inlineElement);
                    _richTextBlock.Blocks().Append(paragraph);
                }
            }
        }
    };
}


