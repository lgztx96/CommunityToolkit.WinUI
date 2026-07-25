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

        static const wil::single_threaded_property<winrt::DependencyProperty> CanResizeProperty;

        winrt::GridLength DesiredWidth() const;
        void DesiredWidth(winrt::GridLength value);

        static const wil::single_threaded_property<winrt::DependencyProperty> DesiredWidthProperty;

        winrt::Microsoft::UI::Xaml::Style ColumnSizerStyle() const;
        void ColumnSizerStyle(winrt::Microsoft::UI::Xaml::Style const& value);

        static const wil::single_threaded_property<DependencyProperty> ColumnSizerStyleProperty;

        wil::single_threaded_rw_property<double> MaxChildDesiredWidth;

        winrt::GridLength CurrentWidth() const;

        double ActualColumnWidth() const;

        void SetCurrentWidth(double value);

        void SetActualColumnWidth(double value);

        void OnApplyTemplate();

        void ColumnSizer_ManipulationStarted(winrt::Windows::Foundation::IInspectable const& sender, winrt::ManipulationStartedRoutedEventArgs const& e);

        void ColumnSizer_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e);

        void ColumnSizer_ManipulationCompleted(winrt::IInspectable const& sender, winrt::ManipulationCompletedRoutedEventArgs const& e);

    private:
        static void DesiredWidth_PropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        void ApplyDesiredWidth(GridLength const& value);

        double _actualColumnWidth{ 0 };
        bool _isInternalResizeUpdate{ false };

        winrt::XamlToolkit::WinUI::Controls::ContentSizer _columnSizer{ nullptr };

        winrt::weak_ref<winrt::XamlToolkit::Labs::WinUI::DataTable> _parent;

        winrt::UIElement::ManipulationStarted_revoker _columnSizerManipulationStartedRevoker;
        winrt::UIElement::ManipulationDelta_revoker _columnSizerManipulationDeltaRevoker;
        winrt::UIElement::ManipulationCompleted_revoker _columnSizerManipulationCompletedRevoker;

        bool _isManipulationResizing{ false };
        double _resizeStartWidth{ 0 };
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct DataColumn : DataColumnT<DataColumn, implementation::DataColumn>
    {
    };
}
