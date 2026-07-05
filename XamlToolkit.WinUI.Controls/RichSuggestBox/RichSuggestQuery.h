#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Text.h>
#include <optional>
#include <ppl.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RichSuggestQuery
	{
		RichSuggestQuery() = default;

		std::optional<winrt::hstring> Prefix;

		std::optional<winrt::hstring> QueryText;

		winrt::Microsoft::UI::Text::ITextRange Range{ nullptr };

		std::optional<concurrency::cancellation_token_source> CancellationTokenSource;
	};
}
