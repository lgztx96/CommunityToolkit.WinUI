#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Text.h>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RichSuggestQuery
	{
		RichSuggestQuery() = default;

		std::optional<winrt::hstring> Prefix;

		std::optional<winrt::hstring> QueryText;

		Microsoft::UI::Text::ITextRange Range{ nullptr };

		winrt::Windows::Foundation::IAsyncAction Task { nullptr };
	};
}
