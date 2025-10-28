#pragma once

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	bool IsNullOrWhiteSpace(winrt::hstring const& str);

	std::wstring_view Trim(std::wstring_view sv);

	template <typename charT>
	constexpr auto Split(std::basic_string_view<charT> view, std::basic_string_view<charT> delim)
	{
		return std::views::split(view, delim) | std::views::transform([](auto&& range)
			{
				return std::basic_string_view<charT>(range);
			});
	}
}
