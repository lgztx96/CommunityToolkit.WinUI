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
    const wil::single_threaded_property<winrt::DependencyProperty> AnimationScope::DelayProperty =
        winrt::DependencyProperty::Register(
            L"Delay",
            winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> AnimationScope::DurationProperty =
        winrt::DependencyProperty::Register(
            L"Duration",
            winrt::xaml_typename<winrt::IReference<winrt::TimeSpan>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> AnimationScope::EasingTypeProperty =
        winrt::DependencyProperty::Register(
            L"EasingType",
            winrt::xaml_typename<winrt::IReference<enum EasingType>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> AnimationScope::EasingModeProperty =
        winrt::DependencyProperty::Register(
            L"EasingMode",
            winrt::xaml_typename<winrt::IReference<winrt::EasingMode>>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    Animations::AnimationBuilder& AnimationScope::AppendToBuilder(
        Animations::AnimationBuilder& builder,
        std::optional<winrt::TimeSpan> delayHint,
        std::optional<winrt::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<winrt::EasingMode> easingModeHint)
    {
        auto delay = Delay();
        auto duration = Duration();
        auto easingType = EasingType();
        auto easingMode = EasingMode();

        auto effectiveDelay = delay ? delay.Value() : delayHint;
        auto effectiveDuration = duration ? duration.Value() : durationHint;
        auto effectiveEasingType = easingType ? easingType.Value() : easingTypeHint;
        auto effectiveEasingMode = easingMode ? easingMode.Value() : easingModeHint;

        for (const auto& element : *this)
        {
            if (auto animation = element.try_as<winrt::XamlToolkit::WinUI::Animations::Animation>())
            {
                if (auto impl = animation.try_as<ITimeline>())
                {
                    impl->AppendToBuilder(
                        builder,
                        effectiveDelay,
                        effectiveDuration,
                        effectiveEasingType,
                        effectiveEasingMode);
                }
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
