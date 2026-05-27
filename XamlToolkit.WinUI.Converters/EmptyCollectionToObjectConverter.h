#pragma once

#include "EmptyCollectionToObjectConverter.g.h"
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
    /// This class converts a collection size into an other object.
    /// Can be used to convert to bind a visibility, a color or an image to the size of the collection.
    /// </summary>
    struct EmptyCollectionToObjectConverter : EmptyCollectionToObjectConverterT<EmptyCollectionToObjectConverter, EmptyObjectToObjectConverter>
    {
        EmptyCollectionToObjectConverter() = default;

    protected:
        /// <summary>
        /// Checks collection for emptiness.
        /// </summary>
        /// <param name="value">Value to be checked.</param>
        /// <returns>True if value is an empty collection or does not implement IEnumerable, false otherwise.</returns>
        bool CheckValueIsEmpty(winrt::IInspectable const& value) const override;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct EmptyCollectionToObjectConverter : EmptyCollectionToObjectConverterT<EmptyCollectionToObjectConverter, implementation::EmptyCollectionToObjectConverter>
	{

    };
}