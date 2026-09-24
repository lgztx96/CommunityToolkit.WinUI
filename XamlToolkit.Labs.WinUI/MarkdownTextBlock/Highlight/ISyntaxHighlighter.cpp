#pragma once
#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <format>
#include <span>
#include <unordered_map>
#endif
#include "ISyntaxHighlighter.h"
#include <tree_sitter/api.h>

std::span<const uint8_t> cpp_highlight_scm();

std::span<const uint8_t> csharp_highlight_scm();

std::span<const uint8_t> bash_highlight_scm();

std::span<const uint8_t> json_highlight_scm();

std::span<const uint8_t> xml_highlight_scm();

std::span<const uint8_t> rust_highlight_scm();

std::span<const uint8_t> midl3_highlight_scm();

extern "C" const TSLanguage* tree_sitter_c_sharp();

extern "C" const TSLanguage* tree_sitter_cpp();

extern "C" const TSLanguage* tree_sitter_xml();

extern "C" const TSLanguage* tree_sitter_bash();

extern "C" const TSLanguage* tree_sitter_json();

extern "C" const TSLanguage* tree_sitter_rust();

extern "C" const TSLanguage* tree_sitter_midl3();

class TreeSitterHighlighter : public IUtf16SyntaxHighlighter
{
protected:
	const TSLanguage* language;

	TSParser* parser = nullptr;
	TSQuery* query = nullptr;
	TSQueryCursor* cursor = nullptr;

public:
	TreeSitterHighlighter(const TSLanguage* lang, std::span<const uint8_t> highlight_scm) : language(lang)
	{
		parser = ts_parser_new();
		ts_parser_set_language(parser, language);

		TSQueryError error_type;
		uint32_t error_offset;
		query = ts_query_new(language, reinterpret_cast<const char*>(highlight_scm.data()), static_cast<uint32_t>(highlight_scm.size()), &error_offset, &error_type);

		if (query == nullptr) 
		{
			throw winrt::hresult_invalid_argument(
				std::format(L"Failed to create TSQuery. Error type: {}, at offset: {}", static_cast<int>(error_type), error_offset).data());
		}

		cursor = ts_query_cursor_new();
	}

	~TreeSitterHighlighter() 
	{
		if (query) ts_query_delete(query);
		if (cursor) ts_query_cursor_delete(cursor);
		if (parser) ts_parser_delete(parser);
	}

	std::vector<Utf16HighlightToken> Highlight(std::wstring_view source, bool isDarkMode) const override 
	{
		std::vector<Utf16HighlightToken> result;
		const auto& theme = isDarkMode ? HighlightDarkTheme : HighlightLightTheme;
		TSTree* tree = ts_parser_parse_string_encoding(
			parser,
			nullptr,
			reinterpret_cast<const char*>(source.data()),
			static_cast<uint32_t>(source.size() * 2),
			TSInputEncodingUTF16LE);
		TSNode root = ts_tree_root_node(tree);

		ts_query_cursor_exec(cursor, query, root);

		TSQueryMatch match;
		size_t last = 0;
		while (ts_query_cursor_next_match(cursor, &match)) {
			for (uint16_t i = 0; i < match.capture_count; i++) {
				TSQueryCapture capture = match.captures[i];
				uint32_t length;
				const char* name = ts_query_capture_name_for_id(query, capture.index, &length);
				TSNode node = capture.node;
				uint32_t start_byte = ts_node_start_byte(node);
				uint32_t end_byte = ts_node_end_byte(node);

				size_t start = start_byte / 2;
				size_t end = end_byte / 2;
				start = std::max(last, start);
				std::wstring_view token = source.substr(start, end - start);

				std::string_view scope(name, length);
				if (end > last) {

					std::wstring color;
					if (auto c = theme.count(scope)) {
						color = theme.at(scope);
					}

					if (!result.empty() && result.back().color.empty()) {
						result.back().color = theme.at("default");
					}

					result.emplace_back(start, end - start, token, scope, color);
					last = end;
				}
				else if (end == last) {
					if (result.back().color.empty()) {
						if (auto c = theme.count(scope)) {
							result.back().color = theme.at(scope);
							result.back().scope = scope;
						}
					}
				}
			}
		}

		ts_tree_delete(tree);
		return result;
	}

