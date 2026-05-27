#pragma once

#include "TokenItemRemovingEventArgs.g.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs>
	{
		TokenItemRemovingEventArgs() = default;
		TokenItemRemovingEventArgs(IInspectable const& item, winrt::XamlToolkit::Labs::WinUI::TokenItem const& tokenItem) : Item(item), TokenItem(tokenItem) {};

		wil::single_threaded_property<IInspectable> Item{ nullptr };

		wil::single_threaded_property<winrt::XamlToolkit::Labs::WinUI::TokenItem> TokenItem{ nullptr };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs, implementation::TokenItemRemovingEventArgs>
	{
	};
}
