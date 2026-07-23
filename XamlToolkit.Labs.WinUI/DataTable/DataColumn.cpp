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
    }

    winrt::GridLength DataColumn::CurrentWidth() const
    {
        return DesiredWidth();
    }

    double DataColumn::ActualColumnWidth() const
    {
        return _actualColumnWidth;
    }

    void DataColumn::SetCurrentWidth(double value)
    {
        // A user resize resolves Auto/Star columns to pixels. Write that result
        // back to the dependency property so XAML, bindings, persistence, and
        // the layout engine all observe the same authoritative value.
        _isInternalResizeUpdate = true;
        try
        {
            DesiredWidth(GridLengthHelper::FromPixels(value));
        }
        catch (...)
        {
            _isInternalResizeUpdate = false;
            throw;
        }
        _isInternalResizeUpdate = false;
    }

    void DataColumn::SetActualColumnWidth(double value)
    {
        _actualColumnWidth = value;
    }

    void DataColumn::OnApplyTemplate()
    {
        if (_columnSizer)
        {
            _columnSizer.TargetControl(nullptr);
            _columnSizerManipulationStartedRevoker.revoke();
            _columnSizerManipulationDeltaRevoker.revoke();
            _columnSizerManipulationCompletedRevoker.revoke();
        }

        _columnSizer = GetTemplateChild(PartColumnSizer).try_as<winrt::XamlToolkit::WinUI::Controls::ContentSizer>();

        if (_columnSizer)
        {
            // Keep ContentSizer's native manipulation recognizer, but explicitly
            // disable its automatic FrameworkElement.Width assignment.
            _columnSizer.TargetControl(nullptr);
            _columnSizerManipulationStartedRevoker = _columnSizer.ManipulationStarted(winrt::auto_revoke, { this, &DataColumn::ColumnSizer_ManipulationStarted });
            _columnSizerManipulationDeltaRevoker = _columnSizer.ManipulationDelta(winrt::auto_revoke, { this, &DataColumn::ColumnSizer_ManipulationDelta });
            _columnSizerManipulationCompletedRevoker = _columnSizer.ManipulationCompleted(winrt::auto_revoke, { this, &DataColumn::ColumnSizer_ManipulationCompleted });
        }

        // Get DataTable parent weak reference for when we manipulate columns.
        if (auto parent = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(*this))
        {
            _parent = winrt::make_weak(parent);
        }

        base_type::OnApplyTemplate();
    }

    void DataColumn::ColumnSizer_ManipulationStarted([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] ManipulationStartedRoutedEventArgs const& e)
    {
        auto parent = _parent.get();
        if (parent == nullptr)
        {
            parent = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(*this);
            if (parent != nullptr)
            {
                _parent = winrt::make_weak(parent);
            }
        }

        if (parent == nullptr)
        {
            return;
        }

        auto parentImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(parent);
        _resizeStartWidth = parentImpl->BeginColumnResize(*this);
        _isManipulationResizing = true;
    }

    void DataColumn::ColumnSizer_ManipulationDelta([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        auto parent = _parent.get();
        if (!_isManipulationResizing || parent == nullptr || _columnSizer == nullptr)
        {
            return;
        }

        const auto dragIncrement = _columnSizer.DragIncrement();
        auto horizontalChange = std::trunc(e.Cumulative().Translation.X / dragIncrement) * dragIncrement;
        if (FlowDirection() == winrt::FlowDirection::RightToLeft)
        {
            horizontalChange *= -1;
        }

        auto width = _resizeStartWidth + horizontalChange;
        width = std::max<double>(width, MinWidth());
        if (std::isfinite(MaxWidth()))
        {
            width = std::min<double>(width, MaxWidth());
        }

        const auto currentWidth = DesiredWidth();
        if (currentWidth.GridUnitType != GridUnitType::Pixel || currentWidth.Value != width)
        {
            SetCurrentWidth(width);
        }
    }

    void DataColumn::ColumnSizer_ManipulationCompleted([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationCompletedRoutedEventArgs const& e)
    {
        _isManipulationResizing = false;

        if (auto parent = _parent.get())
        {
            // Drag deltas only require arrange. Re-measure once at the end so
            // header and row content finalize against the persisted pixel width.
            winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(parent)->ColumnWidthChanged();
        }
    }

    void DataColumn::ApplyDesiredWidth([[maybe_unused]] GridLength const& value)
    {
        if (!_isInternalResizeUpdate)
        {
            InvalidateMeasure();
        }

        auto parent = _parent.get();
        if (parent == nullptr)
        {
            parent = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(*this);
            if (parent == nullptr)
            {
                return;
            }

            _parent = winrt::make_weak(parent);
        }

        auto parentImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(parent);
        if (_isInternalResizeUpdate)
        {
            parentImpl->ColumnResized();
        }
        else
        {
            parentImpl->ColumnWidthChanged();
        }
    }

    void DataColumn::DesiredWidth_PropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // The dependency property is the single source of truth. Its callback
        // only schedules the required layout work.
        if (auto col = d.try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>())
        {
            winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(col)->ApplyDesiredWidth(col.DesiredWidth());
        }
    }
}
