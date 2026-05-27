#include "pch.h"
#include "winrt_module_imports.h"
#include "DataColumn.h"
#if __has_include("DataColumn.g.cpp")
#include "DataColumn.g.cpp"
#endif
#include "../../XamlToolkit.WinUI/common.h"
#include "DataTable.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    GridLength DataColumn::DesiredWidth() const { return winrt::unbox_value<GridLength>(GetValue(DesiredWidthProperty)); }
    void DataColumn::DesiredWidth(GridLength value) { SetValue(DesiredWidthProperty, winrt::box_value(value)); }

    const wil::single_threaded_property<DependencyProperty> DataColumn::DesiredWidthProperty =
        DependencyProperty::Register(L"DesiredWidth", 
            winrt::xaml_typename<GridLength>(), 
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(GridLengthHelper::Auto()), &DataColumn::DesiredWidth_PropertyChanged ));

    bool DataColumn::CanResize() const { return winrt::unbox_value<bool>(GetValue(CanResizeProperty)); }
    void DataColumn::CanResize(bool value) { SetValue(CanResizeProperty, winrt::box_value(value)); }

    const wil::single_threaded_property<DependencyProperty> DataColumn::CanResizeProperty =
        DependencyProperty::Register(L"CanResize",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(false)));

    DataColumn::DataColumn()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    GridLength DataColumn::CurrentWidth() const
    {
		return _currentWidth;
    }

    void DataColumn::OnApplyTemplate()
    {
        if (_columnSizer != nullptr)
        {
            _columnSizer.TargetControl(nullptr);
            _columnSizerManipulationDeltaRevoker.revoke();
			_columnSizerManipulationCompletedRevoker.revoke();
        }

        _columnSizer = GetTemplateChild(PartColumnSizer).try_as<winrt::XamlToolkit::WinUI::Controls::ContentSizer>();

        if (_columnSizer != nullptr)
        {
            _columnSizer.TargetControl(*this);
            _columnSizerManipulationDeltaRevoker = _columnSizer.ManipulationDelta(winrt::auto_revoke, { this, &DataColumn::ColumnSizer_ManipulationDelta });
            _columnSizerManipulationCompletedRevoker = _columnSizer.ManipulationCompleted(winrt::auto_revoke, { this, &DataColumn::ColumnSizer_ManipulationCompleted });
        }

        // Get DataTable parent weak reference for when we manipulate columns.
        auto parent = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(*this);
        if (parent != nullptr)
        {
            _parent = winrt::make_weak(parent);
        }

        base_type::OnApplyTemplate();
    }

    void DataColumn::ColumnSizer_ManipulationDelta([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] ManipulationDeltaRoutedEventArgs const& e)
    {
        ColumnResizedByUserSizer();
    }

    void DataColumn::ColumnSizer_ManipulationCompleted([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] ManipulationCompletedRoutedEventArgs const& e)
    {
        ColumnResizedByUserSizer();
    }

    void DataColumn::ColumnResizedByUserSizer()
    {
        // Update our internal representation to be our size now as a fixed value.
        _currentWidth = GridLength(ActualWidth());

        // Notify the rest of the table to update
        if (auto parent = _parent.get())
        {
			auto parentImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(parent);
            parentImpl->ColumnResized();
        }
    }

    void DataColumn::DesiredWidth_PropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        // If the developer updates the size of the column, update our internal copy
        if (auto col = d.try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>())
        {
			auto colImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(col);
            colImpl->_currentWidth = col.DesiredWidth();
        }
    }
}
