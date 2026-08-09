#include "pch.h"
#include "winrt_module_imports.h"
#include "StopAnimationActivity.h"
#if __has_include("StopAnimationActivity.g.cpp")
#include "StopAnimationActivity.g.cpp"
#endif
#include "../AnimationSet.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StopAnimationActivity::AnimationProperty =
        winrt::DependencyProperty::Register(
            L"Animation",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::AnimationSet>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> StopAnimationActivity::TargetObjectProperty =
        winrt::DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::IAsyncAction StopAnimationActivity::InvokeAsync(winrt::UIElement element)
    {
        auto strongThis = get_strong();

        auto animation = Animation();

        if (!animation)
        {
            throw winrt::hresult_invalid_argument(L"Animation");
        }

        co_await base_type::InvokeAsync(element);

		auto animationImpl = winrt::get_self<AnimationSet>(animation);

        if (auto target = TargetObject())
        {
            animation.Stop(target);
        }
        else if (!animationImpl->ParentReference())
        {
            animation.Stop(element);
        }
        else
        {
            animation.Stop();
        }
    }
}
