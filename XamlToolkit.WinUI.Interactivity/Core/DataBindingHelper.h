#pragma once

#include "ActionCollection.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Data;
}

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    /// <summary>
    /// A helper class that ensures all binding expressions on actions are up to date.
    /// </summary>
    struct DataBindingHelper
    {
        /// <summary>
        /// Ensures that all binding expression on actions are up to date.
        /// </summary>
        /// <remarks>
        /// DataTriggerBehavior fires during data binding phase. Since the ActionCollection is a child of the behavior,
        /// bindings on the action may not be up-to-date. This routine is called before the action
        /// is executed in order to guarantee that all bindings are refreshed with the most current data.
        /// </remarks>
        static void RefreshDataBindingsOnActions(winrt::XamlToolkit::WinUI::Interactivity::ActionCollection const& actions);

    private:
        static void RefreshBinding(winrt::DependencyObject const& target, winrt::DependencyProperty const& property);
    };
}
