#pragma once

#include "Trigger.g.h"
#include "Behavior.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// A base class for behaviors, implementing the basic plumbing of ITrigger
    /// </summary>
    struct Trigger : TriggerT<Trigger, winrt::XamlToolkit::WinUI::Interactivity::implementation::Behavior>
    {
        Trigger() = default;

        /// <summary>
        /// Identifies the <seealso cref="Actions"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ActionsProperty;

        /// <summary>
        /// Gets the collection of actions associated with the behavior. This is a dependency property.
        /// </summary>
        winrt::XamlToolkit::WinUI::Interactivity::ActionCollection Actions();
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct Trigger : TriggerT<Trigger, implementation::Trigger>
    {
    };
}
