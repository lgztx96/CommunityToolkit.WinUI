#include "pch.h"
#include "winrt_module_imports.h"
#include "OffsetDropShadowAnimation.h"
#if __has_include("OffsetDropShadowAnimation.g.cpp")
#include "OffsetDropShadowAnimation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    winrt::hstring OffsetDropShadowAnimation::ExplicitTarget() const noexcept
    {
        return L"Offset";
    }

    winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& OffsetDropShadowAnimation::AppendToBuilder(
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
        winrt::UIElement const& parent,
        std::optional<winrt::TimeSpan> delayHint,
        std::optional<winrt::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<winrt::EasingMode> easingModeHint)
    {
        return ShadowAnimationBase<OffsetDropShadowAnimationTraits>::AppendToBuilder(builder, parent, delayHint, durationHint, easingTypeHint, easingModeHint);
    }
}