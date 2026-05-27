#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Markup;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    /// <summary>
    /// A helper class that enables converting values specified in markup (strings) to their object representation.
    /// </summary>
    struct TypeConverterHelper
    {
        /// <summary>
        /// Converts string representation of a value to its object representation.
        /// </summary>
        /// <param name="value">The value to convert.</param>
        /// <param name="destinationTypeFullName">The full name of the destination type.</param>
        /// <returns>Object representation of the string value.</returns>
        static winrt::IInspectable Convert(winrt::hstring const& value, winrt::hstring const& destinationTypeFullName);

    private:
        static winrt::hstring GetScope(winrt::hstring const& name);
        static winrt::hstring GetType(winrt::hstring const& name);
        static winrt::hstring NormalizeTypeName(winrt::hstring const& name);
    };
}
