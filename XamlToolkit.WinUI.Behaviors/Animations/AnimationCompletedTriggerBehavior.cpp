#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationCompletedTriggerBehavior.h"
#if __has_include("AnimationCompletedTriggerBehavior.g.cpp")
#include "AnimationCompletedTriggerBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    void AnimationCompletedTriggerBehavior::OnAttached()
    {
        base_type::OnAttached();
        SetResolvedCollection(AssociatedObject().try_as<winrt::XamlToolkit::WinUI::Animations::AnimationSet>());
    }

    void AnimationCompletedTriggerBehavior::OnDetaching()
    {
        base_type::OnDetaching();
        SetResolvedCollection(nullptr);
    }

    void AnimationCompletedTriggerBehavior::SetResolvedCollection(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& animationSet)
    {
        if (_animationSet == animationSet)
        {
            return;
        }

        if (_animationSet)
        {
            _animationSet.Completed(_completedToken);
        }

        _animationSet = animationSet;

        if (_animationSet)
        {
            _completedToken = _animationSet.Completed({ this, &AnimationCompletedTriggerBehavior::OnAnimationCompleted });
        }
    }

    void AnimationCompletedTriggerBehavior::OnAnimationCompleted(winrt::IInspectable const& sender, winrt::IInspectable const& e)
    {
        winrt::XamlToolkit::WinUI::Interactivity::Interaction::ExecuteActions(sender, Actions(), e);
    }
}
