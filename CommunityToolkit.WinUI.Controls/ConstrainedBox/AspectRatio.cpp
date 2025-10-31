#include "pch.h"
#include "AspectRatio.h"
#if __has_include("AspectRatio.g.cpp")
#include "AspectRatio.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	winrt::CommunityToolkit::WinUI::Controls::AspectRatio AspectRatio::ConvertToAspectRatio(winrt::hstring const& rawString)
	{
		auto sv = std::wstring_view(rawString);
		auto ratio = Split(sv, L":"sv)
			| std::ranges::views::transform([](std::wstring_view sv) { return std::wstring(sv); })
			| std::ranges::to<std::vector>();

		if (ratio.size() == 2)
		{
			double width = std::stod(ratio[0]);
			double height = std::stod(ratio[1]);

			return winrt::make<AspectRatio>(width, height);
		}
		else if (ratio.size() == 1)
		{
			return winrt::make<AspectRatio>(std::stod(ratio[0]));
		}

		return winrt::make<AspectRatio>(1);
	}

	winrt::hstring AspectRatio::ToString() const
	{
		return winrt::format(L"{}:{}", Width(), Height());
	}
}
