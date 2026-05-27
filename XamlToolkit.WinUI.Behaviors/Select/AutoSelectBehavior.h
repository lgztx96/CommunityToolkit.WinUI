#pragma once

#include "AutoSelectBehavior.g.h"
#include "../BehaviorBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// This behavior automatically selects the entire content of the associated TextBox when it is loaded.
    /// </summary>
    struct AutoSelectBehavior : AutoSelectBehaviorT<AutoSelectBehavior>, BehaviorBase<AutoSelectBehavior, winrt::TextBox>
    {
        AutoSelectBehavior() = default;

    protected:
        /// <summary>
        /// Called when the associated object has been loaded.
        /// </summary>
        void OnAssociatedObjectLoaded() override;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct AutoSelectBehavior : AutoSelectBehaviorT<AutoSelectBehavior, implementation::AutoSelectBehavior>
    {
    };
}
