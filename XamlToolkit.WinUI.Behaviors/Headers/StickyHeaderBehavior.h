#pragma once

#include "StickyHeaderBehavior.g.h"
#include "HeaderBehaviorBase.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// Performs an animation on a ListView or GridView Header to make it 'sticky' and remain in view using composition.
    /// </summary>
    struct StickyHeaderBehavior : StickyHeaderBehaviorT<StickyHeaderBehavior>, HeaderBehaviorBase<StickyHeaderBehavior>
    {
        StickyHeaderBehavior() = default;

        /// <summary>
        /// Show the header
        /// </summary>
        void Show();

    protected:
        /// <summary>
        /// Uses Composition API to get the UIElement and sets an ExpressionAnimation.
        /// </summary>
        /// <returns>true if the assignment was successful; otherwise, false.</returns>
        bool AssignAnimation() override;

        /// <summary>
        /// Stops the animation.
        /// </summary>
        void StopAnimation() override;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct StickyHeaderBehavior : StickyHeaderBehaviorT<StickyHeaderBehavior, implementation::StickyHeaderBehavior>
    {
    };
}
