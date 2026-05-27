#pragma once

#include "FadeHeaderBehavior.g.h"
#include "HeaderBehaviorBase.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// Performs an fade animation on a ListView or GridView Header using composition.
    /// </summary>
    struct FadeHeaderBehavior : FadeHeaderBehaviorT<FadeHeaderBehavior>, HeaderBehaviorBase<FadeHeaderBehavior>
    {
        FadeHeaderBehavior() = default;

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
    struct FadeHeaderBehavior : FadeHeaderBehaviorT<FadeHeaderBehavior, implementation::FadeHeaderBehavior>
    {
    };
}
