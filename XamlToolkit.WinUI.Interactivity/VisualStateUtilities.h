#pragma once

#include "VisualStateUtilities.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// Provides various standard operations for working with <seealso cref="VisualStateManager"/>.
    /// </summary>
    struct VisualStateUtilities
    {
        VisualStateUtilities() = delete;

        /// <summary>
        /// Find the nearest parent which contains visual states.
        /// </summary>
        /// <param name="element">The <see cref="FrameworkElement"/> from which to find the nearest stateful control.</param>
        /// <returns>The nearest <see cref="Control"/> that contains visual states; else null.</returns>
        static winrt::Controls::Control FindNearestStatefulControl(winrt::FrameworkElement const& element);

        /// <summary>
        /// Transitions the control between two states.
        /// </summary>
        /// <param name="control">The <see cref="Control"/> to transition between states.</param>
        /// <param name="stateName">The state to transition to.</param>
        /// <param name="useTransitions">True to use a <see cref="VisualTransition"/> to transition between states; otherwise, false.</param>
        /// <returns>True if the <paramref name="control"/> is successfully transitioned to the new state; otherwise, false.</returns>
        static bool GoToState(winrt::Controls::Control const& control, winrt::hstring const& stateName, bool useTransitions);

        /// <summary>
        /// Gets the value of the VisualStateManager.VisualStateGroups attached property.
        /// </summary>
        /// <param name="element">The <see cref="FrameworkElement"/> from which to get the VisualStateManager.VisualStateGroups.</param>
        /// <returns>The list of VisualStateGroups in the given element.</returns>
        static winrt::IVector<winrt::VisualStateGroup> GetVisualStateGroups(winrt::FrameworkElement const& element);

    private:
        static bool HasVisualStateGroupsDefined(winrt::FrameworkElement const& element);
        static bool ShouldContinueTreeWalk(winrt::FrameworkElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct VisualStateUtilities : VisualStateUtilitiesT<VisualStateUtilities, implementation::VisualStateUtilities>
    {
    };
}
