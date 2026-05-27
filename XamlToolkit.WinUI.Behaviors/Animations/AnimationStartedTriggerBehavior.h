#pragma once

#include "AnimationStartedTriggerBehavior.g.h"

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
    /// A custom Trigger that fires whenever a linked AnimationSet starts.
    /// </summary>
    struct AnimationStartedTriggerBehavior : AnimationStartedTriggerBehaviorT<AnimationStartedTriggerBehavior>
    {
        AnimationStartedTriggerBehavior() = default;

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
        /// Invokes the current actions when the linked animations starts.
        /// </summary>
        /// <param name="sender">The source AnimationSet instance.</param>
        /// <param name="e">The arguments for the event (unused).</param>
        void OnAnimationStarted(winrt::IInspectable const& sender, winrt::IInspectable const& e);

        winrt::XamlToolkit::WinUI::Animations::AnimationSet _animationSet{ nullptr };
        winrt::event_token _startedToken{};
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct AnimationStartedTriggerBehavior : AnimationStartedTriggerBehaviorT<AnimationStartedTriggerBehavior, implementation::AnimationStartedTriggerBehavior>
    {
    };
}
