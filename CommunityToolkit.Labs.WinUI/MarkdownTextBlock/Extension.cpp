#include "pch.h"
#include "Extension.h"
#include <ranges>
#include <regex>
#include <cwctype>
#include <winrt/Windows.Data.Xml.Dom.h>

using namespace winrt::Windows::Data::Xml::Dom;

namespace winrt::CommunityToolkit::Labs::WinUI
{
	void Extensions::ToLower(std::wstring& str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });
	}

	void Extensions::ToUpper(std::wstring& str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](wchar_t c) { return static_cast<wchar_t>(std::towupper(c)); });
	}

	bool Extensions::IsWhiteSpace(std::wstring_view sv)
	{
		return std::all_of(sv.cbegin(), sv.cend(), [](wchar_t c) { return iswspace(c); });
	}

	std::wstring Extensions::ToAlphabetical(int index, bool upper)
	{
		static constexpr auto alphabetical = L"abcdefghijklmnopqrstuvwxyz";
		auto remainder = index;
		std::wstring stringBuilder;
		while (remainder != 0)
		{
			if (remainder > 26)
			{
				auto newRemainder = remainder % 26;
				auto i = (remainder - newRemainder) / 26;
				stringBuilder.append(1, alphabetical[i - 1]);
				remainder = newRemainder;
			}
			else
			{
				stringBuilder.append(1, alphabetical[remainder - 1]);
				remainder = 0;
			}
		}

		if (upper) {
			ToUpper(stringBuilder);
		}

		return stringBuilder;
	}

	std::wstring Extensions::ToRomanNumerals(int x, bool lower) {
		if (x <= 0)
			return L"Negative or zero!";
		auto roman_digit = [](wchar_t one, wchar_t five, wchar_t ten, int x) {
			if (x <= 3)
				return std::wstring(x, one);
			if (x <= 5)
				return std::wstring(5 - x, one) + five;
			if (x <= 8)
				return five + std::wstring(x - 5, one);
			return std::wstring(10 - x, one) + ten;
			};
		if (x >= 1000)
			return x - 1000 > 0 ? L"M" + ToRomanNumerals(x - 1000) : L"M";
		if (x >= 100) {
			auto s = roman_digit(L'C', L'D', L'M', x / 100);
			return x % 100 > 0 ? s + ToRomanNumerals(x % 100) : s;
		}
		if (x >= 10) {
			auto s = roman_digit(L'X', L'L', L'C', x / 10);
			return x % 10 > 0 ? s + ToRomanNumerals(x % 10) : s;
		}

		auto ret = roman_digit(L'I', L'V', L'X', x);

		if (lower) {
			ToLower(ret);
		}

		return ret;
	}

	TextPointer Extensions::GetNextInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection)
	{
		// Check if the current position is already an insertion position
		if (IsAtInsertionPosition(position, logicalDirection))
		{
			// Return the same position
			return position;
		}
		else
		{
			// Try to find the next insertion position by moving one symbol forward
			TextPointer next = position.GetPositionAtOffset(1, logicalDirection);
			// If there is no next position, return null
			if (next == nullptr)
			{
				return nullptr;
			}
			else
			{
				// Recursively call method until an insertion position is found or null is returned
				return GetNextInsertionPosition(next, logicalDirection);
			}
		}
	}

	bool Extensions::IsAtInsertionPosition(TextPointer const& position, LogicalDirection logicalDirection)
	{
		// Get the character rect of the current position
		Rect currentRect = position.GetCharacterRect(logicalDirection);
		// Try to get the next position by moving one symbol forward
		TextPointer next = position.GetPositionAtOffset(1, logicalDirection);
		// If there is no next position, return false
		if (next == nullptr)
		{
			return false;
		}
		else
		{
			// Get the character rect of the next position
			Rect nextRect = next.GetCharacterRect(logicalDirection);
			// Compare the two rects and return true if they are different
			return currentRect != nextRect;
		}
	}

	std::wstring Extensions::RemoveImageSize(std::wstring_view url)
	{
		if (url.empty())
		{
			throw winrt::hresult_invalid_argument(L"URL must not be null or empty");
		}

		// Create a regex pattern to match the URL with width and height
		static constexpr auto pattern = LR"(([^)\s]+)\s*=\s*\d+x\d+\s*)";
		static const std::wregex re(pattern);

		// Replace the matched URL with the URL only
		std::wstring result;
		std::regex_replace(std::back_inserter(result), url.begin(), url.end(), re, L"$1");

		return result;
	}

	Uri Extensions::GetUri(std::wstring_view url, std::wstring_view base) {
		std::wstring validUrl = RemoveImageSize(url);

		try {
			//the url is already absolute
			Uri result{ validUrl };
			return result;
		}
		catch (...) {}

		if (!IsWhiteSpace(base)) {
			//the url is relative, so append the base
			//trim any trailing "/" from the base and any leading "/" from the url
			base = base.substr(0, base.find_last_not_of(L'/') + 1);
			validUrl.erase(0, validUrl.find_first_not_of(L'/'));
			return Uri(winrt::format(L"{}/{}", base, validUrl));
		}
		else {
			//the url is relative to the file system
			//add ms-appx
			validUrl.erase(0, validUrl.find_first_not_of(L'/'));
			return Uri(L"ms-appx:///" + validUrl);
		}
	}

	HtmlElementType Extensions::TagToType(std::string_view tag)
	{
		static constexpr std::array<std::string_view, 34> blockTags
		{
			"address", "article", "aside", "details", "blockquote",
			"canvas", "dd", "div", "dl", "dt", "fieldset", "figcaption",
			"figure", "footer", "form", "h1", "h2", "h3", "h4", "h5",
			"h6", "header", "hr", "li", "main", "nav", "noscript",
			"ol", "p", "pre", "section", "table", "tfoot", "ul"
		};

		std::string lower(tag);
		std::transform(lower.begin(), lower.end(), lower.begin(),
			[](char c) { return static_cast<char>(std::tolower(c)); });

		return std::binary_search(blockTags.begin(), blockTags.end(), lower)
			? HtmlElementType::Block
			: HtmlElementType::Inline;
	}

	bool Extensions::IsHeading(std::wstring_view tag)
	{
		std::wstring copy{ tag };
		ToLower(copy);
		static constexpr std::array<std::wstring_view, 6> headings = { L"h1", L"h2", L"h3", L"h4", L"h5", L"h6" };
		return std::ranges::contains(headings, std::wstring_view{ copy.data(), copy.size() });
	}

	Size Extensions::GetSvgSize(std::wstring_view svgString)
	{
		// Parse the SVG string as an XML document
		XmlDocument svgDocument;
		svgDocument.LoadXml(svgString);

		XmlElement svgElement = svgDocument.DocumentElement();

		XmlAttribute heightAttribute = svgElement.GetAttributeNode(L"height");

		XmlAttribute widthAttribute = svgElement.GetAttributeNode(L"width");

		// Convert the attribute values to double
		double height = _wtof(heightAttribute.Value().c_str());
		double width = _wtof(widthAttribute.Value().c_str());

		// Return the height and width as a tuple
		return Size(static_cast<float>(width), static_cast<float>(height));
	}

	Size Extensions::GetMarkdownImageSize(std::wstring_view url)
	{
		using namespace std::string_view_literals;
		//if (link == nullptr || !link.IsImage)
		//{
		//    throw winrt::hresult_invalid_argument(L"Link must be an image");
		//}

		if (url.empty())
		{
			throw winrt::hresult_invalid_argument(L"Link must have a valid URL");
		}

		// Try to parse the width and height from the URL
		auto parts = std::ranges::views::split(url, L"="sv)
			| std::views::transform([](auto&& range) { return std::wstring_view(range); })
			| std::ranges::to<std::vector>();

		if (parts.size() == 2)
		{
			auto dimensions = std::ranges::views::split(parts[1], L"x"sv)
				| std::views::transform([](auto&& range) { return std::wstring_view(range); })
				| std::ranges::to<std::vector>();

			if (dimensions.size() == 2)
			{
				int width = _wtoi(dimensions[0].data());
				int height = _wtoi(dimensions[1].data());
				return Size(static_cast<float>(width), static_cast<float>(height));
			}
		}

		// not using one as it's seems to be from the HTML renderer
		//// Try to parse the width and height from the special attributes
		//var attributes = link.GetAttributes();
		//if (attributes != null && attributes.Properties != null)
		//{
		//    var width = attributes.Properties.FirstOrDefault(p => p.Key == "width")?.Value;
		//    var height = attributes.Properties.FirstOrDefault(p => p.Key == "height")?.Value;
		//    if (!string.IsNullOrEmpty(width) && !string.IsNullOrEmpty(height) && int.TryParse(width, out int w) && int.TryParse(height, out int h))
		//    {
		//        return new(w, h);
		//    }
		//}

		// Return default values if no width and height are found
		return { 0, 0 };
	}

	SolidColorBrush Extensions::GetAccentColorBrush(UIColorType colorType)
	{
		// Create a UISettings object to get the accent color
		UISettings uiSettings;

		// Get the accent color as a Color value
		auto accentColor = uiSettings.GetColorValue(colorType);

		// Create a SolidColorBrush from the accent color
		SolidColorBrush accentBrush(accentColor);

		return accentBrush;
	}

	std::wstring_view Extensions::EscapeHtmlEntity(std::wstring_view entity) {
		static const std::unordered_map<std::wstring_view, std::wstring_view> entities = {
			{L"&nbsp;",  L"\u00A0"},
			{L"&lt;",    L"\u003C"},
			{L"&gt;",    L"\u003E"},
			{L"&amp;",   L"\u0026"},
			{L"&quot;",  L"\u0022"},
			{L"&apos;",  L"\u0027"},
			{L"&cent;",  L"\u00A2"},
			{L"&pound;", L"\u00A3"},
			{L"&yen;",   L"\u00A5"},
			{L"&euro;",  L"\u20AC"},
			{L"&copy;",  L"\u00A9"},
			{L"&reg;",   L"\u00AE"},
			{L"&trade;", L"\u2122"},
			{L"&hellip;",L"\u2026"},
			{L"&mdash;", L"\u2014"},
			{L"&ndash;", L"\u2013"},
			{L"&lsquo;", L"\u2018"},
			{L"&rsquo;", L"\u2019"},
			{L"&ldquo;", L"\u201C"},
			{L"&rdquo;", L"\u201D"},
			{L"&bull;",  L"\u2022"},
			{L"&middot;",L"\u00B7"},
			{L"&para;",  L"\u00B6"},
			{L"&sect;",  L"\u00A7"}
		};

		if (const auto it = entities.find(entity); it != entities.end()) {
			return it->second;
		}

		return entity;
	}
}