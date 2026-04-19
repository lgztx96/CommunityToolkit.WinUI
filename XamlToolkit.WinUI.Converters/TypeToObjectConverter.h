#pragma once

#include "TypeToObjectConverter.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// This class returns an object or another, depending on whether the type of the provided value matches another provided Type.
    /// </summary>
    struct TypeToObjectConverter : TypeToObjectConverterT<TypeToObjectConverter>
    {
        TypeToObjectConverter() = default;

        /// <summary>
        /// Gets or sets the value to be returned when the type of the provided value matches Type.
        /// </summary>
        winrt::IInspectable TrueValue() const;
        void TrueValue(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the value to be returned when the type of the provided value does not match Type.
        /// </summary>
        winrt::IInspectable FalseValue() const;
        void FalseValue(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the Type used to compare the type of the provided value.
        /// </summary>
        winrt::TypeName Type() const;
        void Type(winrt::TypeName const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TrueValueProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> FalseValueProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TypeProperty;

        /// <summary>
        /// Convert the value's Type to an other object.
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
        /// <param name="value">The source data being passed to the target.</param>
        /// <param name="targetType">The type of the target property.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion. Not used.</param>
        /// <returns>The value to be passed to the target dependency property.</returns>
        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct TypeToObjectConverter : TypeToObjectConverterT<TypeToObjectConverter, implementation::TypeToObjectConverter>
	{

    };
}