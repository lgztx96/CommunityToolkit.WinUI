// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "StaggeredLayout.g.h"
#include "StaggeredLayoutState.h"
#include <span>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct StaggeredLayout : StaggeredLayoutT<StaggeredLayout>
    {
    public:
        StaggeredLayout() = default;

        double DesiredColumnWidth();
        void DesiredColumnWidth(double value);

        void ColumnSpacing(double value);
        double ColumnSpacing();

        void RowSpacing(double value);
        double RowSpacing();

        StaggeredLayoutItemsStretch ItemsStretch();
        void ItemsStretch(StaggeredLayoutItemsStretch value);

#pragma region IVirtualizingLayoutOverrides
        void InitializeForContextCore(Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context);
        void UninitializeForContextCore(Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context);
        Windows::Foundation::Size MeasureOverride(
            Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
            Windows::Foundation::Size const& availableSize);
        Windows::Foundation::Size ArrangeOverride(
            Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
            Windows::Foundation::Size const& finalSize);
        void OnItemsChangedCore(
            Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
            Windows::Foundation::IInspectable const& source,
            Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventArgs const& args);
#pragma endregion

        static void OnDependencyPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args);

        static inline const wil::single_threaded_property<DependencyProperty> DesiredColumnWidthProperty =
            DependencyProperty::Register(L"DesiredColumnWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(250.0), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<DependencyProperty> ItemsStretchProperty =
            DependencyProperty::Register(L"ItemsStretch",
                winrt::xaml_typename<StaggeredLayoutItemsStretch>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(StaggeredLayoutItemsStretch::None), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<DependencyProperty> ColumnSpacingProperty =
            DependencyProperty::Register(L"ColumnSpacing",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(0.0), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<DependencyProperty> RowSpacingProperty =
            DependencyProperty::Register(L"RowSpacing",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(0.0), &StaggeredLayout::OnDependencyPropertyChanged));

    private:
        static int GetColumnIndex(std::span<const double> columnHeights);
        
        winrt::com_ptr<StaggeredLayoutState> GetAsStackState(const winrt::Windows::Foundation::IInspectable& state)
        {
            return winrt::get_self<StaggeredLayoutState>(state)->get_strong();
        }
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct StaggeredLayout : StaggeredLayoutT<StaggeredLayout, implementation::StaggeredLayout>
    {

    };
}