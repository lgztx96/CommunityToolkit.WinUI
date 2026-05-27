#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationScope.h"
#if __has_include("AnimationScope.g.cpp")
#include "AnimationScope.g.cpp"
#endif
#include "Interfaces/ITimeline.h"
#include "Interfaces/IAttachedTimeline.h"
#include "Abstract/Animation.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> AnimationScope::DelayProperty =
        DependencyProperty::Register(
            L"Delay",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> AnimationScope::DurationProperty =
        DependencyProperty::Register(
            L"Duration",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> AnimationScope::EasingTypeProperty =
        DependencyProperty::Register(
            L"EasingType",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<enum EasingType>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<DependencyProperty> AnimationScope::EasingModeProperty =
        DependencyProperty::Register(
            L"EasingMode",
            winrt::xaml_typename<winrt::Windows::Foundation::IReference<enum EasingMode>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    Animations::AnimationBuilder& AnimationScope::AppendToBuilder(
        Animations::AnimationBuilder& builder,
        std::optional<winrt::Windows::Foundation::TimeSpan> delayHint,
        std::optional<winrt::Windows::Foundation::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<enum EasingMode> easingModeHint)
    {
        auto delay = Delay();
        auto duration = Duration();
        auto easingType = EasingType();
        auto easingMode = EasingMode();

        auto effectiveDelay = delay ? delay.Value() : delayHint;
        auto effectiveDuration = duration ? duration.Value() : durationHint;
        auto effectiveEasingType = easingType ? easingType.Value() : easingTypeHint;
        auto effectiveEasingMode = easingMode ? easingMode.Value() : easingModeHint;

        for (auto const& element : *this)
        {
            if (auto animation = element.try_as<winrt::XamlToolkit::WinUI::Animations::Animation>())
            {
                auto impl = winrt::get_self<winrt::XamlToolkit::WinUI::Animations::implementation::Animation>(animation);

                impl->AppendToBuilder(
                    builder,
                    effectiveDelay,
                    effectiveDuration,
                    effectiveEasingType,
                    effectiveEasingMode);
            }
            else if (auto scope = element.try_as<winrt::XamlToolkit::WinUI::Animations::AnimationScope>())
            {
                auto impl = winrt::get_self<winrt::XamlToolkit::WinUI::Animations::implementation::AnimationScope>(scope);

                impl->AppendToBuilder(
                    builder,
                    effectiveDelay,
                    effectiveDuration,
                    effectiveEasingType,
                    effectiveEasingMode);
            }
            else
            {
                throw winrt::hresult_invalid_argument(L"AnimationScope can only contain timeline nodes.");
            }
        }

        return builder;
    }
}
