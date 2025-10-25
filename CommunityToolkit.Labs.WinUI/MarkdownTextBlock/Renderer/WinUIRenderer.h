
#pragma once

#include "../md4c/md4c.h"
#include <MarkdownTextBlock/TextElements/IAddChild.h>
#include <MarkdownTextBlock/TextElements/MdFlowDocument.h>
#include <vector>
#include <wil/cppwinrt_authoring.h>
#include <wil/wistd_type_traits.h>
#include <winrt/CommunityToolkit.Labs.WinUI.h>

namespace winrt::CommunityToolkit::Labs::WinUI
{
	using namespace winrt::CommunityToolkit::Labs::WinUI::TextElements;

	struct ListContext {
		bool ordered;
		bool isTight;
		size_t nextNumber;

		ListContext() = default;

		ListContext(bool ordered, bool isTight, size_t startIndex = 1)
			: ordered(ordered), isTight(isTight), nextNumber(startIndex) {}
	};

	struct TableContext {
		int colCount;
		int headRowCount;
		int bodyRowCount;
		int currentRow = 0;
		int currentColumn = 0;

		TableContext() = default;

		TableContext(int colCount, int headRowCount, int bodyRowCount)
			: colCount(colCount), headRowCount(headRowCount), bodyRowCount(bodyRowCount),
			currentRow(0), currentColumn(0) { }
	};

	struct WinUIRenderer
	{
		CommunityToolkit::Labs::WinUI::MarkdownConfig _config = MarkdownConfig::Default();

		winrt::weak_ref<CommunityToolkit::Labs::WinUI::MarkdownTextBlock> MarkdownTextBlock();

		wil::single_threaded_rw_property<std::shared_ptr<TextElements::MdFlowDocument>> FlowDocument;

		CommunityToolkit::Labs::WinUI::MarkdownConfig Config() const noexcept { return _config; }
		void Config(CommunityToolkit::Labs::WinUI::MarkdownConfig const& value) { _config = value; }

		WinUIRenderer(
			std::shared_ptr<TextElements::MdFlowDocument> const& document,
			MarkdownConfig const& config,
			CommunityToolkit::Labs::WinUI::MarkdownTextBlock const& markdownTextBlock);

		void Render(std::wstring_view text);

		void ReloadDocument();

	private:
		std::shared_ptr<TextElements::IAddChild> _lastImage;

		std::vector<std::shared_ptr<TextElements::IAddChild>> _containerStack;

		std::vector<std::shared_ptr<TextElements::IAddChild>> _inlineStack;

		std::vector<ListContext> _listContextStack;

		std::vector<TableContext> _tableContextStack;

		std::vector<std::shared_ptr<TextElements::IAddChild>> _elementCache;

		winrt::weak_ref<CommunityToolkit::Labs::WinUI::MarkdownTextBlock> _markdownTextBlock;

		int _currentRow = 0;
		int _currentColumn = 0;

		std::shared_ptr<TextElements::IAddChild> Top();

		void WriteBlock(TextElements::IAddChild* block);

		void WriteInline(TextElements::IAddChild* span);

		void WriteText(std::wstring_view text);

		static void AddInline(TextElements::IAddChild* parent, TextElements::IAddChild* child);

		void BeginBlock(std::shared_ptr<TextElements::IAddChild> const& block);

		void EndBlock();

		void AddChildToCurrent(TextElements::IAddChild* run);

		void BeginInlineContainer(std::shared_ptr<TextElements::IAddChild> const& inlineContainer);
		
		void PushListContext(bool ordered, bool isTight, size_t startIndex = 0);

		void PopListContext();

		std::wstring CurrentListBullet();

		void AdvanceListNumberIfOrdered();

		void EndInlineContainer();

		void AddInlineLeaf(TextElements::IAddChild* leaf);

		void PushTableContext(int colCount, int headRowCount, int bodyRowCount);

		void PopTableContext();

		TableContext* CurrentTable();

		static int EnterBlockCallback(MD_BLOCKTYPE type, void* detail, void* userdata);

		static int LeaveBlockCallback(MD_BLOCKTYPE type, void* detail, void* userdata);

		static int EnterSpanCallback(MD_SPANTYPE type, void* detail, void* userdata);

		static int LeaveSpanCallback(MD_SPANTYPE type, void* detail, void* userdata);

		static int MdTextCallback(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* userdata);
	};
}


