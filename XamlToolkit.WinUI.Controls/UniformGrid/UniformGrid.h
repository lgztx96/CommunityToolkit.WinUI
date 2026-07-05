#pragma once

#include "UniformGrid.g.h"
#include "TakenSpotsReferenceHolder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <generator>
#include <memory>
#include <span>
#include <tuple>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct UniformGrid : UniformGridT<UniformGrid>
    {
		UniformGrid() = default;

        winrt::Size MeasureOverride(winrt::Size availableSize);

        winrt::Size ArrangeOverride(winrt::Size finalSize);

        static void OnPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::IInspectable const& newValue);

        static const wil::single_threaded_property<winrt::DependencyProperty> AutoLayoutProperty;

        static void SetAutoLayout(winrt::FrameworkElement const& element, winrt::IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty(), value);
        }

        static winrt::IReference<bool> GetAutoLayout(winrt::FrameworkElement const& element)
        {
            return element.GetValue(AutoLayoutProperty()).try_as<winrt::IReference<bool>>();
        }

        static void SetAutoLayout(winrt::ColumnDefinition const& element, winrt::IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty(), value);
        }

        static winrt::IReference<bool> GetAutoLayout(winrt::ColumnDefinition const& element)
        {
            return element.GetValue(AutoLayoutProperty()).try_as<winrt::IReference<bool>>();
        }

        static void SetAutoLayout(winrt::RowDefinition const& element, winrt::IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty(), value);
        }

        static winrt::IReference<bool> GetAutoLayout(winrt::RowDefinition const& element)
        {
            return element.GetValue(AutoLayoutProperty()).try_as<winrt::IReference<bool>>();
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ColumnsProperty;

        int32_t Columns() const
        { 
            return winrt::unbox_value<int32_t>(GetValue(ColumnsProperty())); 
        }

        void Columns(int32_t value) const
        { 
            SetValue(ColumnsProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> FirstColumnProperty;

        int32_t FirstColumn() const
        { 
            return winrt::unbox_value<int32_t>(GetValue(FirstColumnProperty()));
        }

        void FirstColumn(int32_t value) const
        { 
            SetValue(FirstColumnProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

        winrt::Orientation Orientation() const
        {
            return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty()));
        }

        void Orientation(winrt::Orientation value) const
        {
            SetValue(OrientationProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> RowsProperty;

        int32_t Rows() const
        { 
            return winrt::unbox_value<int32_t>(GetValue(RowsProperty()));
        }

        void Rows(int32_t value) const
        { 
            SetValue(RowsProperty(), winrt::box_value(value));
        }

    private:
        // Provides the next spot in the boolean array with a 'false' value.
        static std::generator<std::tuple<int, int>> GetFreeSpot(TakenSpotsReferenceHolder* arrayref, int firstcolumn, bool topdown);

        // Based on the number of visible children,
        // returns the dimensions of the
        // grid we need to hold all elements.
        static std::tuple<int, int> GetDimensions(std::span<winrt::FrameworkElement> visible, int rows, int cols, int firstColumn);

        // Used to interleave specified row dimensions with automatic rows added to use
        // underlying Grid layout for main arrange of UniformGrid.
        void SetupRowDefinitions(uint32_t rows);

        // Used to interleave specified column dimensions with automatic columns added to use
        // underlying Grid layout for main arrange of UniformGrid.
        void SetupColumnDefinitions(uint32_t columns);

        std::vector<winrt::UIElement> _overflow;
        std::unique_ptr<TakenSpotsReferenceHolder> _spotref;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct UniformGrid : UniformGridT<UniformGrid, implementation::UniformGrid>
    {
    };
}