	static inline const std::unordered_map<std::string_view, std::wstring_view> HighlightDarkTheme = 
	{
		{"default", L"#FFFFFF" },
		//C#, C++, XML, Bash, JSON
		{"comment", L"#6A9955"},                 // C#, C++, XML, Bash, JSON

		//C#, C++, XML, Bash
		{"keyword", L"#569CD6"},                 // C#, C++, XML, Bash
		{"keyword.modifier", L"#569CD6"},        // C++
		{"keyword.exception", L"#C586C0"},       // C++
		{"keyword.operator", L"#C586C0"},        // C++
		{"keyword.directive", L"#9b9b9b"},       // C++
		{"keyword.import", L"#9b9b9b"},          // C++
		{"keyword.type", L"#569CD6"},            // C++
		{"keyword.return", L"#569CD6"},          // C++
		{"keyword.conditional", L"#C586C0"},     // C++
		{"keyword.repeat", L"#C586C0"},          // C++
		{"keyword.conditional.ternary", L"#C586C0"}, // C++

		//C#, C++, XML, Bash
		{"type", L"#4EC9B0"},                    // C#, C++, XML
		{"type.builtin", L"#569CD6"},            // C#, C++
		{"type.definition", L"#4EC9B0"},         // C++

		//C#, C++, XML, Bash
		{"function", L"#DCDCAA"},                // C#, C++, Bash
		{"function.method", L"#DCDCAA"},         // C++
		{"function.call", L"#DCDCAA"},           // C++
		{"function.builtin", L"#DCDCAA"},        // C++
		{"constructor", L"#DCDCAA"},             // C++

		//C#, C++, XML, Bash
		{"variable", L"#9CDCFE"},                // C#, C++
		{"variable.member", L"#9CDCFE"},         // C++
		{"property", L"#9CDCFE"},                // C++, XML, Bash

		//C#, C++, XML, Bash, JSON
		{"operator", L"#D4D4D4"},                // C#, C++, XML, Bash

		{"punctuation.delimiter", L"#D4D4D4"},   // C#, C++, XML
		{"punctuation.bracket", L"#D4D4D4"},     // C#, C++, XML

		{"number", L"#B5CEA8"},                   // C#, C++, XML, Bash, JSON
		{"boolean", L"#569CD6"},                  // C++

		{"string", L"#CE9178"},                   // C#, C++, XML, Bash, JSON
		{"character", L"#CE9178"},                // C++
		{"string.special", L"#D7BA7D"},           // XML
		{"string.special.symbol", L"#D7BA7D"},    // XML

		{"constant", L"#4FC1FF"},                 // C++, Bash
		{"constant.builtin", L"#4FC1FF"},         // C#, C++, JSON, Bash
		{"constant.macro", L"#DCDCAA"},           // C++

		{"module", L"#c8c8c8"},                   // C++
		{"tag", L"#569CD6"},                      // XML element name
		{"embedded", L"#9CDCFE"},                 // XML, Bash
		{"markup", L"#D4D4D4"},                   // XML
		{"markup.raw", L"#CE9178"},               // XML
		{"markup.heading", L"#569CD6"},           // XML
		{"error", L"#F44747"},                    // XML

		{"string.raw", L"#CE9178"},               // Bash
		{"string.heredoc", L"#D69D85"},           // Bash
		{"heredoc_body", L"#D69D85"},             // Bash
		{"heredoc_start", L"#D69D85"},            // Bash
		{"function_definition", L"#DCDCAA"},      // Bash
		{"command_name", L"#DCDCAA"},             // Bash
		{"variable_name", L"#9CDCFE"},            // Bash
		{"keyword.control", L"#C586C0"},          // Bash
		{"keyword.special", L"#C586C0"},          // Bash
		{"file_descriptor", L"#B5CEA8"},          // Bash
		{"command_substitution", L"#DCDCAA"},     // Bash
		{"process_substitution", L"#DCDCAA"},     // Bash
		{"expansion", L"#9CDCFE"},                // Bash
		{"comment.block", L"#6A9955"},            // Bash
		{"comment.line", L"#6A9955"},             // Bash

		{"string.special.key", L"#9CDCFE"},       // JSON

		{"property.accessor", L"#569CD6"},        // MIDL3 get/set
		{"attribute", L"#569CD6"}                 // MIDL3 attribute name
	};

