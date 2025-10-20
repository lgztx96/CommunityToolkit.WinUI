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

    class MdListItem : public IAddChild
    {
    private:
        Paragraph _paragraph;
        RichTextBlock _richTextBlock;
        WinUIRenderer* _renderer;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdListItem(WinUIRenderer* renderer)
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

        void SetBullet([[maybe_unused]] size_t bulletCount, std::wstring_view bullet) {

            auto themes = _renderer->Config().Themes();
            Thickness margin = themes.ParagraphMargin();
            margin.Left += themes.ListGutterWidth(); // * bulletCount;
            _paragraph.Margin(margin);
            Run bulletRun;
            bulletRun.Text(bullet + L"\t");
            _paragraph.Inlines().Append(bulletRun);
            _paragraph.TextIndent(-themes.ListGutterWidth());
        }

        void SetTaskListMask(winrt::hstring const& mask)
        {
            Run run;
			run.Text(mask);
            _paragraph.Inlines().Append(run);
        }

        void Enter() 
        {
            InlineUIContainer inlineUIContainer;
            _richTextBlock.TextWrapping(TextWrapping::Wrap);
            inlineUIContainer.Child(_richTextBlock);
            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<Inline>())
            {
                if (_richTextBlock.Blocks().Size() == 0)
                {
                    Paragraph paragraph;
                    paragraph.Inlines().Append(inlineChild);
                    _richTextBlock.Blocks().Append(paragraph);
                }
                else {
					auto index = _richTextBlock.Blocks().Size() - 1;
					auto paragraph = _richTextBlock.Blocks().GetAt(index).as<Paragraph>();
					paragraph.Inlines().Append(inlineChild);
                }

            }
            else if (auto blockChild = child->TextElement().try_as<Block>())
            {
                _richTextBlock.Blocks().Append(blockChild);
            }
        }
    };
}


