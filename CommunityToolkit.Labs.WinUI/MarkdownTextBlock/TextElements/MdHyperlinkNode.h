// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdHyperlink.h"
#include "MdHyperlinkButton.h"

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    class MdHyperlinkNode final : public IAddChild
    {
    private:
        std::unique_ptr<IAddChild> _hyperlink;
        std::wstring_view _url;
        std::wstring_view _baseUrl;

    public:
       // bool IsHtml() const { return _htmlNode != nullptr; }

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            if (_hyperlink) {
				return _hyperlink->TextElement();
			}
            else {
                assert(false);
				return nullptr;
            }
        }

        //MdHyperlink(std::optional<std::wstring> baseUrl)
        //{
        //    _baseUrl = baseUrl;
        //    //auto url = linkInline.GetDynamicUrl != nullptr ? linkInline.GetDynamicUrl() ? ? linkInline.Url : linkInline.Url;
        //    //_linkInline = linkInline;
        //    _hyperlink = Hyperlink();
        //    //{
        //    //    NavigateUri = Extensions.GetUri(url, baseUrl),
        //    //};
        //}

        MdHyperlinkNode(std::wstring_view url, std::wstring_view baseUrl)
        {
            _url = url;
            _baseUrl = baseUrl;
            // auto url = htmlNode.GetAttributeValue("href", "#");
            // _htmlNode = htmlNode;
            //_hyperlink = Hyperlink();
            //{
            //    NavigateUri = Extensions.GetUri(url, baseUrl),
            //};
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto image = dynamic_cast<TextElements::MdImage*>(child))
            {
				assert(_hyperlink == nullptr);
                _hyperlink = std::make_unique<MdHyperlinkButton>(_url, _baseUrl);
                _hyperlink->AddChild(child);
            }
            else if (child && child->TextElement().try_as<Inline>())
            {
                if (!_hyperlink) _hyperlink = std::make_unique<MdHyperlink>(_url, _baseUrl);
                _hyperlink->AddChild(child);
            }
            else {
				assert(false);
            }
        }
    };
}


