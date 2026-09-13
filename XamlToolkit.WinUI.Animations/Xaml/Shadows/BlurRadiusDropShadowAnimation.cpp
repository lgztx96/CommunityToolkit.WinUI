#include "pch.h"
#include "winrt_module_imports.h"
#include "BlurRadiusDropShadowAnimation.h"
#if __has_include("BlurRadiusDropShadowAnimation.g.cpp")
#include "BlurRadiusDropShadowAnimation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    winrt::hstring BlurRadiusDropShadowAnimation::ExplicitTarget() const noexcept
    {
        return L"BlurRadius";
    }

    winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& BlurRadiusDropShadowAnimation::AppendToBuilder(
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
        winrt::UIElement const& parent,
        std::optional<winrt::TimeSpan> delayHint,
        std::optional<winrt::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<winrt::EasingMode> easingModeHint)
    {
        return ShadowAnimationBase<BlurRadiusDropShadowAnimationTraits>::AppendToBuilder(builder, parent, delayHint, durationHint, easingTypeHint, easingModeHint);
    }
}