#include "pch.h"
#include "WinUIRenderer.h"
#include <MarkdownTextBlock/TextElements/MdAutolinkInline.h>
#include <MarkdownTextBlock/TextElements/MdBlockContainer.h>
#include <MarkdownTextBlock/TextElements/MdCodeBlock.h>
#include <MarkdownTextBlock/TextElements/MdEmphasisInline.h>
#include <MarkdownTextBlock/TextElements/MdHeading.h>
#include <MarkdownTextBlock/TextElements/MdHyperlink.h>
#include <MarkdownTextBlock/TextElements/MdHyperlinkNode.h>
#include <MarkdownTextBlock/TextElements/MdImage.h>
#include <MarkdownTextBlock/TextElements/MdInlineCode.h>
#include <MarkdownTextBlock/TextElements/MdInlineText.h>
#include <MarkdownTextBlock/TextElements/MdLineBreak.h>
#include <MarkdownTextBlock/TextElements/MdList.h>
#include <MarkdownTextBlock/TextElements/MdListItem.h>
#include <MarkdownTextBlock/TextElements/MdParagraph.h>
#include <MarkdownTextBlock/TextElements/MdQuote.h>
#include <MarkdownTextBlock/TextElements/MdTable.h>
#include <MarkdownTextBlock/TextElements/MdTableCell.h>
#include <MarkdownTextBlock/TextElements/MdTaskListCheckBox.h>
#include <MarkdownTextBlock/TextElements/MdThematicBreak.h>
#include "../md4c/md4c.h"

namespace winrt::CommunityToolkit::Labs::WinUI
{
	using namespace winrt::CommunityToolkit::Labs::WinUI::TextElements;

	WinUIRenderer::WinUIRenderer(
		std::shared_ptr<TextElements::MdFlowDocument> const& document,
		MarkdownConfig const& config) : _config(config), FlowDocument(document)
	{

	}

	void WinUIRenderer::AddChildToCurrent(TextElements::IAddChild* child)
	{
		if (!_inlineStack.empty()) {
			_inlineStack.back()->AddChild(child);
		}
		else if (!_containerStack.empty()) {
			_containerStack.back()->AddChild(child);
		}
		else {
			assert(child == FlowDocument().get());
		}
	}

	void WinUIRenderer::BeginBlock(std::shared_ptr<TextElements::IAddChild> const& block)
	{
		block->Enter();
		_containerStack.emplace_back(block);
		_inlineStack.clear();
	}

	void WinUIRenderer::EndBlock()
	{
		if (_containerStack.empty()) return;

		std::shared_ptr<TextElements::IAddChild> top = _containerStack.back();
		top->Leave();
		_containerStack.pop_back();

		AddChildToCurrent(top.get());

		_inlineStack.clear();
	}

	void WinUIRenderer::BeginInlineContainer(std::shared_ptr<TextElements::IAddChild> const& inlineContainer)
	{
		inlineContainer->Enter();
		_inlineStack.emplace_back(inlineContainer);
	}

	void WinUIRenderer::EndInlineContainer()
	{
		if (_inlineStack.empty()) return;

		std::shared_ptr<TextElements::IAddChild> top = _inlineStack.back();
		top->Leave();
		_inlineStack.pop_back();

		AddChildToCurrent(top.get());
	}

	void WinUIRenderer::AddInlineLeaf(TextElements::IAddChild* leaf)
	{
		if (!_inlineStack.empty()) {
			_inlineStack.back()->AddChild(leaf);
		}
		else if (!_containerStack.empty()) {
			_containerStack.back()->AddChild(leaf);
		}
		else {
			FlowDocument()->AddChild(leaf);
		}
	}

	void WinUIRenderer::ReloadDocument()
	{
		_inlineStack.clear();
		_containerStack.clear();
		_elementCache.clear();

		FlowDocument()->RichTextBlock().Blocks().Clear();
	}

	void WinUIRenderer::PushListContext(bool ordered, bool isTight, size_t startIndex)
	{
		_listContextStack.emplace_back(ordered, isTight, startIndex);
	}

	void WinUIRenderer::PopListContext()
	{
		if (!_listContextStack.empty()) _listContextStack.pop_back();
	}

	std::wstring WinUIRenderer::CurrentListBullet()
	{
		if (_listContextStack.empty()) return {};
		auto& ctx = _listContextStack.back();
		if (ctx.ordered) {
			auto bullet = std::to_wstring(ctx.nextNumber) + L". ";
			return bullet;
		}
		else {
			int depth = static_cast<int>(_listContextStack.size()) - 1;
			int idx = std::clamp(depth, 0, 2);
			static constexpr std::wstring_view bullets[]{
				L"• ",
				L"◦ ",
				L"▪ "
			};
			return std::wstring(bullets[idx]);
		}
	}

