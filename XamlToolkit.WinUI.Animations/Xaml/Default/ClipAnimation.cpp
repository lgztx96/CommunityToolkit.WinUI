#include "pch.h"
#include "winrt_module_imports.h"
#include "ClipAnimation.h"
#if __has_include("ClipAnimation.g.cpp")
#include "ClipAnimation.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    Animations::AnimationBuilder& ClipAnimation::AppendToBuilder(
        Animations::AnimationBuilder& builder,
        std::optional<winrt::Windows::Foundation::TimeSpan> delayHint,
        std::optional<winrt::Windows::Foundation::TimeSpan> durationHint,
        std::optional<enum EasingType> easingTypeHint,
        std::optional<enum EasingMode> easingModeHint)
    {
        auto to = To();

        if (!to)
        {
            throw winrt::hresult_invalid_argument(L"ClipAnimation.To must be set.");
        }

        std::optional<Thickness> from = From();

        return builder.Clip(
            to.Value(),
            from,
            Delay() ? Delay().Value() : delayHint,
            Duration() ? Duration().Value() : durationHint,
            Repeat(),
            EasingType() ? EasingType().Value() : easingTypeHint.value_or(Animations::AnimationExtensions::DefaultEasingType()),
            EasingMode() ? EasingMode().Value() : easingModeHint.value_or(Animations::AnimationExtensions::DefaultEasingMode()));
    }
}
