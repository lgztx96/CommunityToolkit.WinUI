#pragma once

#include "HyperlinkExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Documents;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct HyperlinkExtensions
    {
        /// <summary>
        /// Attached <see cref="DependencyProperty"/> for binding an <see cref="ICommand"/> instance to a <see cref="Hyperlink"/>
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> CommandProperty;

        /// <summary>
        /// Attached <see cref="DependencyProperty"/> for binding a command parameter to a <see cref="Hyperlink"/>
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> CommandParameterProperty;

        /// <summary>
        /// Gets the <see cref="ICommand"/> instance associated with the specified <see cref="Hyperlink"/>
        /// </summary>
        /// <param name="obj">The <see cref="Hyperlink"/> from which to get the associated <see cref="ICommand"/> instance</param>
        /// <returns>The <see cref="ICommand"/> instance associated with the <see cref="Hyperlink"/> or null</returns>
        static winrt::ICommand GetCommand(winrt::Hyperlink const& obj);

        /// <summary>
        /// Sets the <see cref="ICommand"/> instance associated with the specified <see cref="Hyperlink"/>
        /// </summary>
        /// <param name="obj">The <see cref="Hyperlink"/> to associated the <see cref="ICommand"/> instance to</param>
        /// <param name="value">The <see cref="ICommand"/> instance to bind to the <see cref="Hyperlink"/></param>
        static void SetCommand(winrt::Hyperlink const& obj, winrt::ICommand const& value);

        /// <summary>
        /// Gets the <see cref="CommandProperty"/> instance associated with the specified <see cref="Hyperlink"/>
        /// </summary>
        /// <param name="obj">The <see cref="Hyperlink"/> from which to get the associated <see cref="CommandParameterProperty"/> value</param>
        /// <returns>The <see cref="CommandParameterProperty"/> value associated with the <see cref="Hyperlink"/> or null</returns>
        static winrt::IInspectable GetCommandParameter(winrt::Hyperlink const& obj);

        /// <summary>
        /// Sets the <see cref="CommandProperty"/> associated with the specified <see cref="Hyperlink"/>
        /// </summary>
        /// <param name="obj">The <see cref="Hyperlink"/> to associated the <see cref="CommandParameterProperty"/> instance to</param>
        /// <param name="value">The <see cref="object"/> to set the <see cref="CommandParameterProperty"/> to</param>
        static void SetCommandParameter(winrt::Hyperlink const& obj, winrt::IInspectable const& value);

    private:
        static void OnCommandPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        static void OnHyperlinkClicked(winrt::Hyperlink const& sender, winrt::HyperlinkClickEventArgs const& args);

        static const wil::single_threaded_property<winrt::DependencyProperty> ClickEventTokenProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct HyperlinkExtensions : HyperlinkExtensionsT<HyperlinkExtensions, implementation::HyperlinkExtensions>
    {
    };
}
