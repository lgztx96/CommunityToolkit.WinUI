#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "StringExtensions.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	bool IsNullOrWhiteSpace(winrt::hstring const& str)
	{
		if (str.empty()) return true;
		return std::all_of(str.cbegin(), str.cend(), [](wchar_t c) { return ::iswspace(c); });
	}

	std::wstring_view Trim(std::wstring_view sv)
	{
		const auto first = std::ranges::find_if_not(sv, [](wchar_t c) { return ::iswspace(c); });
		if (first == sv.end())
		{
			return {};
		}

		const auto last = std::ranges::find_if_not(sv | std::views::reverse, [](wchar_t c) { return ::iswspace(c); });
		return sv.substr(first - sv.begin(), last.base() - first);
	}
}
