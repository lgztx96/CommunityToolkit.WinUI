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

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::Size finalSize);

	private:
		winrt::Panel InitializeParentHeaderConnection();

		double GetTreePadding();

		void DataRow_Unloaded(winrt::Windows::Foundation::IInspectable const& sender, RoutedEventArgs const& e);

		winrt::Panel _parentPanel{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::DataTable _parentTable{ nullptr };

		bool _isTreeView{ false };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct DataRow : DataRowT<DataRow, implementation::DataRow>
	{
	};
}