	void WinUIRenderer::AdvanceListNumberIfOrdered()
	{
		if (!_listContextStack.empty() && _listContextStack.back().ordered)
		{
			_listContextStack.back().nextNumber++;
		}
	}

	static int enter_block_callback(MD_BLOCKTYPE type, void* detail, void* userdata)
	{
		auto renderer = static_cast<WinUIRenderer*>(userdata);

		switch (type)
		{
		case MD_BLOCK_DOC:
			renderer->BeginBlock(renderer->FlowDocument());
			break;
		case MD_BLOCK_QUOTE:
		{
			auto quote = std::make_shared<TextElements::MdQuote>(renderer->Config().Themes());
			renderer->BeginBlock(quote);
			break;
		}
		case MD_BLOCK_UL:
		{
			auto ulDetail = static_cast<const MD_BLOCK_UL_DETAIL*>(detail);
			renderer->PushListContext(false, ulDetail->is_tight != 0);
			break;
		}
		case MD_BLOCK_OL:
		{
			auto olDetail = static_cast<const MD_BLOCK_OL_DETAIL*>(detail);
			renderer->PushListContext(true, olDetail->is_tight != 0, olDetail->start);
			break;
		}
		case MD_BLOCK_LI:
		{
			auto liDetail = static_cast<const MD_BLOCK_LI_DETAIL*>(detail);

			auto item = std::make_shared<TextElements::MdListItem>(renderer);

			auto bullet = renderer->CurrentListBullet();
			if (!bullet.empty()) {
				item->SetBullet(renderer->_listContextStack.size(), bullet);
			}

			if (liDetail->is_task) {
				auto mask = winrt::format(L"[{}] ", static_cast<wchar_t>(liDetail->task_mark));
				item->SetTaskListMask(mask);
			}

			renderer->BeginBlock(item);
			break;
		}
		case MD_BLOCK_H:
		{
			auto hDetail = static_cast<const MD_BLOCK_H_DETAIL*>(detail);
			auto heading = std::make_shared<TextElements::MdHeading>(hDetail->level, renderer->Config());
			renderer->BeginInlineContainer(heading);
			break;
		}
		case MD_BLOCK_CODE:
		{
			auto isDarkMode = Application::Current().RequestedTheme() == ApplicationTheme::Dark;
			auto codeDetail = static_cast<const MD_BLOCK_CODE_DETAIL*>(detail);
			std::wstring_view language{ codeDetail->lang.text, codeDetail->lang.size };
			auto codeBlock = std::make_shared<TextElements::MdCodeBlock>(language, renderer->Config(), isDarkMode);
			renderer->BeginInlineContainer(codeBlock);
			break;
		}
		case MD_BLOCK_HTML:
			break;

		case MD_BLOCK_P:
		{
			auto paragraph = std::make_shared<TextElements::MdParagraph>(renderer);
			renderer->BeginInlineContainer(paragraph);
			break;
		}
		case MD_BLOCK_TABLE:
		{
			auto tableDetail = static_cast<const MD_BLOCK_TABLE_DETAIL*>(detail);
			auto table = std::make_shared<TextElements::MdTable>(
				tableDetail->col_count,
				tableDetail->head_row_count,
				tableDetail->body_row_count,
				renderer->Config().Themes());

			renderer->BeginBlock(table);
			renderer->_currentRow = 0;
			renderer->_currentColumn = 0;
			break;
		}
		case MD_BLOCK_TR:
			renderer->_currentColumn = 0;
			break;
		case MD_BLOCK_TH:
		case MD_BLOCK_TD:
		{
			auto tdDetail = static_cast<const MD_BLOCK_TD_DETAIL*>(detail);
			Microsoft::UI::Xaml::TextAlignment align;
			switch (tdDetail->align) {
			case MD_ALIGN_LEFT:
				align = Microsoft::UI::Xaml::TextAlignment::Left;
				break;
			case MD_ALIGN_RIGHT:
				align = Microsoft::UI::Xaml::TextAlignment::Right;
				break;
			case MD_ALIGN_CENTER:
				align = Microsoft::UI::Xaml::TextAlignment::Center;
				break;
			case MD_ALIGN_DEFAULT:
			default:
				align = Microsoft::UI::Xaml::TextAlignment::Left;
				break;
			}

			bool isHeader = (type == MD_BLOCK_TH);
			auto cell = std::make_shared<TextElements::MdTableCell>(align, isHeader, renderer->_currentColumn, renderer->_currentRow);

			renderer->BeginBlock(cell);

			renderer->_currentColumn++;
			break;
		}
		case MD_BLOCK_HR:
		{
			TextElements::MdThematicBreak hr(renderer->Config().Themes());
			renderer->AddInlineLeaf(&hr);
			break;
		}
		default:
			break;
		}

		return 0;
	}

