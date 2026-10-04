#include "pch.h"
#include "winrt_module_imports.h"
#include "DataColumn.h"
#if __has_include("DataColumn.g.cpp")
#include "DataColumn.g.cpp"
#endif
#include "../Diagnostics/ToolkitProfilerTracing.h"
#include "../../XamlToolkit.WinUI/common.h"
#include "DataTable.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    winrt::GridLength DataColumn::DesiredWidth() const 
    { 
        return winrt::unbox_value<winrt::GridLength>(GetValue(DesiredWidthProperty()));
    }

    void DataColumn::DesiredWidth(winrt::GridLength value) 
    { 
        SetValue(DesiredWidthProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<winrt::DependencyProperty> DataColumn::DesiredWidthProperty =
        winrt::DependencyProperty::Register(
            L"DesiredWidth",
            winrt::xaml_typename<winrt::GridLength>(), 
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::GridLengthHelper::Auto()), &DataColumn::DesiredWidth_PropertyChanged ));

    bool DataColumn::CanResize() const 
    { 
        return winrt::unbox_value<bool>(GetValue(CanResizeProperty())); 
    }

    void DataColumn::CanResize(bool value) 
    { 
        SetValue(CanResizeProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<winrt::DependencyProperty> DataColumn::CanResizeProperty =
        winrt::DependencyProperty::Register(
            L"CanResize",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(false)));

    DataColumn::DataColumn()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        Loaded({ this, &DataColumn::DataColumn_Loaded });
        Unloaded({ this, &DataColumn::DataColumn_Unloaded });
        XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
    }

    DataColumn::~DataColumn()
    {
    	XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
    }

    winrt::GridLength DataColumn::CurrentWidth() const
    {
		return _currentWidth;
    }

    void DataColumn::OnApplyTemplate()
    {
        DetachColumnSizer();

        _columnSizer = GetTemplateChild(PartColumnSizer).try_as<winrt::XamlToolkit::WinUI::Controls::ContentSizer>();

        AttachColumnSizer();

        // Get DataTable parent weak reference for when we manipulate columns.
        if (auto parent = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(*this))
        {
            _parent = winrt::make_weak(parent);
        }

        base_type::OnApplyTemplate();
    }

    void DataColumn::DataColumn_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        AttachColumnSizer();
    }

    void DataColumn::DataColumn_Unloaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        DetachColumnSizer();
    }

    // The sizer is a template part of this column and its TargetControl is a DependencyProperty, i.e. a
    // strong reference back to the column. Left alone, the two hold each other and neither is ever
    // released — the column keeps the whole page alive with it. Detaching on unload breaks that; the
    // Loaded handler attaches it again so dragging still resizes the column after a reload.
    void DataColumn::AttachColumnSizer()
    {
        if (!_columnSizer)
        {
            return;
        }

        _columnSizer.ManipulationDelta(_columnSizerManipulationDeltaToken);
        _columnSizer.ManipulationCompleted(_columnSizerManipulationCompletedToken);

        _columnSizer.TargetControl(*this);
        _columnSizerManipulationDeltaToken = _columnSizer.ManipulationDelta({ this, &DataColumn::ColumnSizer_ManipulationDelta });
        _columnSizerManipulationCompletedToken = _columnSizer.ManipulationCompleted({ this, &DataColumn::ColumnSizer_ManipulationCompleted });
    }

    void DataColumn::DetachColumnSizer()
    {
        if (!_columnSizer)
        {
            return;
        }

        _columnSizer.TargetControl(nullptr);
        _columnSizer.ManipulationDelta(_columnSizerManipulationDeltaToken);
        _columnSizer.ManipulationCompleted(_columnSizerManipulationCompletedToken);

        _columnSizerManipulationDeltaToken = {};
        _columnSizerManipulationCompletedToken = {};
    }

    void DataColumn::ColumnSizer_ManipulationDelta([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        ColumnResizedByUserSizer();
    }

    void DataColumn::ColumnSizer_ManipulationCompleted([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationCompletedRoutedEventArgs const& e)
    {
        ColumnResizedByUserSizer();
    }

    void DataColumn::ColumnResizedByUserSizer()
    {
        // Update our internal representation to be our size now as a fixed value.
        _currentWidth = winrt::GridLengthHelper::FromPixels(ActualWidth());

        // Notify the rest of the table to update
        if (auto parent = _parent.get())
        {
			auto parentImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(parent);
            parentImpl->ColumnResized();
        }
    }

    void DataColumn::DesiredWidth_PropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // If the developer updates the size of the column, update our internal copy
        if (auto col = d.try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>())
        {
			auto colImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(col);
            colImpl->_currentWidth = col.DesiredWidth();
        }
    }
}
