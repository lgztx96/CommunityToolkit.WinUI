#pragma once

#include "ImplicitAnimation{TValue,TKeyFrame}.h"
#include "../Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// C++ counterpart for C# CustomAnimation<TValue, TKeyFrame>.
    /// </summary>
    template<AnimationBaseTraits TTraits>
    struct CustomAnimationBase : ImplicitAnimationBase<TTraits>
    {
        using traits_type = TTraits;
        using parsed_value_type = typename traits_type::parsed_value_type;

        CustomAnimationBase() = default;

        /// <summary>
        /// Gets or sets the explicit target path used by this custom animation.
        /// </summary>
        wil::single_threaded_rw_property<winrt::hstring> Target = L"";
        /// <summary>
        /// Gets or sets the framework layer targeted by this custom animation.
        /// This is ignored when the animation is being used as an implicit composition animation.
        /// </summary>
        wil::single_threaded_rw_property<FrameworkLayer> Layer = FrameworkLayer::Composition;

        /// <inheritdoc/>
        winrt::hstring ExplicitTarget() const override
        {
            return Target();
        }

        /// <inheritdoc/>
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt) override
        {
            auto explicitTarget = ExplicitTarget();

            if (explicitTarget.empty())
            {
                throw winrt::hresult_invalid_argument(L"Target property cannot be null");
            }

            return builder.NormalizedKeyFrames<parsed_value_type>(
                explicitTarget,
                [this, easingTypeHint, easingModeHint](Animations::INormalizedKeyFrameAnimationBuilder<parsed_value_type>& keyFrameBuilder)
                {
                    this->AnimationBase<TTraits>::AppendToBuilder(keyFrameBuilder, easingTypeHint, easingModeHint);
                },
                this->Delay() ? this->Delay().Value() : delayHint,
                this->Duration() ? this->Duration().Value() : durationHint,
                this->Repeat(),
                this->DelayBehavior(),
                Layer());
        }
    };
}

