// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../IAddChild.h"
#include "../MdFlowDocument.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements::Html
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    // block
    class MdDetails : public IAddChild
    {
    private:
        InlineUIContainer _inlineUIContainer;
        Expander _expander;
        MdFlowDocument _flowDocument;
        Paragraph _paragraph;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdDetails()
        {
            //_htmlNode = details;

         /*   auto header = _htmlNode.ChildNodes
                .FirstOrDefault(
                    x => x.Name == "summary" ||
                    x.Name == "header");*/

            _expander.HorizontalAlignment(HorizontalAlignment::Stretch);
            //_flowDocument = MdFlowDocument(details);
            _flowDocument.RichTextBlock().HorizontalAlignment(HorizontalAlignment::Stretch);
            _expander.Content(_flowDocument.RichTextBlock());
            auto headerBlock = TextBlock();
            //headerBlock.Text(header.InnerText);
            headerBlock.HorizontalAlignment(HorizontalAlignment::Stretch);
            _expander.Header(headerBlock);
            _inlineUIContainer.Child(_expander);
            _paragraph = Paragraph();
            _paragraph.Inlines().Append(_inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}

