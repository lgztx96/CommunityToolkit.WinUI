#pragma once

#include "EmptyObjectToObjectConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// This class converts an object value into a an object (if the value is null returns the false value).
    /// Can be used to bind a visibility, a color or an image to the value of an object.
    /// </summary>
    struct EmptyObjectToObjectConverter : EmptyObjectToObjectConverterT<EmptyObjectToObjectConverter>
    {
        EmptyObjectToObjectConverter() = default;

        /// <summary>
        /// Gets or sets the value to be returned when the object is neither null nor empty.
        /// </summary>
        winrt::IInspectable NotEmptyValue() const;
        void NotEmptyValue(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the value to be returned when the object is either null or empty.
        /// </summary>
        winrt::IInspectable EmptyValue() const;
        void EmptyValue(winrt::IInspectable const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> NotEmptyValueProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> EmptyValueProperty;

        /// <summary>
        /// Convert a boolean value to an other object.
        /// </summary>
        /// <param name="value">The source data being passed to the target.</param>
        /// <param name="targetType">The type of the target property, as a type reference.</param>
        /// <param name="parameter">An optional parameter to be used to invert the converter logic.</param>
        /// <param name="language">The language of the conversion.</param>
        /// <returns>The value to be passed to the target dependency property.</returns>
        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

        /// <summary>
        /// Not implemented.
        /// </summary>
        /// <param name="value">The target data being passed to the source.</param>
        /// <param name="targetType">The type of the target property.</param>
        /// <param name="parameter">An optional parameter to be used to invert the converter logic.</param>
        /// <param name="language">The language of the conversion.</param>
        /// <returns>The value to be passed to the source object.</returns>
        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

    protected:
        /// <summary>
        /// Checks value for emptiness.
        /// </summary>
        /// <param name="value">Value to be checked.</param>
        /// <returns>True if value is null, false otherwise.</returns>
        virtual bool CheckValueIsEmpty(winrt::IInspectable const& value) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct EmptyObjectToObjectConverter : EmptyObjectToObjectConverterT<EmptyObjectToObjectConverter, implementation::EmptyObjectToObjectConverter>
	{

    };
}