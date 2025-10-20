// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "WinUIRenderer.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;

    class MdParagraph final : public IAddChild
    {
    private:
        Paragraph _paragraph;
        WinUIRenderer* _renderer;
    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdParagraph(WinUIRenderer* renderer)
        {
            _renderer = renderer;

            // Lists are plain Paragraph_s, one per item.
            // This is so that you can select across list items.
            auto themes = renderer->Config().Themes();
           
            if (themes.ParagraphLineHeight() > 0)
            {
                _paragraph.LineHeight(themes.ParagraphLineHeight());
            }
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<Inline>())
            {
                _paragraph.Inlines().Append(inlineChild);
            }
            else if (auto blockChild = child->TextElement().try_as<Block>())
            {
                InlineUIContainer inlineUIContainer;
                RichTextBlock richTextBlock;
                richTextBlock.TextWrapping(TextWrapping::Wrap);
                richTextBlock.Blocks().Append(blockChild);
                inlineUIContainer.Child(richTextBlock);
                _paragraph.Inlines().Append(inlineUIContainer);
            }
        }
    };
}

