// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../MarkdownTextBlock.h"
#include "../Renderer/WinUIRenderer.h"
#include "IAddChild.h"
#include "Mdimage.h"
#include "MdInlineText.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml::Documents;

	class MdAutolinkInline final : public IAddChild
	{
	private:
		Hyperlink _hyperlink;

	public:
		Microsoft::UI::Xaml::Documents::TextElement TextElement() const override 
		{
			return _hyperlink;
		}

		MdAutolinkInline(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
		{
			_hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
			_hyperlink.Click([markdownWeak{ renderer->MarkdownTextBlock() }](auto& sender, auto&)
				{
					if (auto hyperlink = sender.template try_as<Hyperlink>())
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
		}

		void AddChild(TextElements::IAddChild* child) override
		{
			try
			{
				if (auto text = dynamic_cast<const MdInlineText*>(child)) {
					_hyperlink.Inlines().Append(text->TextElement().as<Run>());
				}
			}
			catch (const winrt::hresult_error& ex)
			{
				throw winrt::hresult_illegal_method_call(std::format(L"Error adding child to MdAutolinkInline: {}", ex.message()).c_str());
			}
		}
	};
}


