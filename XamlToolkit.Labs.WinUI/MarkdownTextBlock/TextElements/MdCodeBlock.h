// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../Extension.h"
#include "../Highlight/ISyntaxHighlighter.h"
#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
	class MdCodeBlock : public IAddChild
	{
	private:
		winrt::Paragraph _paragraph;
		winrt::TextBlock _richTextBlock;
		MarkdownTextBlock _control;
		std::wstring _language;
		std::wstring _sourceCode;
		bool _isDarkMode;

		winrt::TextElement TextElement() const override
		{
			return _paragraph;
		}

	public:

		MdCodeBlock(std::wstring_view lang, MarkdownTextBlock const& control, bool isDarkMode)
			: _control(control), _language(lang), _isDarkMode(isDarkMode)
		{
			Extensions::ToLower(_language);
		}

		void Enter() override 
		{
			winrt::InlineUIContainer container;
			winrt::Border border;
			border.Background(_control.CodeBlockBackground());
			border.BorderBrush(_control.CodeBlockBorderBrush());
			border.BorderThickness(_control.CodeBlockBorderThickness());
			border.Padding(_control.CodeBlockPadding());
			border.Margin(_control.CodeBlockMargin());
			border.CornerRadius(_control.CodeBlockCornerRadius());

			_richTextBlock.IsTextSelectionEnabled(_control.IsTextSelectionEnabled());
			_richTextBlock.FlowDirection(winrt::FlowDirection::LeftToRight);
			_richTextBlock.FontFamily(_control.CodeBlockFontFamily());
			_richTextBlock.Foreground(_control.CodeBlockForeground());
			_richTextBlock.TextWrapping(winrt::TextWrapping::Wrap);

			border.Child(_richTextBlock);
			container.Child(border);
			_paragraph.Inlines().Append(container);
		}

		IUtf16SyntaxHighlighter* GetOrCreate(std::wstring_view key) 
		{
			struct wstring_hash
			{
				using is_transparent = void;
				size_t operator()(std::wstring_view str) const { return std::hash<std::wstring_view>{}(str); }
				size_t operator()(const std::wstring& str) const { return std::hash<std::wstring_view>{}(str); }
			};

			static std::unordered_map<std::wstring, std::unique_ptr<IUtf16SyntaxHighlighter>, wstring_hash, std::equal_to<>> pool;

			if (auto it = pool.find(key); it != pool.end())
			{
				return it->second.get();
			}

			if (auto highlighter = IUtf16SyntaxHighlighter::Create(key))
			{
				IUtf16SyntaxHighlighter* ptr = highlighter.get();
				pool.emplace(key, std::move(highlighter));
				return ptr;
			}
			
			return nullptr;
		}

		void Leave() override 
		{
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

		void FormatInlines(
			std::wstring_view source, 
			const IUtf16SyntaxHighlighter* highlighter, 
			winrt::InlineCollection const& inlines) const 
		{
		
			const auto tokens = highlighter->Highlight(source, _isDarkMode);
			size_t lastPos = 0;
			for (const auto& token : tokens) {

				if (token.start > lastPos) {
					std::wstring_view plain = source.substr(lastPos, token.start - lastPos);
					winrt::Run run;
					run.Text(winrt::hstring(plain));
					inlines.Append(run);
				}

				std::wstring_view snippet = source.substr(token.start, token.length);
				winrt::Run run;
				run.Text(winrt::hstring(snippet));
				run.Foreground(winrt::SolidColorBrush(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(token.color)));
				inlines.Append(run);

				lastPos = token.start + token.length;
			}

			if (lastPos < source.size()) {
				std::wstring_view plain = source.substr(lastPos);
				winrt::Run run;
				run.Text(winrt::hstring(plain));
				inlines.Append(run);
			}
		}

		void AddInlineText(std::wstring_view code) override 
		{
			_sourceCode.append(code);
		}

		void AddChild(IAddChild* child) override
		{
			if (auto element = child->TextElement())
			{
				if (auto run = element.try_as<winrt::Run>()) 
				{
					AddInlineText(run.Text());
				}
			}
		}
	};
}
