#pragma once

#include "Animation.h"
#include "../Extensions/AnimationExtensions.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "../Builders/NormalizedKeyFrameAnimationBuilder{T}.Xaml.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#include <tuple>
#include <concepts>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    template<typename TTraits>
    concept AnimationBaseTraits =
        !std::is_void_v<TTraits> &&
        requires
        {
            typename TTraits::owner_type;
            typename TTraits::class_type;
            typename TTraits::public_value_type;
            typename TTraits::parsed_value_type;
            typename TTraits::keyframe_impl_type;
            typename TTraits::keyframe_type;
            { TTraits::Parse(std::declval<typename TTraits::public_value_type const&>()) } -> std::same_as<std::optional<typename TTraits::parsed_value_type>>;
        };

    /// <summary>
    /// C++ counterpart for C# Animation<TValue, TKeyFrame>.
    /// WinRT IDL doesn't support generics, so this is a C++-only layering helper.
    /// </summary>
    template<AnimationBaseTraits TTraits>
    struct AnimationBase : public Animation
    {
        using traits_type = TTraits;
        using public_value_type = typename traits_type::public_value_type;
        using parsed_value_type = typename traits_type::parsed_value_type;
        using keyframe_impl_type = typename traits_type::keyframe_impl_type;
        using keyframe_type = typename traits_type::keyframe_type;

        AnimationBase() = default;

        static inline const wil::single_threaded_property<DependencyProperty> ToProperty =
            DependencyProperty::Register(
                L"To",
                winrt::xaml_typename<public_value_type>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                nullptr);

        static inline const wil::single_threaded_property<DependencyProperty> FromProperty =
            DependencyProperty::Register(
                L"From",
                winrt::xaml_typename<public_value_type>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                nullptr);

        static inline const wil::single_threaded_property<DependencyProperty> KeyFramesProperty =
            DependencyProperty::Register(
                L"KeyFrames",
                winrt::xaml_typename<winrt::Windows::Foundation::Collections::IVector<keyframe_type>>(),
                winrt::xaml_typename<typename traits_type::class_type>(),
                nullptr);

        /// <summary>
        /// Gets or sets the final value for the animation.
        /// </summary>
        public_value_type To() const
        {
            return UnboxOrDefault<public_value_type>(GetValue(ToProperty()));
        }

        /// <summary>
        /// Gets or sets the final value for the animation.
        /// </summary>
        void To(public_value_type const& value)
        {
            SetValue(ToProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional starting value for the animation.
        /// </summary>
        public_value_type From() const
        {
            return UnboxOrDefault<public_value_type>(GetValue(FromProperty()));
        }

        /// <summary>
        /// Gets or sets the optional starting value for the animation.
        /// </summary>
        void From(public_value_type const& value)
        {
            SetValue(FromProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional keyframe collection for the current animation.
        /// Setting this will overwrite the <see cref="To"/> and <see cref="From"/> values.
        /// </summary>
        winrt::Windows::Foundation::Collections::IVector<keyframe_type> KeyFrames()
        {
            auto keyFrames = GetValue(KeyFramesProperty()).try_as<winrt::Windows::Foundation::Collections::IVector<keyframe_type>>();

            if (!keyFrames)
            {
                keyFrames = winrt::single_threaded_vector<keyframe_type>();
                SetValue(KeyFramesProperty(), winrt::box_value(keyFrames));
            }

            return keyFrames;
        }

        /// <summary>
        /// Gets or sets the optional keyframe collection for the current animation.
        /// Setting this will overwrite the <see cref="To"/> and <see cref="From"/> values.
        /// </summary>
        void KeyFrames(winrt::Windows::Foundation::Collections::IVector<keyframe_type> const& value)
        {
            SetValue(KeyFramesProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets the explicit property path used when appending this animation to a builder.
        /// </summary>
        virtual winrt::hstring ExplicitTarget() const = 0;

        /// <inheritdoc/>
        Animations::AnimationBuilder& AppendToBuilder(
            Animations::AnimationBuilder& builder,
            std::optional<winrt::Windows::Foundation::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> durationHint = std::nullopt,
            std::optional<enum EasingType> easingTypeHint = std::nullopt,
            std::optional<enum EasingMode> easingModeHint = std::nullopt) override
        {
            auto explicitTarget = ExplicitTarget();

            if (explicitTarget.empty())
            {
                throw winrt::hresult_invalid_argument(L"ExplicitTarget property cannot be null");
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
                this->DelayBehavior());
        }

    protected:
        template<typename TValue>
        static TValue UnboxOrDefault(winrt::Windows::Foundation::IInspectable const& value)
        {
            if constexpr (std::is_same_v<TValue, winrt::hstring>)
            {
                return winrt::unbox_value_or<winrt::hstring>(value, L"");
            }
            else
            {
                return value.try_as<TValue>();
            }
        }

        auto& Owner()
        {
            return *static_cast<typename traits_type::owner_type*>(this);
        }

        auto GetParsedValues() const
        {
            return std::make_tuple(traits_type::Parse(To()), traits_type::Parse(From()));
        }

        winrt::Windows::Foundation::TimeSpan ResolveDelay(std::optional<winrt::Windows::Foundation::TimeSpan> delayHint) const
        {
            auto delay = this->Delay();
            return delay ? delay.Value() : delayHint.value_or(Animations::AnimationExtensions::DefaultDelay());
        }

        winrt::Windows::Foundation::TimeSpan ResolveDuration(std::optional<winrt::Windows::Foundation::TimeSpan> durationHint) const
        {
            auto duration = this->Duration();
            return duration ? duration.Value() : durationHint.value_or(Animations::AnimationExtensions::DefaultDuration());
        }

        enum EasingType ResolveEasingType(std::optional<enum EasingType> easingTypeHint) const
        {
            auto easingType = this->EasingType();
            return easingType ? easingType.Value() : easingTypeHint.value_or(Animations::AnimationExtensions::DefaultEasingType());
        }

        enum EasingMode ResolveEasingMode(std::optional<enum EasingMode> easingModeHint) const
        {
            auto easingMode = this->EasingMode();
            return easingMode ? easingMode.Value() : easingModeHint.value_or(Animations::AnimationExtensions::DefaultEasingMode());
        }

        /// <summary>
        /// Appends the current keyframe values to a target <see cref="INormalizedKeyFrameAnimationBuilder{T}"/> instance.
        /// This method will also automatically generate keyframes for <see cref="To"/> and <see cref="From"/>.
        /// </summary>
        /// <param name="builder">The target <see cref="INormalizedKeyFrameAnimationBuilder{T}"/> instance to add the keyframe to.</param>
        /// <param name="easingTypeHint">A hint for the easing type, if present.</param>
        /// <param name="easingModeHint">A hint for the easing mode, if present.</param>
        /// <returns>The same <see cref="INormalizedKeyFrameAnimationBuilder{T}"/> instance as <paramref name="builder"/>.</returns>
        INormalizedKeyFrameAnimationBuilder<parsed_value_type>& AppendToBuilder(
            INormalizedKeyFrameAnimationBuilder<parsed_value_type>& builder,
            std::optional<enum EasingType> easingTypeHint,
            std::optional<enum EasingMode> easingModeHint)
        {
            for (auto const& keyFrame : Owner().KeyFrames())
            {
                auto keyFrameImpl = winrt::get_self<keyframe_impl_type>(keyFrame);
                keyFrameImpl->AppendToBuilder(builder);
            }

            auto [to, from] = GetParsedValues();

            if (to.has_value())
            {
                builder.KeyFrame(1.0, to.value(), ResolveEasingType(easingTypeHint), ResolveEasingMode(easingModeHint));
            }

            if (from.has_value())
            {
                builder.KeyFrame(0.0, from.value());
            }

            return builder;
        }
    };
}
