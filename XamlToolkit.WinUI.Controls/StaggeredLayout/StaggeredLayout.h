// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "StaggeredLayout.g.h"
#include "StaggeredLayoutState.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <memory>
#include <span>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct StaggeredLayout : StaggeredLayoutT<StaggeredLayout>
    {
    public:
        StaggeredLayout() = default;

        double DesiredColumnWidth() const;
        void DesiredColumnWidth(double value);

        void ColumnSpacing(double value);
        double ColumnSpacing() const;

        void RowSpacing(double value);
        double RowSpacing() const;

        StaggeredLayoutItemsStretch ItemsStretch() const;
        void ItemsStretch(StaggeredLayoutItemsStretch value);

#pragma region IVirtualizingLayoutOverrides
        void InitializeForContextCore(winrt::VirtualizingLayoutContext const& context);
        void UninitializeForContextCore(winrt::VirtualizingLayoutContext const& context);
        winrt::Size MeasureOverride(
            winrt::VirtualizingLayoutContext const& context,
            winrt::Size const& availableSize);
        winrt::Size ArrangeOverride(
            winrt::VirtualizingLayoutContext const& context,
            winrt::Size const& finalSize);
        void OnItemsChangedCore(
            winrt::VirtualizingLayoutContext const& context,
            winrt::IInspectable const& source,
            winrt::NotifyCollectionChangedEventArgs const& args);
#pragma endregion

        static void OnDependencyPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        static inline const wil::single_threaded_property<winrt::DependencyProperty> DesiredColumnWidthProperty =
            winrt::DependencyProperty::Register(L"DesiredColumnWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(250.0), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ItemsStretchProperty =
            winrt::DependencyProperty::Register(L"ItemsStretch",
                winrt::xaml_typename<StaggeredLayoutItemsStretch>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(StaggeredLayoutItemsStretch::None), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ColumnSpacingProperty =
            winrt::DependencyProperty::Register(L"ColumnSpacing",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(0.0), &StaggeredLayout::OnDependencyPropertyChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> RowSpacingProperty =
            winrt::DependencyProperty::Register(L"RowSpacing",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(0.0), &StaggeredLayout::OnDependencyPropertyChanged));

    private:
        static int GetColumnIndex(std::span<const double> columnHeights);
        
        winrt::com_ptr<StaggeredLayoutState> GetAsStackState(winrt::IInspectable const& state)
        {
            return winrt::get_self<StaggeredLayoutState>(state)->get_strong();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct StaggeredLayout : StaggeredLayoutT<StaggeredLayout, implementation::StaggeredLayout>
    {

    };
}
