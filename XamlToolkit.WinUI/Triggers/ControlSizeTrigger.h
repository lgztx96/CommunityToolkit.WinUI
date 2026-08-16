#pragma once

#include "ControlSizeTrigger.g.h"

#ifdef __INTELLISENSE__
#include <limits>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct ControlSizeTrigger : ControlSizeTriggerT<ControlSizeTrigger>
    {
        ControlSizeTrigger() = default;

        bool CanTrigger() const { return winrt::unbox_value<bool>(GetValue(CanTriggerProperty)); }
        void CanTrigger(bool value) const { SetValue(CanTriggerProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> CanTriggerProperty;

        double MaxWidth() const { return winrt::unbox_value<double>(GetValue(MaxWidthProperty)); }
        void MaxWidth(double value) const { SetValue(MaxWidthProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> MaxWidthProperty;

        double MinWidth() const { return winrt::unbox_value<double>(GetValue(MinWidthProperty)); }
        void MinWidth(double value) const { SetValue(MinWidthProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> MinWidthProperty;

        double MaxHeight() const { return winrt::unbox_value<double>(GetValue(MaxHeightProperty)); }
        void MaxHeight(double value) const { SetValue(MaxHeightProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> MaxHeightProperty;

        double MinHeight() const { return winrt::unbox_value<double>(GetValue(MinHeightProperty)); }
        void MinHeight(double value) const { SetValue(MinHeightProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> MinHeightProperty;

        winrt::FrameworkElement TargetElement() const { return winrt::unbox_value<winrt::FrameworkElement>(GetValue(TargetElementProperty)); }
        void TargetElement(winrt::FrameworkElement const& value) const { SetValue(TargetElementProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> TargetElementProperty;

        wil::single_threaded_property<bool> IsActive;

        // Handle event to get current values
        void OnTargetElementSizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        void UpdateTargetElement(winrt::FrameworkElement const& oldValue, winrt::FrameworkElement const& newValue);

        // Logic to evaluate and apply trigger value
        void UpdateTrigger();

    private:
        static void OnTargetElementPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const&);

        winrt::FrameworkElement::SizeChanged_revoker _sizeChangedRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct ControlSizeTrigger : ControlSizeTriggerT<ControlSizeTrigger, implementation::ControlSizeTrigger>
    {
    };
}
