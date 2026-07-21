// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "WinUIRenderer.h"
#include "MdTaskListCheckBox.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdListItem : public IAddChild
    {
    private:
        winrt::Paragraph _paragraph;
        winrt::RichTextBlock _richTextBlock;
        WinUIRenderer* _renderer;

    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdListItem(WinUIRenderer* renderer)
        {
            _renderer = renderer;
        }

        void SetBullet(size_t bulletCount, std::wstring_view bullet)
        {
            // Lists are plain Paragraph_s, one per item.
            // This is so that you can select across list items.
            auto themes = _renderer->Config().Themes();

            if (themes.ParagraphLineHeight() > 0)
            {
                _paragraph.LineHeight(themes.ParagraphLineHeight());
            }

            double bulletSpacing = themes.ListBulletSpacing();
            if (bulletSpacing < 0)
            {
                throw winrt::hresult_invalid_argument(L"ListBulletSpacing cannot be negative");
            }
            // Use spaces to create spacing between bullet and text based on ListBulletSpacing
            winrt::hstring spacing(std::wstring(static_cast<size_t>(bulletSpacing), L' '));
            winrt::Run bulletRun;
            bulletRun.Text(bullet + spacing);
            _paragraph.Inlines().Append(bulletRun);

            winrt::Thickness margin = themes.ParagraphMargin();
            if (bulletCount > 1) 
            {
                margin.Left += themes.ListGutterWidth();
            }

            _paragraph.Margin(margin);
        }

        void SetTaskListMask(wchar_t mask)
        {
            auto themes = _renderer->Config().Themes();
			MdTaskListCheckBox checkBox(mask);
            _paragraph.Inlines().Append(checkBox.TextElement().as<winrt::InlineUIContainer>());
            winrt::Run spacingRun;
            spacingRun.Text(L" ");
            _paragraph.Inlines().Append(spacingRun);
        }

        void Enter() 
        {
            winrt::InlineUIContainer inlineUIContainer;
            _richTextBlock.TextWrapping(TextWrapping::Wrap);
            inlineUIContainer.Child(_richTextBlock);
            _paragraph.Inlines().Append(inlineUIContainer);
        }

        void AddChild(IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<winrt::Inline>())
            {
                if (_richTextBlock.Blocks().Size() == 0)
                {
                    winrt::Paragraph paragraph;
                    paragraph.Inlines().Append(inlineChild);
                    _richTextBlock.Blocks().Append(paragraph);
                }
                else {
					auto index = _richTextBlock.Blocks().Size() - 1;
					auto paragraph = _richTextBlock.Blocks().GetAt(index).as<winrt::Paragraph>();
					paragraph.Inlines().Append(inlineChild);
                }

            }
            else if (auto blockChild = child->TextElement().try_as<winrt::Block>())
            {
                _richTextBlock.Blocks().Append(blockChild);
            }
        }
    };
}


