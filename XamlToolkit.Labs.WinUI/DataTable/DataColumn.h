#pragma once

#include "DataColumn.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct DataColumn : DataColumnT<DataColumn>
    {
		static constexpr auto PartColumnSizer = L"PART_ColumnSizer";

        DataColumn();

        bool CanResize() const;
        void CanResize(bool value);

        static const wil::single_threaded_property<DependencyProperty> CanResizeProperty;

        GridLength DesiredWidth() const;
        void DesiredWidth(GridLength value);

        static const wil::single_threaded_property<DependencyProperty> DesiredWidthProperty;

        wil::single_threaded_rw_property<double> MaxChildDesiredWidth;

        GridLength CurrentWidth() const;

        void OnApplyTemplate();

        void ColumnSizer_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

        void ColumnSizer_ManipulationCompleted(winrt::Windows::Foundation::IInspectable const& sender, ManipulationCompletedRoutedEventArgs const& e);

        void ColumnResizedByUserSizer();

    private:
        static void DesiredWidth_PropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline GridLength StarLength = GridLength(1, GridUnitType::Star);

        GridLength _currentWidth;

        winrt::XamlToolkit::WinUI::Controls::ContentSizer _columnSizer{ nullptr };

        winrt::weak_ref<winrt::XamlToolkit::Labs::WinUI::DataTable> _parent;

		UIElement::ManipulationDelta_revoker _columnSizerManipulationDeltaRevoker;
		UIElement::ManipulationCompleted_revoker _columnSizerManipulationCompletedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct DataColumn : DataColumnT<DataColumn, implementation::DataColumn>
    {
    };
}
