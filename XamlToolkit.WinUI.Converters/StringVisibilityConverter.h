#pragma once

#include "StringVisibilityConverter.g.h"
#include "EmptyStringToObjectConverter.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// This class converts a string value into a Visibility value (if the value is null or empty returns a collapsed value).
    /// </summary>
    struct StringVisibilityConverter : StringVisibilityConverterT<StringVisibilityConverter, EmptyStringToObjectConverter>
    {
        StringVisibilityConverter()
        {
            NotEmptyValue(winrt::box_value(winrt::Visibility::Visible));
            EmptyValue(winrt::box_value(winrt::Visibility::Collapsed));
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct StringVisibilityConverter : StringVisibilityConverterT<StringVisibilityConverter, implementation::StringVisibilityConverter>
	{

    };
}