// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    class MdHyperlink final : public IAddChild
    {
    private:
        Hyperlink _hyperlink;
        std::wstring_view _baseUrl;
        Hyperlink::Click_revoker _clickRevoker;

    public:
       // bool IsHtml() const { return _htmlNode != nullptr; }

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _hyperlink;
        }

        MdHyperlink(std::wstring_view url, std::wstring_view baseUrl)
        {
            _baseUrl = baseUrl;
            // auto url = htmlNode.GetAttributeValue("href", "#");
            // _htmlNode = htmlNode;

            _hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
            _hyperlink.Foreground(MarkdownConfig::Default().Themes().LinkForeground());
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


