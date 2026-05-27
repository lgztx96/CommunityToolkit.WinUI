#pragma once

#include "AnimationCompletedTriggerBehavior.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// A custom Trigger that fires whenever a linked AnimationSet completes.
    /// </summary>
    struct AnimationCompletedTriggerBehavior : AnimationCompletedTriggerBehaviorT<AnimationCompletedTriggerBehavior>
    {
        AnimationCompletedTriggerBehavior() = default;

        /// <summary>
        /// Called after the behavior is attached to the AssociatedObject.
        /// </summary>
        void OnAttached();

        /// <summary>
        /// Called when the behavior is being detached from its AssociatedObject.
        /// </summary>
        void OnDetaching();

    private:
        /// <summary>
        /// Sets the current AnimationSet instance in use.
        /// </summary>
        /// <param name="animationSet">The AnimationSet instance in use.</param>
        void SetResolvedCollection(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& animationSet);

        /// <summary>
        /// Invokes the current actions when the linked animations completes.
        /// </summary>
        /// <param name="sender">The source AnimationSet instance.</param>
        /// <param name="e">The arguments for the event (unused).</param>
        void OnAnimationCompleted(winrt::IInspectable const& sender, winrt::IInspectable const& e);

        winrt::XamlToolkit::WinUI::Animations::AnimationSet _animationSet{ nullptr };
        winrt::event_token _completedToken{};
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct AnimationCompletedTriggerBehavior : AnimationCompletedTriggerBehaviorT<AnimationCompletedTriggerBehavior, implementation::AnimationCompletedTriggerBehavior>
    {
    };
}
