#pragma once

#include "CollectionVisibilityConverter.g.h"
#include "EmptyCollectionToObjectConverter.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::Xaml::Interop;
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
            NotEmptyValue(winrt::box_value(Visibility::Visible));
            EmptyValue(winrt::box_value(Visibility::Collapsed));
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct CollectionVisibilityConverter : CollectionVisibilityConverterT<CollectionVisibilityConverter, implementation::CollectionVisibilityConverter>
	{
    };
}