	static int leave_block_callback(MD_BLOCKTYPE type, [[maybe_unused]] void* detail, void* userdata)
	{
		auto renderer = static_cast<WinUIRenderer*>(userdata);

		switch (type)
		{
		case MD_BLOCK_DOC:
			renderer->EndBlock();
			break;

		case MD_BLOCK_QUOTE:
			renderer->EndBlock();
			break;

		case MD_BLOCK_UL:
		case MD_BLOCK_OL:
			renderer->PopListContext();
			break;

		case MD_BLOCK_LI:
			renderer->EndBlock();
			renderer->AdvanceListNumberIfOrdered();
			break;

		case MD_BLOCK_H:
			renderer->EndInlineContainer();
			break;

		case MD_BLOCK_CODE:
			renderer->EndInlineContainer();
			break;

		case MD_BLOCK_HTML:
			break;

		case MD_BLOCK_P:
			renderer->EndInlineContainer();
			break;

		case MD_BLOCK_TABLE:
			renderer->EndBlock();
			break;

		case MD_BLOCK_TR:
			renderer->_currentRow++;
			break;

		case MD_BLOCK_TH:
		case MD_BLOCK_TD:
			renderer->EndBlock();
			break;

		case MD_BLOCK_HR:
			break;

		default:
			break;
		}

		return 0;
	}

	static int enter_span_callback(MD_SPANTYPE type, void* detail, void* userdata)
	{
		auto renderer = static_cast<WinUIRenderer*>(userdata);

		switch (type)
		{
		case MD_SPAN_EM:
		{
			auto emphasis = std::make_shared<TextElements::MdEmphasisInline>();
			emphasis->SetItalic();
			renderer->BeginInlineContainer(emphasis);
			break;
		}
		case MD_SPAN_STRONG:
		{
			auto emphasis = std::make_shared<TextElements::MdEmphasisInline>();
			emphasis->SetBold();
			renderer->BeginInlineContainer(emphasis);
			break;
		}
		case MD_SPAN_U:
		{
			auto emphasis = std::make_shared<TextElements::MdEmphasisInline>();
			emphasis->SetUnderline();
			renderer->BeginInlineContainer(emphasis);
			break;
		}
		case MD_SPAN_A:
		{
			auto spanADetail = static_cast<const MD_SPAN_A_DETAIL*>(detail);
			std::wstring_view url{ spanADetail->href.text , spanADetail->href.size };
			if (spanADetail->is_autolink) {

				auto autoLink = std::make_shared<TextElements::MdAutolinkInline>(url, renderer->Config().BaseUrl());
				renderer->BeginInlineContainer(autoLink);
			}
			else {
				auto hyperlink = std::make_shared<TextElements::MyHyperlinkNode>(url, renderer->Config().BaseUrl());
				renderer->BeginInlineContainer(hyperlink);
			}
			break;
		}
		case MD_SPAN_IMG:
		{
			auto imgDetail = static_cast<const MD_SPAN_IMG_DETAIL*>(detail);
			auto uri = Extensions::GetUri(std::wstring_view{ imgDetail->src.text, imgDetail->src.size }, renderer->Config().BaseUrl());
			auto image = std::make_shared<TextElements::MdImage>(uri, renderer->Config());
			renderer->AddInlineLeaf(image.get());
			renderer->_elementCache.emplace_back(image);
			renderer->_lastImage = image;
			break;
		}
		case MD_SPAN_CODE:
		{
			auto inlineCode = std::make_shared<TextElements::MdInlineCode>(renderer->Config());
			renderer->BeginInlineContainer(inlineCode);
			break;
		}
		case MD_SPAN_DEL:
		{
			auto emphasis = std::make_shared<TextElements::MdEmphasisInline>();
			emphasis->SetStrikeThrough();
			renderer->BeginInlineContainer(emphasis);
			break;
		}
		case MD_SPAN_LATEXMATH:
			break;
		case MD_SPAN_LATEXMATH_DISPLAY:
			break;
		case MD_SPAN_WIKILINK:
		{
			auto wikiDetail = static_cast<const struct MD_SPAN_WIKILINK*>(detail);
			auto link = std::wstring_view{ wikiDetail->target.text, wikiDetail->target.size };
			auto autoLink = std::make_shared<TextElements::MdAutolinkInline>(link, renderer->Config().BaseUrl());
			renderer->BeginInlineContainer(autoLink);
			break;
		}
		default:
			break;
		}

		return 0;
	}

