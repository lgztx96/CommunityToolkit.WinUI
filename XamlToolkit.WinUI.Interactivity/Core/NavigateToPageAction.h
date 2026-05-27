#pragma once

#include "NavigateToPageAction.g.h"
#include "../Utility/IVisualTreeHelper.h"

#ifdef __INTELLISENSE__
#include <memory>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Markup;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that switches the current visual to the specified <see cref="Page"/>.
    /// </summary>
    struct NavigateToPageAction : NavigateToPageActionT<NavigateToPageAction>
    {
        /// <summary>
        /// Initializes a new instance of the NavigateToPageAction class.
        /// </summary>
        NavigateToPageAction();

        /// <summary>
        /// Identifies the <seealso cref="TargetPage"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetPageProperty;
        /// <summary>
        /// Identifies the <seealso cref="Parameter"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ParameterProperty;

        /// <summary>
        /// Gets or sets the fully qualified name of the <see cref="Page"/> to navigate to. This is a dependency property.
        /// </summary>
        winrt::hstring TargetPage() const;
        void TargetPage(winrt::hstring const& value);

        /// <summary>
        /// Gets or sets the parameter which will be passed to the <see cref="Frame.Navigate"/> method.
        /// </summary>
        winrt::IInspectable Parameter() const;
        void Parameter(winrt::IInspectable const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if the navigation to the specified page is successful; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);

    private:
        std::unique_ptr<winrt::XamlToolkit::WinUI::Interactivity::IVisualTreeHelper> _visualTreeHelper;
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct NavigateToPageAction : NavigateToPageActionT<NavigateToPageAction, implementation::NavigateToPageAction>
    {
    };
}
