#pragma once

#include "EqualPanel.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <ranges>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct EqualPanel : EqualPanelT<EqualPanel>
    {
        double _maxItemWidth = 0;
        double _maxItemHeight = 0;
        int _visibleItemsCount = 0;

        double Spacing() { return winrt::unbox_value<double>(GetValue(SpacingProperty)); }
        void Spacing(double value) { return SetValue(SpacingProperty, winrt::box_value(value)); }

        static void OnSpacingChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        /// <summary>
        /// Identifies the Spacing dependency property.
        /// </summary>
        /// <returns>The identifier for the <see cref="Spacing"/> dependency property.</returns>
        static inline const wil::single_threaded_property<DependencyProperty> SpacingProperty = DependencyProperty::Register(
            L"Spacing",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(0.0), &EqualPanel::OnSpacingChanged));

        EqualPanel()
        {
            RegisterPropertyChangedCallback(FrameworkElement::HorizontalAlignmentProperty(), 
                { this, &EqualPanel::OnHorizontalAlignmentChanged });
        }

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);

        void OnHorizontalAlignmentChanged(DependencyObject const& sender, DependencyProperty const& dp);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct EqualPanel : EqualPanelT<EqualPanel, implementation::EqualPanel>
    {
    };
}
