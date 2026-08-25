#include "pch.h"
#include "winrt_module_imports.h"
#include "StartAnimationAction.h"
#if __has_include("StartAnimationAction.g.cpp")
#include "StartAnimationAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StartAnimationAction::AnimationProperty =
        winrt::DependencyProperty::Register(
            L"Animation",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::AnimationSet>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> StartAnimationAction::TargetObjectProperty =
        winrt::DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::IInspectable StartAnimationAction::Execute(
        winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::IInspectable const& parameter) const
    {
        auto animation = Animation();
        if (!animation)
        {
            throw winrt::hresult_invalid_argument(L"Animation");
        }

        if (auto target = TargetObject())
        {
            animation.Start(target);
        }
        else if (auto animationSetOwner = animation.try_as<winrt::XamlToolkit::WinUI::Animations::IAnimationSetOwner>()) //// TODO: Tidy... apply same pattern to Activities?
        {
            if (auto parent = animationSetOwner.ParentReference())
            {
                animation.Start(parent);
            }
        }
        else
        {
            animation.Start(sender.try_as<winrt::UIElement>());
        }

        return nullptr;
    }
}
