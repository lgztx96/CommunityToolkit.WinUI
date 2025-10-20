#pragma once

#include "ControlSizeTrigger.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;

    struct ControlSizeTrigger : ControlSizeTriggerT<ControlSizeTrigger>
    {
        ControlSizeTrigger() = default;

        winrt::event_token size_changed_token;

        bool CanTrigger() const { return winrt::unbox_value<bool>(GetValue(CanTriggerProperty)); }
        void CanTrigger(bool value) const { SetValue(CanTriggerProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> CanTriggerProperty = DependencyProperty::Register(
            L"CanTrigger",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(true), [](auto&& d, auto&&) 
                { 
                    auto self = winrt::get_self<ControlSizeTrigger>(d.template as<class_type>())->get_strong();
                    self->UpdateTrigger();
                }));

        double MaxWidth() const { return winrt::unbox_value<double>(GetValue(MaxWidthProperty)); }
        void MaxWidth(double value) const { SetValue(MaxWidthProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> MaxWidthProperty = DependencyProperty::Register(
            L"MaxWidth",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(std::numeric_limits<double>::infinity()), [](auto&& d, auto&&) 
                {
                    auto self = winrt::get_self<ControlSizeTrigger>(d.template as<class_type>())->get_strong();
                    self->UpdateTrigger();
                }));

        double MinWidth() const { return winrt::unbox_value<double>(GetValue(MinWidthProperty)); }
        void MinWidth(double value) const { SetValue(MinWidthProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> MinWidthProperty = DependencyProperty::Register(
            L"MinWidth",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(0.0), [](auto&& d, auto&&)
                { 
                    auto self = winrt::get_self<ControlSizeTrigger>(d.template as<class_type>())->get_strong();
                    self->UpdateTrigger();
                }));

        double MaxHeight() const { return winrt::unbox_value<double>(GetValue(MaxHeightProperty)); }
        void MaxHeight(double value) const { SetValue(MaxHeightProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> MaxHeightProperty = DependencyProperty::Register(
            L"MaxHeight",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(std::numeric_limits<double>::infinity()), [](auto&& d, auto&&)
                { 
                    auto self = winrt::get_self<ControlSizeTrigger>(d.template as<class_type>())->get_strong();
                    self->UpdateTrigger();
                }));

        double MinHeight() const { return winrt::unbox_value<double>(GetValue(MinHeightProperty)); }
        void MinHeight(double value) const { SetValue(MinHeightProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> MinHeightProperty = DependencyProperty::Register(
            L"MinHeight",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename <class_type>(),
            PropertyMetadata(winrt::box_value(0.0), [](auto&& d, auto&&)
                { 
                    auto self = winrt::get_self<ControlSizeTrigger>(d.template as<class_type>())->get_strong();
                    self->UpdateTrigger();
                }));

        FrameworkElement TargetElement() const { return winrt::unbox_value<FrameworkElement>(GetValue(TargetElementProperty)); }
        void TargetElement(FrameworkElement const& value) const { SetValue(TargetElementProperty, winrt::box_value(value)); }

        static void OnTargetElementPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
        {
            winrt::get_self<ControlSizeTrigger>(d.as<class_type>())->
                UpdateTargetElement(e.OldValue().as<FrameworkElement>(), e.NewValue().as<FrameworkElement>());
        }

        static inline const wil::single_threaded_property<DependencyProperty> TargetElementProperty = DependencyProperty::Register(
                L"TargetElement",
                winrt::xaml_typename<FrameworkElement>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr, &ControlSizeTrigger::OnTargetElementPropertyChanged));

        wil::single_threaded_property<bool> IsActive;

        // Handle event to get current values
        void OnTargetElementSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

        void UpdateTargetElement(FrameworkElement const& oldValue, FrameworkElement const& newValue);

        // Logic to evaluate and apply trigger value
        void UpdateTrigger();
    };
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
    struct ControlSizeTrigger : ControlSizeTriggerT<ControlSizeTrigger, implementation::ControlSizeTrigger>
    {
    };
}
