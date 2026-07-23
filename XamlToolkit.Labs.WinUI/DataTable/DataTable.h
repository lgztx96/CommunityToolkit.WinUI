#pragma once

#include "DataTable.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <set>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct DataTable : DataTableT<DataTable>
	{
		DataTable() = default;

		// TODO: We should cache this result and update if column properties change
		bool IsAnyColumnAuto();

		// TODO: Check with Sergio if there's a better structure here, as I don't need a Dictionary like ConditionalWeakTable
		std::set<winrt::XamlToolkit::Labs::WinUI::DataRow>& Rows();

		double ColumnWidth(uint32_t index) const;

		double BeginColumnResize(winrt::XamlToolkit::Labs::WinUI::DataColumn const& column);

		void ColumnWidthChanged();

		void ColumnResized();

		//// TODO: Would we want this named 'Spacing' instead if we support an Orientation in the future for columns being items instead of rows?
		double ColumnSpacing() const;
		void ColumnSpacing(double value);

        static const wil::single_threaded_property<winrt::DependencyProperty> ColumnSpacingProperty;

        winrt::Size MeasureOverride(winrt::Size availableSize);

        winrt::Size ArrangeOverride(winrt::Size finalSize);

	private:
		void UpdateColumnWidths(double availableWidth);

		std::set<winrt::XamlToolkit::Labs::WinUI::DataRow> _rows;
		std::vector<double> _columnWidths;
		double _layoutWidth{ 0 };
		bool _isFreezingColumnWidths{ false };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct DataTable : DataTableT<DataTable, implementation::DataTable>
	{
	};
}