	static inline const std::unordered_map<std::string_view, std::wstring_view> HighlightLightTheme = 
	{
		{"default", L"#24292E"},                      // default text color

		// Comments: C#, C++, XML, Bash, JSON
		{"comment", L"#6A737D"},                      // C#, C++, XML, Bash, JSON

		// Keywords: C#, C++, XML, Bash
		{"keyword", L"#D73A49"},                      // C#, C++, XML, Bash
		{"keyword.modifier", L"#D73A49"},             // C++
		{"keyword.exception", L"#D73A49"},            // C++
		{"keyword.operator", L"#D73A49"},             // C++
		{"keyword.directive", L"#D73A49"},            // C++
		{"keyword.import", L"#D73A49"},               // C++
		{"keyword.type", L"#D73A49"},                 // C++
		{"keyword.return", L"#D73A49"},               // C++
		{"keyword.conditional", L"#D73A49"},          // C++
		{"keyword.repeat", L"#D73A49"},               // C++
		{"keyword.conditional.ternary", L"#D73A49"},  // C++

		// Types: C#, C++, XML, Bash
		{"type", L"#005CC5"},                         // C#, C++, XML
		{"type.builtin", L"#D73A49"},                 // C#, C++
		{"type.definition", L"#005CC5"},              // C++

		// Functions: C#, C++, XML, Bash
		{"function", L"#005CC5"},                     // C#, C++, Bash
		{"function.method", L"#005CC5"},              // C++
		{"function.call", L"#005CC5"},                // C++
		{"function.builtin", L"#005CC5"},             // C++
		{"constructor", L"#005CC5"},                  // C++

		// Variables: C#, C++, XML, Bash
		{"variable", L"#E36209"},                     // C#, C++
		{"variable.member", L"#24292E"},              // C++
		{"property", L"#24292E"},                     // C++, XML, Bash

		// Operators: C#, C++, XML, Bash, JSON
		{"operator", L"#24292E"},                     // C#, C++, XML, Bash

		{"punctuation.delimiter", L"#24292E"},        // C#, C++, XML
		{"punctuation.bracket", L"#24292E"},          // C#, C++, XML

		{"number", L"#005CC5"},                       // C#, C++, XML, Bash, JSON
		{"boolean", L"#005CC5"},                      // C++

		{"string", L"#032F62"},                       // C#, C++, XML, Bash, JSON
		{"character", L"#032F62"},                    // C++
		{"string.special", L"#6F42C1"},               // XML
		{"string.special.symbol", L"#6F42C1"},        // XML

		{"constant", L"#005CC5"},                     // C++, Bash
		{"constant.builtin", L"#005CC5"},             // C#, C++, JSON, Bash
		{"constant.macro", L"#005CC5"},               // C++

		{"module", L"#005CC5"},                       // C++
		{"tag", L"#22863A"},                          // XML element name
		{"embedded", L"#24292E"},                     // XML, Bash
		{"markup", L"#24292E"},                       // XML
		{"markup.raw", L"#032F62"},                   // XML
		{"markup.heading", L"#005CC5"},               // XML
		{"error", L"#B31D28"},                        // XML

		{"string.raw", L"#032F62"},                   // Bash
		{"string.heredoc", L"#032F62"},               // Bash
		{"heredoc_body", L"#032F62"},                 // Bash
		{"heredoc_start", L"#032F62"},                // Bash
		{"function_definition", L"#005CC5"},          // Bash
		{"command_name", L"#005CC5"},                 // Bash
		{"variable_name", L"#E36209"},                // Bash
		{"keyword.control", L"#D73A49"},              // Bash
		{"keyword.special", L"#D73A49"},              // Bash
		{"file_descriptor", L"#005CC5"},              // Bash
		{"command_substitution", L"#005CC5"},         // Bash
		{"process_substitution", L"#005CC5"},         // Bash
		{"expansion", L"#E36209"},                    // Bash
		{"comment.block", L"#6A737D"},                // Bash
		{"comment.line", L"#6A737D"},                 // Bash

		{"string.special.key", L"#005CC5"},           // JSON

		{"property.accessor", L"#005CC5"},            // MIDL3 get/set
		{"attribute", L"#6F42C1"},                    // attribute name
	};
};

class CSharpHighlighter : public TreeSitterHighlighter 
{
public:
	CSharpHighlighter() : TreeSitterHighlighter(tree_sitter_c_sharp(), csharp_highlight_scm()) { }
};

class CPlusPlusHighlighter : public TreeSitterHighlighter 
{
public:
	CPlusPlusHighlighter() : TreeSitterHighlighter(tree_sitter_cpp(), cpp_highlight_scm()) { }
};

class XmlHighlighter : public TreeSitterHighlighter
{
public:
	XmlHighlighter() : TreeSitterHighlighter(tree_sitter_xml(), xml_highlight_scm()) { }
};

class BashHighlighter : public TreeSitterHighlighter
{
public:
	BashHighlighter() : TreeSitterHighlighter(tree_sitter_bash(), bash_highlight_scm()) { }
};

class JsonHighlighter : public TreeSitterHighlighter
{
public:
	JsonHighlighter() : TreeSitterHighlighter(tree_sitter_json(), json_highlight_scm()) { }
};

class RustHighlighter : public TreeSitterHighlighter
{
public:
	RustHighlighter() : TreeSitterHighlighter(tree_sitter_rust(), rust_highlight_scm()) { }
};

class Midl3Highlighter : public TreeSitterHighlighter
{
public:
	Midl3Highlighter() : TreeSitterHighlighter(tree_sitter_midl3(), midl3_highlight_scm()) { }
};

std::unique_ptr<IUtf16SyntaxHighlighter> IUtf16SyntaxHighlighter::Create(std::wstring_view language)
{
	if (language == L"csharp" || language == L"c#")
	{
		return std::make_unique<CSharpHighlighter>();
	}

	if (language == L"json") 
	{ 
		return std::make_unique<JsonHighlighter>();
	}

	if (language == L"xml" || language == L"xaml")
	{
		return std::make_unique<XmlHighlighter>();
	}

	if (language == L"c" || language == L"c++" || language == L"cpp")
	{
		return std::make_unique<CPlusPlusHighlighter>();
	}

	if (language == L"bash" || language == L"shell")
	{
		return std::make_unique<BashHighlighter>();
	}

	if (language == L"rust")
	{
		return std::make_unique<RustHighlighter>();
	}

	if (language == L"midl3" || language == L"idl")
	{
		return std::make_unique<Midl3Highlighter>();
	}

	return nullptr;
}
