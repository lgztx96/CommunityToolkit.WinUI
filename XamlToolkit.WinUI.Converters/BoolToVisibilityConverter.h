#pragma once

#include "BoolToVisibilityConverter.g.h"
#include "BoolToObjectConverter.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    struct BoolToVisibilityConverter : BoolToVisibilityConverterT<BoolToVisibilityConverter, BoolToObjectConverter>
    {
        BoolToVisibilityConverter()
        {
            TrueValue(winrt::box_value(winrt::Visibility::Visible));
            FalseValue(winrt::box_value(winrt::Visibility::Collapsed));
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
    struct BoolToVisibilityConverter : BoolToVisibilityConverterT<BoolToVisibilityConverter, implementation::BoolToVisibilityConverter>
    {
    };
}
