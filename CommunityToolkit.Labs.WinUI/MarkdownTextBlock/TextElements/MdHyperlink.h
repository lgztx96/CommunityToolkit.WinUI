// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <winrt/Windows.Foundation.Collections.h>
#include "../Renderer/WinUIRenderer.h"
#include "../MarkdownTextBlock.h"

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    class MdHyperlink final : public IAddChild
    {
    private:
        Hyperlink _hyperlink;
        std::wstring_view _baseUrl;

    public:
       // bool IsHtml() const { return _htmlNode != nullptr; }
        wil::typed_event<Hyperlink, HyperlinkClickEventArgs> ClickEvent;

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _hyperlink;
        }

        MdHyperlink(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
        {
            _baseUrl = baseUrl;
            // auto url = htmlNode.GetAttributeValue("href", "#");
            // _htmlNode = htmlNode;

            _hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
            _hyperlink.Foreground(MarkdownConfig::Default().Themes().LinkForeground());
            _hyperlink.Click([weakMarkdown{ renderer->MarkdownTextBlock() }](auto& sender, auto&)
                {
                    if (auto hyperlink = sender.template try_as<Hyperlink>())
                    {
                        const auto uri = hyperlink.NavigateUri();

                        if (auto markdown = weakMarkdown.get())
                        {
                            auto markdownStrong = winrt::get_self<
                                winrt::CommunityToolkit::Labs::WinUI::implementation::MarkdownTextBlock>(markdown)->get_strong();

                            const bool handled = markdownStrong->RaiseLinkClickedEvent(uri);

                            if (handled)
                            {
                                hyperlink.NavigateUri(nullptr);
                            }
                        }
                    }
                });
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            try {
                if (auto inlineChild = child->TextElement().try_as<Inline>())
                {
                    _hyperlink.Inlines().Append(inlineChild);
                }
            }
            catch (const winrt::hresult_error&) 
            {
                // todo [Changelog for `libllama` API](https://github.com/ggml-org/llama.cpp/issues/9289) not working
            }
        }
    };
}