	static int leave_span_callback(MD_SPANTYPE type, [[maybe_unused]] void* detail, void* userdata)
	{
		auto renderer = static_cast<WinUIRenderer*>(userdata);

		switch (type) {
		case MD_SPAN_EM:
		case MD_SPAN_STRONG:
		case MD_SPAN_U:
		case MD_SPAN_A:
		case MD_SPAN_CODE:
		case MD_SPAN_DEL:
		case MD_SPAN_WIKILINK:
			renderer->EndInlineContainer();
			break;
		case MD_SPAN_IMG:
			renderer->_lastImage = nullptr;
			break;
		case MD_SPAN_LATEXMATH:
		case MD_SPAN_LATEXMATH_DISPLAY:
			break;
		default:
			break;
		}

		return 0;
	}

	static int text_callback(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* userdata)
	{
		auto renderer = static_cast<WinUIRenderer*>(userdata);

		std::wstring_view sv{ text, size };

		switch (type)
		{
		case MD_TEXT_NULLCHAR:
			break;
		case MD_TEXT_BR:
		{
			TextElements::MdLineBreak lineBreak;
			renderer->AddInlineLeaf(&lineBreak);
		}
		break;
		case MD_TEXT_SOFTBR:
		{
			TextElements::MdInlineText textLeaf(L" ");
			renderer->AddInlineLeaf(&textLeaf);
		}
		break;
		case MD_TEXT_HTML:
		{
			break;
		}
		case MD_TEXT_ENTITY:
		{
			sv = Extensions::EscapeHtmlEntity(sv);
			TextElements::MdInlineText textLeaf(sv);
			renderer->AddInlineLeaf(&textLeaf);
		}
		break;
		case MD_TEXT_CODE:
		{
			TextElements::MdInlineText textLeaf(sv);
			renderer->AddInlineLeaf(&textLeaf);
		}
		break;
		default:
		{
			auto parent = renderer->Top();
			if (renderer->_lastImage) {
				auto image = std::dynamic_pointer_cast<TextElements::MdImage>(renderer->_lastImage);
				image->SetToolTip({ text, size });
			}
			else {
				TextElements::MdInlineText textLeaf(sv);
				renderer->AddInlineLeaf(&textLeaf);
			}
			break;
		}
		}

		return 0;
	}

	static void debug_log_callback(const char* msg, [[maybe_unused]] void* userdata)
	{
		fprintf(stderr, "MD4C: %s\n", msg);
	}

	void WinUIRenderer::Render(std::wstring_view text)
	{
		MD_PARSER parser
		{
			.abi_version = 0,
			.flags = MD_DIALECT_GITHUB | MD_FLAG_WIKILINKS,
			.enter_block = enter_block_callback,
			.leave_block = leave_block_callback,
			.enter_span = enter_span_callback,
			.leave_span = leave_span_callback,
			.text = text_callback,
			.debug_log = debug_log_callback,
			.syntax = nullptr
		};

		md_parse(text.data(), static_cast<MD_SIZE>(text.size()), &parser, this);
	}

	std::shared_ptr<TextElements::IAddChild> WinUIRenderer::Top()
	{
		if (!_inlineStack.empty()) return _inlineStack.back();
		if (!_containerStack.empty()) return _containerStack.back();
		return nullptr;
	}

	void WinUIRenderer::WriteBlock(TextElements::IAddChild* obj)
	{
		if (!_containerStack.empty()) {
			_containerStack.back()->AddChild(obj);
		}
		else {
			FlowDocument()->AddChild(obj);
		}
	}

	void WinUIRenderer::WriteInline(TextElements::IAddChild* span)
	{
		AddInlineLeaf(span);
	}

	void WinUIRenderer::WriteText(std::wstring_view text)
	{
		if (text.empty()) return;
		TextElements::MdInlineText textLeaf(text);
		AddInlineLeaf(&textLeaf);
	}

	void WinUIRenderer::AddInline(TextElements::IAddChild* parent, TextElements::IAddChild* child)
	{
		parent->AddChild(child);
	}

	void WinUIRenderer::PushTableContext(int colCount, int headRowCount, int bodyRowCount)
	{
		_tableContextStack.emplace_back(colCount, headRowCount, bodyRowCount);
	}

	void WinUIRenderer::PopTableContext()
	{
		if (_tableContextStack.empty()) return;

		_tableContextStack.pop_back();
	}

	TableContext* WinUIRenderer::CurrentTable()
	{
		if (_tableContextStack.empty()) return nullptr;
		return &_tableContextStack.back();
	}
}