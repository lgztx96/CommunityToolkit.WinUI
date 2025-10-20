#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h> 
#include <winrt/Microsoft.UI.Xaml.Documents.h> 
#include <MarkdownTextBlock/TextElements/HtmlElementType.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Windows.UI.ViewManagement.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml::Documents;
using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::CommunityToolkit::Labs::WinUI::TextElements;

namespace winrt::CommunityToolkit::Labs::WinUI
{
	class Extensions
	{
	public:
		static std::wstring ToAlphabetical(int index, bool upper = false);

		static std::wstring ToRomanNumerals(int x, bool lower = false);

		static bool IsAtInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection);

		static TextPointer GetNextInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection);

		static std::wstring RemoveImageSize(std::wstring_view url);

		static Uri GetUri(std::wstring_view url, std::wstring_view base);

		static HtmlElementType TagToType(std::string_view tag);

		static bool IsHeading(std::wstring_view tag);

		static Size GetSvgSize(std::wstring_view svgString);

		static Size GetMarkdownImageSize(std::wstring_view link);

		static SolidColorBrush GetAccentColorBrush(UIColorType colorType = UIColorType::Accent);

		static std::wstring_view EscapeHtmlEntity(std::wstring_view entity);
	};
}