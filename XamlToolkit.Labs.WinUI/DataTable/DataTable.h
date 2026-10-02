#pragma once

#include "DataTable.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <unordered_map>
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
		DataTable();

		// TODO: We should cache this result and update if column properties change
		bool IsAnyColumnAuto();

		void RegisterRow(winrt::XamlToolkit::Labs::WinUI::DataRow const& row);
		void UnregisterRow(winrt::XamlToolkit::Labs::WinUI::DataRow const& row);
		std::vector<winrt::XamlToolkit::Labs::WinUI::DataRow> Rows();

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
		void DataTable_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::RoutedEventArgs const& e);
		void UpdateColumnWidths(double availableWidth);

		// Measurement can register a row that never reaches Loaded/Unloaded.
		// The header must observe rows without owning their visual lifetimes.
		std::unordered_map<void*, winrt::weak_ref<winrt::XamlToolkit::Labs::WinUI::DataRow>> _rows;
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
