#pragma once

#include <MarkdownTextBlock/TextElements/HtmlElementType.h>

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#endif

namespace winrt 
{
	using namespace Windows::Foundation;
	using namespace Windows::Data::Xml::Dom;
	using namespace Windows::UI::ViewManagement;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace XamlToolkit::Labs::WinUI::TextElements;
}

namespace winrt::XamlToolkit::Labs::WinUI
{
	class Extensions
	{
	public:
		static void ToLower(std::wstring& str);

		static void ToUpper(std::wstring& str);

		static bool IsWhiteSpace(std::wstring_view sv);

		static std::wstring ToAlphabetical(int index, bool upper = false);

		static std::wstring ToRomanNumerals(int x, bool lower = false);

		static bool IsAtInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection);

		static TextPointer GetNextInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection);

		static std::wstring_view RemoveImageSize(std::wstring_view url);

		static Uri GetUri(std::wstring_view url, std::wstring_view base);

		static HtmlElementType TagToType(std::string_view tag);

		static bool IsHeading(std::wstring_view tag);

		static Size GetSvgSize(std::wstring_view svgString);

		static Size GetMarkdownImageSize(std::wstring_view link);

		static SolidColorBrush GetAccentColorBrush(UIColorType colorType = UIColorType::Accent);

		static std::wstring_view EscapeHtmlEntity(std::wstring_view entity);
	};
}
