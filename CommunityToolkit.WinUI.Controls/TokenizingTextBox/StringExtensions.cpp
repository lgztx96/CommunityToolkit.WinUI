#include "pch.h"
#include "StringExtensions.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	bool IsNullOrWhiteSpace(winrt::hstring const& str)
	{
		if (str.empty()) return true;
		return std::all_of(str.cbegin(), str.cend(), [](wchar_t c) { return ::iswspace(c); });
	}

	std::wstring_view Trim(std::wstring_view sv)
	{
		auto first = std::ranges::find_if_not(sv, [](wchar_t c) { return ::iswspace(c); });
		auto last = std::ranges::find_if_not(sv | std::views::reverse, [](wchar_t c) { return ::iswspace(c); });
		return sv.substr(first - sv.begin(), sv.end() - last.base());
	}
}
