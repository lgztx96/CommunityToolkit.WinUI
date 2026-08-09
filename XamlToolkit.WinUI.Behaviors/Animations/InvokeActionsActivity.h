#pragma once

#include "InvokeActionsActivity.g.h"

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
    /// The InvokeActionsActivity is an Activity which allows bridging to performing any behavior based IAction within the schedule.
    /// </summary>
    struct InvokeActionsActivity : InvokeActionsActivityT<InvokeActionsActivity>
    {
        InvokeActionsActivity() = default;

        /// <summary>
        /// Identifies the Actions dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ActionsProperty;

        /// <summary>
        /// Gets the collection of actions associated with the behavior. This is a dependency property.
        /// </summary>
        winrt::XamlToolkit::WinUI::Interactivity::ActionCollection Actions();

        /// <summary>
        /// Invokes the activity asynchronously.
        /// </summary>
        /// <param name="element">The element.</param>
        winrt::Windows::Foundation::IAsyncAction InvokeAsync(winrt::UIElement element);
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct InvokeActionsActivity : InvokeActionsActivityT<InvokeActionsActivity, implementation::InvokeActionsActivity>
    {
    };
}
