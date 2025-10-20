// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

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
		Hyperlink::Click_revoker _clickRevoker;

	public:
		Microsoft::UI::Xaml::Documents::TextElement TextElement() const override 
		{
			return _hyperlink;
		}

		wil::typed_event<Hyperlink, HyperlinkClickEventArgs> ClickEvent;

		MdAutolinkInline(std::wstring_view url, std::wstring_view baseUrl)
		{
			_hyperlink.NavigateUri(Extensions::GetUri(url, baseUrl));
			_clickRevoker = _hyperlink.Click(winrt::auto_revoke,
				[this](auto& sender, auto& args) { ClickEvent.invoke(sender, args); });
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


