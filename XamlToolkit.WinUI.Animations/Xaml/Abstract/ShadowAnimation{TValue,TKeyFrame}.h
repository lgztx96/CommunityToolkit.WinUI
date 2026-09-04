#pragma once

#include "Animation{TValue,TKeyFrame}.h"
#include "../Interfaces/IAttachedTimeline.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <optional>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// C++ counterpart for C# ShadowAnimation<TValue, TKeyFrame>.
    /// </summary>
    template<typename TTraits>
    struct ShadowAnimationBase : public AnimationBase<TTraits>, public IAttachedTimeline
    {
        using traits_type = TTraits;
        using parsed_value_type = typename traits_type::parsed_value_type;

        ShadowAnimationBase() = default;

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TargetProperty =
            winrt::DependencyProperty::Register(
                L"Target",
                winrt::xaml_typename<winrt::XamlToolkit::WinUI::IAttachedShadow>(),
                winrt::xaml_typename<traits_type::class_type>(),
                winrt::PropertyMetadata(nullptr));

        /// <summary>
        /// Gets or sets the linked <see cref="IAttachedShadow"/> instance to animate.
        /// </summary>
        winrt::XamlToolkit::WinUI::IAttachedShadow Target() const
        {
            return Animation::GetValue(TargetProperty()).try_as<winrt::XamlToolkit::WinUI::IAttachedShadow>();
        }

        /// <summary>
        /// Gets or sets the linked <see cref="IAttachedShadow"/> instance to animate.
        /// </summary>
        void Target(winrt::XamlToolkit::WinUI::IAttachedShadow const& value)
        {
            Animation::SetValue(TargetProperty(), value);
        }

        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder&,
            [[maybe_unused]] std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            [[maybe_unused]] std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            [[maybe_unused]] std::optional<enum EasingType> easingTypeHint = std::nullopt,
            [[maybe_unused]] std::optional<winrt::EasingMode> easingModeHint = std::nullopt) override
        {
            throw winrt::hresult_illegal_method_call(L"Shadow animations require a parent UIElement context.");
        }

        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& AppendToBuilder(
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            winrt::UIElement const& parent,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt) override
        {
            auto explicitTarget = this->ExplicitTarget();

            if (explicitTarget.empty())
            {
                throw winrt::hresult_invalid_argument(L"The target shadow cannot be animated at this time.");
            }

            auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderComposition<parsed_value_type>(
                explicitTarget,
                this->Delay() ? this->Delay().Value() : delayHint.value_or(AnimationExtensions::DefaultDelay()),
                this->Duration() ? this->Duration().Value() : durationHint.value_or(AnimationExtensions::DefaultDuration()),
                this->Repeat(),
                this->DelayBehavior());

            this->AnimationBase<TTraits>::AppendToBuilder(
                keyFrameBuilder,
                easingTypeHint,
                easingModeHint);

            auto appendAnimation = [&](winrt::DropShadow const& shadow)
            {
                if (!shadow)
                {
                    return;
                }

                winrt::CompositionObject targetObject{ nullptr };
                auto animation = keyFrameBuilder.GetAnimation(shadow, targetObject);
                builder.ExternalAnimation(targetObject ? targetObject : shadow, animation);
            };

            if (auto target = Target())
            {
                for (const auto& context : target.EnumerateElementContexts())
                {
                    appendAnimation(context.Shadow());
                }

                return builder;
            }

            if (auto frameworkElement = parent.try_as<winrt::FrameworkElement>())
            {
                if (auto shadowBase = winrt::XamlToolkit::WinUI::Effects::GetShadow(frameworkElement))
                {
                    appendAnimation(shadowBase.GetElementContext(frameworkElement).Shadow());
                    return builder;
                }
            }

            return builder;
        }
    };
}
