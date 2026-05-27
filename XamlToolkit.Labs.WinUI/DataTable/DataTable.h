#pragma once

#include "DataTable.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <set>
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

        void ColumnResized();

        //// TODO: Would we want this named 'Spacing' instead if we support an Orientation in the future for columns being items instead of rows?
        bool ColumnSpacing() const;
        void ColumnSpacing(double value);

        static const wil::single_threaded_property<DependencyProperty> ColumnSpacingProperty;

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);

    private:
		std::set<winrt::XamlToolkit::Labs::WinUI::DataRow> _rows;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct DataTable : DataTableT<DataTable, implementation::DataTable>
    {
    };
}
