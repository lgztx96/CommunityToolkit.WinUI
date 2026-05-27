#pragma once

#include "DataRow.g.h"

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct DataRow : DataRowT<DataRow>
	{
		DataRow();

		Size MeasureOverride(Size availableSize);

		Size ArrangeOverride(Size finalSize);

	private:
		Panel InitializeParentHeaderConnection();

		void DataRow_Unloaded(winrt::Windows::Foundation::IInspectable const& sender, RoutedEventArgs const& e);

		Panel _parentPanel{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::DataTable _parentTable{ nullptr };

		bool _isTreeView{ false };
		double _treePadding{ 0.0 };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct DataRow : DataRowT<DataRow, implementation::DataRow>
	{
	};
}
