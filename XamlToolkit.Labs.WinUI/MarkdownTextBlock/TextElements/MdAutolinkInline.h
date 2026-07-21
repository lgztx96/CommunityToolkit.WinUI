// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../MarkdownTextBlock.h"
#include "../Renderer/WinUIRenderer.h"
#include "IAddChild.h"
#include "Mdimage.h"
#include "MdInlineText.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Documents;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
	class MdAutolinkInline final : public IAddChild
	{
	private:
		Hyperlink _hyperlink;

	public:
		winrt::TextElement TextElement() const override
		{
			return _hyperlink;
		}

		MdAutolinkInline(std::wstring_view url, std::wstring_view baseUrl, WinUIRenderer* renderer)
		{
			_hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
			_hyperlink.Foreground(renderer->Config().Themes().LinkForeground());
			_hyperlink.Click([markdownWeak{ renderer->MarkdownTextBlock() }](auto& sender, auto&)
				{
					if (auto hyperlink = sender.template try_as<Hyperlink>())
					{
						auto uri = hyperlink.NavigateUri();

						if (auto markdown = markdownWeak.get())
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


