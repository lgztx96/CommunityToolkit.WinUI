#pragma once

#include "CollectionVisibilityConverter.g.h"
#include "EmptyCollectionToObjectConverter.h"

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
    /// This class converts a collection size to visibility.
    /// </summary>
    struct CollectionVisibilityConverter : CollectionVisibilityConverterT<CollectionVisibilityConverter, EmptyCollectionToObjectConverter>
    {
        CollectionVisibilityConverter()
        {
            NotEmptyValue(winrt::box_value(winrt::Visibility::Visible));
            EmptyValue(winrt::box_value(winrt::Visibility::Collapsed));
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct CollectionVisibilityConverter : CollectionVisibilityConverterT<CollectionVisibilityConverter, implementation::CollectionVisibilityConverter>
	{
    };
}