#pragma once

#include "BoolNegationConverter.g.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    /// <summary>
    /// Value converter that applies NOT operator to a bool value.
    /// </summary>
    struct BoolNegationConverter : BoolNegationConverterT<BoolNegationConverter>
    {
        BoolNegationConverter() = default;

        /// <summary>
        /// Convert a boolean value to its negation.
        /// </summary>
        /// <param name="value">The bool value to negate.</param>
        /// <param name="targetType">The type of the target property, as a type reference.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion. Not used.</param>
        /// <returns>The value to be passed to the target dependency property.</returns>
        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;

        /// <summary>
        /// Convert back a boolean value to its negation.
        /// </summary>
        /// <param name="value">The bool value to negate.</param>
        /// <param name="targetType">The type of the target property, as a type reference.</param>
        /// <param name="parameter">Optional parameter. Not used.</param>
        /// <param name="language">The language of the conversion. Not used.</param>
        /// <returns>The value to be passed to the target dependency property.</returns>
        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
	struct BoolNegationConverter : BoolNegationConverterT<BoolNegationConverter, implementation::BoolNegationConverter>
	{

    };
}