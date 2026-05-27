#pragma once

#include "FileSizeToFriendlyStringConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// Converts a file size in bytes to a more human-readable friendly format.
    /// </summary>
    struct FileSizeToFriendlyStringConverter : FileSizeToFriendlyStringConverterT<FileSizeToFriendlyStringConverter>
    {
        FileSizeToFriendlyStringConverter() = default;

        /// <summary>
        /// Convert a file size in bytes to a human-readable string.
        /// </summary>
        /// <param name="value">The source data being passed to the target.</param>
        /// <param name="targetType">The type of the target property, as a type reference.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion. Not used.</param>
        /// <returns>The formatted file size string.</returns>
        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

        /// <summary>
        /// Not implemented.
        /// </summary>
        /// <param name="value">The target data being passed to the source.</param>
        /// <param name="targetType">The type of the target property.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion.</param>
        /// <returns>The value to be passed to the source object.</returns>
        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

    private:
        /// <summary>
        /// Convert a file size to a human-readable string.
        /// </summary>
        /// <param name="size">The file size in bytes.</param>
        /// <returns>The formatted string.</returns>
        static winrt::hstring ToFileSizeString(int64_t size);
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct FileSizeToFriendlyStringConverter : FileSizeToFriendlyStringConverterT<FileSizeToFriendlyStringConverter, implementation::FileSizeToFriendlyStringConverter>
	{

    };
}