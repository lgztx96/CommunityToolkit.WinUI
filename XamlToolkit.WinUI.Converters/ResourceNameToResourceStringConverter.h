#pragma once

#include "ResourceNameToResourceStringConverter.g.h"
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
	using namespace Microsoft::Windows::ApplicationModel::Resources;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// Value converter that look up for the source string in the App Resources strings and returns its value, if found.
    /// </summary>
    struct ResourceNameToResourceStringConverter : ResourceNameToResourceStringConverterT<ResourceNameToResourceStringConverter>
    {
        ResourceNameToResourceStringConverter() = default;

        /// <summary>
        /// Take the source string as a resource name that will be looked up in the App Resources.
        /// If the resource exists, the value is returned, otherwise.
        /// </summary>
        /// <param name="value">The source string being passed to the target.</param>
        /// <param name="targetType">The type of the target property, as a type reference.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion.</param>
        /// <returns>The string corresponding to the resource name.</returns>
        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

        /// <summary>
        /// Not implemented.
        /// </summary>
        /// <param name="value">The source string being passed to the target.</param>
        /// <param name="targetType">The type of the target property.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion.</param>
        /// <returns>The value to be passed to the target dependency property.</returns>
        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

    private:
		winrt::ResourceManager _resourceManager;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct ResourceNameToResourceStringConverter : ResourceNameToResourceStringConverterT<ResourceNameToResourceStringConverter, implementation::ResourceNameToResourceStringConverter>
	{

    };
}