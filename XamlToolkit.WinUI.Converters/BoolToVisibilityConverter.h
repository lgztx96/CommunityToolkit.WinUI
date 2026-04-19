#pragma once

#include "BoolToVisibilityConverter.g.h"
#include "BoolToObjectConverter.h"

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    struct BoolToVisibilityConverter : BoolToVisibilityConverterT<BoolToVisibilityConverter, BoolToObjectConverter>
    {
        BoolToVisibilityConverter()
        {
            TrueValue(winrt::box_value(Visibility::Visible));
            FalseValue(winrt::box_value(Visibility::Collapsed));
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
    struct BoolToVisibilityConverter : BoolToVisibilityConverterT<BoolToVisibilityConverter, implementation::BoolToVisibilityConverter>
    {
    };
}
