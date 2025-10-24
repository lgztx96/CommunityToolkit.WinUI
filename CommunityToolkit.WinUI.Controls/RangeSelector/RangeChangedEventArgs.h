#pragma once

#include "RangeChangedEventArgs.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct RangeChangedEventArgs : RangeChangedEventArgsT<RangeChangedEventArgs, implementation::RangeChangedEventArgs>
    {
    };
}
