#include "pch.h"
#include "winrt_module_imports.h"
#include "ColorDropShadowAnimation.h"
#if __has_include("ColorDropShadowAnimation.g.cpp")
#include "ColorDropShadowAnimation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    winrt::hstring ColorDropShadowAnimation::ExplicitTarget() const noexcept
    {
        return L"Color";
    }

    winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& ColorDropShadowAnimation::AppendToBuilder(
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
        winrt::UIElement const& parent,
        std::optional<winrt::TimeSpan> delayHint,
        std::optional<winrt::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<winrt::EasingMode> easingModeHint)
    {
        return ShadowAnimationBase<ColorDropShadowAnimationTraits>::AppendToBuilder(builder, parent, delayHint, durationHint, easingTypeHint, easingModeHint);
    }
}