#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationStartedTriggerBehavior.h"
#if __has_include("AnimationStartedTriggerBehavior.g.cpp")
#include "AnimationStartedTriggerBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    void AnimationStartedTriggerBehavior::OnAttached()
    {
        base_type::OnAttached();
        SetResolvedCollection(this->AssociatedObject().try_as<winrt::XamlToolkit::WinUI::Animations::AnimationSet>());
    }

    void AnimationStartedTriggerBehavior::OnDetaching()
    {
        base_type::OnDetaching();
        SetResolvedCollection(nullptr);
    }

    void AnimationStartedTriggerBehavior::SetResolvedCollection(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& animationSet)
    {
        if (_animationSet == animationSet)
        {
            return;
        }

        if (_animationSet)
        {
            _animationSet.Started(_startedToken);
        }

        _animationSet = animationSet;

        if (_animationSet)
        {
            _startedToken = _animationSet.Started({ this, &AnimationStartedTriggerBehavior::OnAnimationStarted });
        }
    }

    void AnimationStartedTriggerBehavior::OnAnimationStarted(winrt::IInspectable const& sender, winrt::IInspectable const& e)
    {
        winrt::XamlToolkit::WinUI::Interactivity::Interaction::ExecuteActions(sender, this->Actions(), e);
    }
}
