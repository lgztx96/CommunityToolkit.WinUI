#pragma once

#include "UniformGrid.g.h"
#include "TakenSpotsReferenceHolder.h"
#include <generator>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;
	using namespace winrt::Windows::Foundation::Collections;

    struct UniformGrid : UniformGridT<UniformGrid>
    {
    private:
        std::vector<UIElement> _overflow;
        std::unique_ptr<TakenSpotsReferenceHolder> _spotref;

        // Provides the next spot in the boolean array with a 'false' value.
        static std::generator<std::tuple<int, int>> GetFreeSpot(TakenSpotsReferenceHolder* arrayref, int firstcolumn, bool topdown);

        // Based on the number of visible children,
        // returns the dimensions of the
        // grid we need to hold all elements.
        static std::tuple<int, int> GetDimensions(std::span<FrameworkElement> visible, int rows, int cols, int firstColumn);

        // Used to interleave specified row dimensions with automatic rows added to use
        // underlying Grid layout for main arrange of UniformGrid.
        void SetupRowDefinitions(uint32_t rows);

        // Used to interleave specified column dimensions with automatic columns added to use
        // underlying Grid layout for main arrange of UniformGrid.
        void SetupColumnDefinitions(uint32_t columns);

    public:
		UniformGrid() = default;

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);

        static void OnPropertyChanged(DependencyObject const& d, [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& newValue)
        {
            if (auto self = d.try_as<class_type>())
            {
                self.InvalidateMeasure();
            }
        }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AutoLayoutProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
                L"AutoLayout",
                winrt::xaml_typename<IReference<bool>>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

        static void SetAutoLayout(winrt::Microsoft::UI::Xaml::FrameworkElement const& element, IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty, value);
        }

        static IReference<bool> GetAutoLayout(winrt::Microsoft::UI::Xaml::FrameworkElement const& element)
        {
            return element.GetValue(AutoLayoutProperty).try_as<IReference<bool>>();
        }

        static void SetAutoLayout(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition const& element, IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty, value);
        }

        static IReference<bool> GetAutoLayout(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition const& element)
        {
            return element.GetValue(AutoLayoutProperty).try_as<IReference<bool>>();
        }

        static void SetAutoLayout(winrt::Microsoft::UI::Xaml::Controls::RowDefinition const& element, IReference<bool> const& value)
        {
            element.SetValue(AutoLayoutProperty, value);
        }

        static IReference<bool> GetAutoLayout(winrt::Microsoft::UI::Xaml::Controls::RowDefinition const& element)
        {
            return element.GetValue(AutoLayoutProperty).try_as<IReference<bool>>();
        }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ColumnsProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Columns",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

        int32_t Columns() const { return winrt::unbox_value<int32_t>(GetValue(ColumnsProperty)); }
        void Columns(int32_t value) const { SetValue(ColumnsProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> FirstColumnProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"FirstColumn",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

        int32_t FirstColumn() const { return winrt::unbox_value<int32_t>(GetValue(FirstColumnProperty)); }
        void FirstColumn(int32_t value) const { SetValue(FirstColumnProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OrientationProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Orientation",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal), &UniformGrid::OnPropertyChanged });

        winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const
        {
            return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty));
        }
        void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation value) const
        {
            SetValue(OrientationProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RowsProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Rows",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

        int32_t Rows() const { return winrt::unbox_value<int32_t>(GetValue(RowsProperty)); }
        void Rows(int32_t value) const { SetValue(RowsProperty, winrt::box_value(value)); }
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct UniformGrid : UniformGridT<UniformGrid, implementation::UniformGrid>
    {
    };
}
