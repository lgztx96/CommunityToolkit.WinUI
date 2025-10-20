// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../IAddChild.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements::Html
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    class MdInline : public IAddChild
    {
    private:
        Paragraph _paragraph;
        InlineUIContainer _inlineUIContainer;
        RichTextBlock _richTextBlock;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _inlineUIContainer;
        }

        MdInline()
        {
            _richTextBlock.Blocks().Append(_paragraph);
            _richTextBlock.HorizontalAlignment(HorizontalAlignment::Stretch);
            _inlineUIContainer.Child(_richTextBlock);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<Inline>())
            {
                _paragraph.Inlines().Append(inlineChild);
            }
            // we shouldn't support rendering block in inline
            // but if we want to support it, we can do it like this:
            //else if (child.TextElement is Block blockChild)
            //{
            //    _richTextBlock.Blocks.Add(blockChild);
            //    // if we add a block to an inline container,
            //    // if the next child is inline, it needs to be added after the block
            //    // so we add a paragraph. This way the next time
            //    // AddChild is called, it's added to the paragraph
            //    _paragraph = Paragraph();
            //    _richTextBlock.Blocks.Add(_paragraph);
            //}
        }
    };
}


