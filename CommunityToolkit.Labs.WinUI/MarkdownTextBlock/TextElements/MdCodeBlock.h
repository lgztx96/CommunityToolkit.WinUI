// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../Extension.h"
#include "../Highlight/ISyntaxHighlighter.h"
#include "IAddChild.h"
#include <ranges>
#include <winrt/CommunityToolkit.Labs.WinUI.h>
#include <winrt/CommunityToolkit.WinUI.Helpers.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
	using namespace winrt;
	using namespace Windows::UI;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;

	class MdCodeBlock : public IAddChild
	{
	private:
		Paragraph _paragraph;
		TextBlock _richTextBlock;
		MarkdownConfig _config;
		std::wstring _language;
		std::wstring _sourceCode;
		bool _isDarkMode;

		Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
		{
			return _paragraph;
		}

	public:

		MdCodeBlock(std::wstring_view lang, MarkdownConfig const& config, bool isDarkMode)
			: _config(config), _language(lang), _isDarkMode(isDarkMode)
		{
			Extensions::ToLower(_language);
		}

		void Enter() override {
			InlineUIContainer container;
			Border border;
			border.Background(_config.Themes().CodeBlockBackground());
			border.BorderBrush(_config.Themes().CodeBlockBorderBrush());
			border.BorderThickness(_config.Themes().CodeBlockBorderThickness());
			border.Padding(_config.Themes().CodeBlockPadding());
			border.Margin(_config.Themes().CodeBlockMargin());
			border.CornerRadius(_config.Themes().CodeBlockCornerRadius());

			_richTextBlock.IsTextSelectionEnabled(true);
			_richTextBlock.FontFamily(_config.Themes().CodeBlockFontFamily());
			_richTextBlock.Foreground(_config.Themes().CodeBlockForeground());
			_richTextBlock.TextWrapping(TextWrapping::Wrap);

			border.Child(_richTextBlock);
			container.Child(border);
			_paragraph.Inlines().Append(container);
		}

		IUtf16SyntaxHighlighter* GetOrCreate(std::wstring_view key) {
			static std::unordered_map<std::wstring_view, std::unique_ptr<IUtf16SyntaxHighlighter>> pool;

			auto it = pool.find(key);
			if (it != pool.end()) {
				return it->second.get();
			}

			auto highlighter = IUtf16SyntaxHighlighter::Create(key);
			if (highlighter == nullptr) {
				return nullptr;
			}
			IUtf16SyntaxHighlighter* ptr = highlighter.get();
			pool.emplace(key, std::move(highlighter));
			return ptr;
		}

		void Leave() override {
			if (_sourceCode.empty() || Extensions::IsWhiteSpace(_sourceCode))
			{
				return;
			}

			if (_sourceCode.back() == L'\n') _sourceCode.pop_back();

			auto highlighter = GetOrCreate(_language);

			if (highlighter == nullptr) {
				_richTextBlock.Text(_sourceCode);
				return;
			}

			FormatInlines(_sourceCode, highlighter, _richTextBlock.Inlines());
		}

		void FormatInlines(std::wstring_view source, const IUtf16SyntaxHighlighter* highlighter, InlineCollection const& inlines) const {
		
			const auto tokens = highlighter->Highlight(source, _isDarkMode);
			size_t lastPos = 0;
			for (const auto& token : tokens) {

				if (token.start > lastPos) {
					std::wstring_view plain = source.substr(lastPos, token.start - lastPos);
					Run run;
					run.Text(hstring(plain));
					inlines.Append(run);
				}

				std::wstring_view snippet = source.substr(token.start, token.length);
				Run run;
				run.Text(hstring(snippet));
				run.Foreground(SolidColorBrush(CommunityToolkit::WinUI::Helpers::ColorHelper::ToColor(token.color)));
				inlines.Append(run);

				lastPos = token.start + token.length;
			}

			if (lastPos < source.size()) {
				std::wstring_view plain = source.substr(lastPos);
				Run run;
				run.Text(hstring(plain));
				inlines.Append(run);
			}
		}

		void AddInlineText(std::wstring_view code) override {
			_sourceCode.append(code);
		}

		void AddChild(TextElements::IAddChild* child) override
		{
			if (auto element = child->TextElement())
			{
				if (auto run = element.try_as<Run>()) {
					AddInlineText(run.Text());
				}
			}
		}
	};
}
