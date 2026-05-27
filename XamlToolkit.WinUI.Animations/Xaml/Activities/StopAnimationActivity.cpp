#include "pch.h"
#include "winrt_module_imports.h"
#include "StopAnimationActivity.h"
#if __has_include("StopAnimationActivity.g.cpp")
#include "StopAnimationActivity.g.cpp"
#endif
#include "../AnimationSet.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> StopAnimationActivity::AnimationProperty =
        DependencyProperty::Register(
            L"Animation",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::AnimationSet>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> StopAnimationActivity::TargetObjectProperty =
        DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::Windows::Foundation::IAsyncAction StopAnimationActivity::InvokeAsync(UIElement const& element)
    {
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
