#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <string>
#include <vector>
#endif
#include "AspectRatio.h"
#if __has_include("AspectRatio.g.cpp")
#include "AspectRatio.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::XamlToolkit::WinUI::Controls::AspectRatio AspectRatio::ConvertToAspectRatio(winrt::hstring const& rawString)
	{
		using std::string_view_literals::operator""sv;

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
