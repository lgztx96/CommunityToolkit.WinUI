#pragma once

#include "TokenItemRemovingEventArgs.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs>
	{
		TokenItemRemovingEventArgs() = default;
		TokenItemRemovingEventArgs(IInspectable const& item, winrt::CommunityToolkit::Labs::WinUI::TokenItem const& tokenItem) : Item(item), TokenItem(tokenItem) {};

		wil::single_threaded_property<IInspectable> Item{ nullptr };

		wil::single_threaded_property<winrt::CommunityToolkit::Labs::WinUI::TokenItem> TokenItem{ nullptr };
	};
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs, implementation::TokenItemRemovingEventArgs>
	{
	};
}
