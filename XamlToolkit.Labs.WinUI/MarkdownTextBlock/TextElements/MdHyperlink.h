// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../MarkdownTextBlock.h"
#include "../Renderer/WinUIRenderer.h"
#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <string_view>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdHyperlink final : public IAddChild
    {
    private:
        winrt::Hyperlink _hyperlink;
        std::wstring_view _baseUrl;

    public:
        wil::typed_event<winrt::Hyperlink, winrt::HyperlinkClickEventArgs> ClickEvent;

        winrt::TextElement TextElement() const override
        {
            return _hyperlink;
        }

        MdHyperlink(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
        {
            _baseUrl = baseUrl;
            _hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
            _hyperlink.Foreground(renderer->MarkdownTextBlock().LinkForeground());
            _hyperlink.Click([weak = winrt::make_weak(renderer->MarkdownTextBlock())](auto& sender, auto&)
                {
                    if (auto hyperlink = sender.template try_as<winrt::Hyperlink>())
                    {
                        auto uri = hyperlink.NavigateUri();

                        if (auto markdown = weak.get())
                        {
                            auto markdownStrong = winrt::get_self<
                                winrt::XamlToolkit::Labs::WinUI::implementation::MarkdownTextBlock>(markdown)->get_strong();

                            bool handled = markdownStrong->RaiseLinkClickedEvent(uri);

                            if (handled)
                            {
                                hyperlink.NavigateUri(nullptr);
                            }
                        }
                    }
                });
        }

        void AddChild(IAddChild* child) override
        {
            try {
                if (auto inlineChild = child->TextElement().try_as<winrt::Inline>())
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


