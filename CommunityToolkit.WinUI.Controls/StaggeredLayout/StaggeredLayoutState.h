#pragma once

#include "StaggeredColumnLayout.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct StaggeredLayoutState : winrt::implements<StaggeredLayoutState, winrt::Windows::Foundation::IInspectable>
	{
	public:
		double ColumnWidth() const noexcept { return _columnWidth; }
		void ColumnWidth(double value) { _columnWidth = value; }
		int NumberOfColumns() const noexcept { return static_cast<int>(_columnLayout.size()); }
		void ClearColumns() { _columnLayout.clear(); }
		double RowSpacing() const noexcept { return _rowSpacing; }
		void RowSpacing(double value) { _rowSpacing = value; }
		void AddItemToColumn(StaggeredItem* item, int columnIndex);
		void RemoveFromIndex(int index);
		double GetHeight();
		StaggeredItem* GetItemAt(int index);
		StaggeredColumnLayout& GetColumnLayout(int columnIndex) { return _columnLayout[columnIndex]; }
		void RecycleElementAt(int index);
		void RemoveRange(int startIndex, int endIndex);
		void Clear();
		void InitializeForContext(winrt::Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context);

	private:
		std::vector<StaggeredItem*> _items;
		winrt::Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext _context;
		std::unordered_map<int, StaggeredColumnLayout> _columnLayout;
		double _lastAverageHeight;
		double _columnWidth;
		double _rowSpacing;

	public:
		StaggeredLayoutState() noexcept
			: _columnWidth(0.0), _rowSpacing(0.0), _lastAverageHeight(0.0), _context(nullptr)
		{
		}

		~StaggeredLayoutState()
		{
			Clear();
		}
	};
}