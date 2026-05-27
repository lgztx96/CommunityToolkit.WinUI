#pragma once

#include "NavigateToUriAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// NavigateToUriAction represents an action that allows navigate to a specified URL defined in XAML, similiar to a Hyperlink and HyperlinkButton. No action will be invoked if the Uri cannot be navigated to.
    /// </summary>
    struct NavigateToUriAction : NavigateToUriActionT<NavigateToUriAction>
    {
        NavigateToUriAction() = default;

        /// <summary>
        /// Gets or sets the Uniform Resource Identifier (URI) to navigate to when the object is clicked.
        /// </summary>
        winrt::Uri NavigateUri() const
        {
            return GetValue(NavigateUriProperty()).try_as<winrt::Uri>();
        }
        void NavigateUri(winrt::Uri const& value)
        {
            SetValue(NavigateUriProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Identifies the NavigateUri dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> NavigateUriProperty;

        /// <summary>
        /// Executes the action.
        /// </summary>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct NavigateToUriAction : NavigateToUriActionT<NavigateToUriAction, implementation::NavigateToUriAction>
    {
    };
}
