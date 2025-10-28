// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../MarkdownTextBlock.h"
#include "../Renderer/WinUIRenderer.h"
#include "IAddChild.h"
#include "MdFlowDocument.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
	class MdHyperlinkButton final : public IAddChild
	{
	private:
		HyperlinkButton _hyperLinkButton;
		InlineUIContainer _inlineUIContainer;
		std::unique_ptr<MdFlowDocument> _flowDoc;
		std::wstring_view _baseUrl;

	public:
		//bool IsHtml() const { return _htmlNode != nullptr; }

		Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
		{
			return _inlineUIContainer;
		}

		MdHyperlinkButton(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
		{
			_baseUrl = baseUrl;
			Init(url, baseUrl, renderer);
		}

		//MdHyperlinkButton(std::wstring_view baseUrl)
		//{
		//    _baseUrl = baseUrl;
		//   /* _htmlNode = htmlNode;
		//    auto url = htmlNode.GetAttributeValue("href", "#");*/
		//    Init({}, baseUrl);
		//}

		void Init(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
		{
			_hyperLinkButton.NavigateUri(Extensions::GetUri(url, baseUrl));
			_hyperLinkButton.Padding(winrt::Microsoft::UI::Xaml::Thickness(0, 0, 0, 0));
			_hyperLinkButton.Margin(winrt::Microsoft::UI::Xaml::Thickness(0, 0, 0, 0));
			/* if (IsHtml() && _htmlNode != nullptr)
			 {
				 _flowDoc = std::make_unique<MdFlowDocument>(_htmlNode);
			 }
			 else if (_linkInline != nullptr)
			 {
				 _flowDoc = std::make_unique<MdFlowDocument>(_linkInline);
			 }*/
			_hyperLinkButton.Click([markdownWeak{ renderer->MarkdownTextBlock() }](auto& sender, auto&)
				{
					if (auto hyperlink = sender.template try_as<HyperlinkButton>())
					{
						auto uri = hyperlink.NavigateUri();

						if (auto markdown = markdownWeak.get())
						{
							auto markdownStrong = winrt::get_self<
								winrt::CommunityToolkit::Labs::WinUI::implementation::MarkdownTextBlock>(markdown)->get_strong();

							bool handled = markdownStrong->RaiseLinkClickedEvent(uri);

							if (handled)
							{
								hyperlink.NavigateUri(nullptr);
							}
						}
					}
				});
			_flowDoc = std::make_unique<MdFlowDocument>();
			_inlineUIContainer.Child(_hyperLinkButton);
			_flowDoc->RichTextBlock().Foreground(MarkdownConfig().Default().Themes().LinkForeground());
			_hyperLinkButton.Content(_flowDoc->RichTextBlock());
		}

		void AddChild(TextElements::IAddChild* child) override
		{
			if (_flowDoc) _flowDoc->AddChild(child);
		}
	};
}


