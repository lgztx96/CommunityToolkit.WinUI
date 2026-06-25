#pragma once

#include "RangeChangedEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct RangeChangedEventArgs : RangeChangedEventArgsT<RangeChangedEventArgs>
    {
        RangeChangedEventArgs() = default;

        wil::single_threaded_property<double> OldValue;

        wil::single_threaded_property<double> NewValue;

        wil::single_threaded_property<RangeSelectorProperty> ChangedRangeProperty;

        RangeChangedEventArgs(double oldValue, double newValue, RangeSelectorProperty changedRangeProperty) 
            : OldValue(oldValue), NewValue(newValue), ChangedRangeProperty(changedRangeProperty)
        {

        }
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct RangeChangedEventArgs : RangeChangedEventArgsT<RangeChangedEventArgs, implementation::RangeChangedEventArgs>
    {
    };
}
