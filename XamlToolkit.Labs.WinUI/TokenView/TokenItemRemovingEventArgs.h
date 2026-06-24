#pragma once

#include "TokenItemRemovingEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs>
	{
		TokenItemRemovingEventArgs() = default;

		TokenItemRemovingEventArgs(winrt::IInspectable const& item, winrt::XamlToolkit::Labs::WinUI::TokenItem const& tokenItem) : Item(item), TokenItem(tokenItem) {};

		wil::single_threaded_property<winrt::IInspectable> Item{ nullptr };

		wil::single_threaded_property<winrt::XamlToolkit::Labs::WinUI::TokenItem> TokenItem{ nullptr };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TokenItemRemovingEventArgs : TokenItemRemovingEventArgsT<TokenItemRemovingEventArgs, implementation::TokenItemRemovingEventArgs>
	{
	};
}
