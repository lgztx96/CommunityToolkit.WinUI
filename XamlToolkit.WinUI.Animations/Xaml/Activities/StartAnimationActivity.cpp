#include "pch.h"
#include "winrt_module_imports.h"
#include "StartAnimationActivity.h"
#if __has_include("StartAnimationActivity.g.cpp")
#include "StartAnimationActivity.g.cpp"
#endif
#include "../AnimationSet.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StartAnimationActivity::AnimationProperty =
        winrt::DependencyProperty::Register(
            L"Animation",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::AnimationSet>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> StartAnimationActivity::TargetObjectProperty =
        winrt::DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::IAsyncAction StartAnimationActivity::InvokeAsync(winrt::UIElement element)
    {
        auto strongThis = get_strong();

        auto animation = Animation();

        if (!animation)
        {
            throw winrt::hresult_invalid_argument(L"Animation");
        }

        co_await base_type::InvokeAsync(element);

        auto animationImpl = winrt::get_self<AnimationSet>(animation);

        // If we've specified an explicit target for the animation, we can use that. Otherwise, we can
        // check whether the target animation has an implicit parent. If that's the case, we will use
        // that to start the animation, or just use the input (usually the parent) as fallback.
        if (auto target = TargetObject())
        {
            co_await animation.StartAsync(target);
        }
        else if (!animationImpl->ParentReference())
        {
            co_await animation.StartAsync(element);
        }
        else
        {
            co_await animation.StartAsync();
        }
    }
}
