#pragma once

#include "EmptyStringToObjectConverter.g.h"
#include "EmptyObjectToObjectConverter.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// This class converts a string value into a an object (if the value is null or empty returns the false value).
    /// Can be used to bind a visibility, a color or an image to the value of a string.
    /// </summary>
    struct EmptyStringToObjectConverter : EmptyStringToObjectConverterT<EmptyStringToObjectConverter, EmptyObjectToObjectConverter>
    {
        EmptyStringToObjectConverter() = default;

    protected:
        /// <summary>
        /// Checks string for emptiness.
        /// </summary>
        /// <param name="value">Value to be checked.</param>
        /// <returns>True if value is null or empty string, false otherwise.</returns>
        bool CheckValueIsEmpty(winrt::IInspectable const& value) const override;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct EmptyStringToObjectConverter : EmptyStringToObjectConverterT<EmptyStringToObjectConverter, implementation::EmptyStringToObjectConverter>
	{

    };
}