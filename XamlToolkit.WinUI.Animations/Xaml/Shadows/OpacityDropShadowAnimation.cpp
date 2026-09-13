#include "pch.h"
#include "winrt_module_imports.h"
#include "OpacityDropShadowAnimation.h"
#if __has_include("OpacityDropShadowAnimation.g.cpp")
#include "OpacityDropShadowAnimation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    winrt::hstring OpacityDropShadowAnimation::ExplicitTarget() const noexcept
    {
        return L"Opacity";
    }

    winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& OpacityDropShadowAnimation::AppendToBuilder(
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
        winrt::UIElement const& parent,
        std::optional<winrt::TimeSpan> delayHint,
        std::optional<winrt::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<winrt::EasingMode> easingModeHint)
    {
        return ShadowAnimationBase<OpacityDropShadowAnimationTraits>::AppendToBuilder(builder, parent, delayHint, durationHint, easingTypeHint, easingModeHint);
    }
}