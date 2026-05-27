#pragma once

#include "AnimationBuilder.h"
#include "AnimationBuilder.KeyFrames.h"
#include "Interfaces/IPropertyAnimationBuilder{T}.h"
#include "../Enums/Axis.h"
#include "../Enums/Side.h"

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Media::Animation;
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    template<typename TFactory>
    class CompositionClipAnimationFactory final : public ICompositionAnimationFactory
    {
    private:
        TFactory factory;

    public:
        explicit CompositionClipAnimationFactory(TFactory const& factory)
            : factory(factory)
        {
        }

        CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
        {
            auto visual = targetHint.try_as<Visual>();

            if (!visual)
            {
                throw winrt::hresult_invalid_argument(L"Clip property animations require a Visual target.");
            }

            auto clip = visual.Clip().try_as<InsetClip>();

            if (!clip)
            {
                clip = visual.Compositor().CreateInsetClip();
                visual.Clip(clip);
            }

            CompositionObject ignored{ nullptr };
            auto animation = factory.GetAnimation(clip, ignored);

            target = clip;

            return animation;
        }
    };

    template<typename TFactory>
    class XamlTransformAnimationFactory final : public IXamlAnimationFactory
    {
    private:
        TFactory factory;

    public:
        explicit XamlTransformAnimationFactory(TFactory const& factory)
            : factory(factory)
        {
        }

        Timeline GetAnimation(DependencyObject const& targetHint) override
        {
            auto element = targetHint.try_as<UIElement>();

            if (!element)
            {
                throw winrt::hresult_invalid_argument(L"XAML transform property animations require a UIElement target.");
            }

            auto transform = element.RenderTransform().try_as<CompositeTransform>();

            if (!transform)
            {
                transform = CompositeTransform();
                element.RenderTransform(transform);
            }

            return factory.GetAnimation(transform);
        }
    };

    enum class PropertyAnimationBuilderTarget
    {
        Default,
        CompositionClip,
        XamlTransform,
    };

    /// <summary>
    /// A custom <see cref="IPropertyAnimationBuilder{T}"/> for a shared animation.
    /// </summary>
    template<typename T>
    class PropertyAnimationBuilder : public IPropertyAnimationBuilder<T>
    {
    private:
        AnimationBuilder& builder;
        winrt::hstring property;
        FrameworkLayer layer;
        PropertyAnimationBuilderTarget targetKind;

        PropertyAnimationBuilder(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            FrameworkLayer layer,
            PropertyAnimationBuilderTarget targetKind)
            : builder(builder)
            , property(property)
            , layer(layer)
            , targetKind(targetKind)
        {
        }

    public:
        PropertyAnimationBuilder(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            FrameworkLayer layer)
            : PropertyAnimationBuilder(builder, property, layer, PropertyAnimationBuilderTarget::Default)
        {
        }

        static PropertyAnimationBuilder CreateCompositionClip(
            AnimationBuilder& builder,
            winrt::hstring const& property)
        {
            return PropertyAnimationBuilder(builder, property, FrameworkLayer::Composition, PropertyAnimationBuilderTarget::CompositionClip);
        }

        static PropertyAnimationBuilder CreateXamlTransform(
            AnimationBuilder& builder,
            winrt::hstring const& property)
        {
            return PropertyAnimationBuilder(builder, property, FrameworkLayer::Xaml, PropertyAnimationBuilderTarget::XamlTransform);
        }

        /// <inheritdoc/>
        AnimationBuilder& NormalizedKeyFrames(
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) override
        {
            if (targetKind == PropertyAnimationBuilderTarget::Default)
            {
                return builder.NormalizedKeyFrames<T>(property, std::move(build), delay, duration, repeatOption, delayBehavior, layer);
            }

            if (targetKind == PropertyAnimationBuilderTarget::CompositionClip)
            {
                auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    duration.value_or(AnimationExtensions::DefaultDuration()),
                    repeatOption.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);
                builder.AddCompositionAnimationFactory(CompositionClipAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

                return builder;
            }

            auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderXaml<T>(
                property,
                delay,
                duration.value_or(AnimationExtensions::DefaultDuration()),
                repeatOption.value_or(RepeatOptionHelper::Once()));

            build(keyFrameBuilder);
            builder.AddXamlAnimationFactory(XamlTransformAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

            return builder;
        }

        /// <inheritdoc/>
        AnimationBuilder& TimedKeyFrames(
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) override
        {
            if (targetKind == PropertyAnimationBuilderTarget::Default)
            {
                return builder.TimedKeyFrames<T>(property, std::move(build), delay, repeatOption, delayBehavior, layer);
            }

            if (targetKind == PropertyAnimationBuilderTarget::CompositionClip)
            {
                auto keyFrameBuilder = TimedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    repeatOption.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);
                builder.AddCompositionAnimationFactory(CompositionClipAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

                return builder;
            }

            auto keyFrameBuilder = TimedKeyFrameAnimationBuilderXaml<T>(
                property,
                delay,
                repeatOption.value_or(RepeatOptionHelper::Once()));

            build(keyFrameBuilder);
            builder.AddXamlAnimationFactory(XamlTransformAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

            return builder;
        }
    };

    /// <summary>
    /// Extension methods for AnimationBuilder to create property animation builders.
    /// </summary>
    class AnimationBuilderPropertyBuilders
    {
    public:
        /// <summary>
        /// Adds a new anchor point animation for a single axis to the current schedule.
        /// </summary>
        /// <param name="builder">The AnimationBuilder instance.</param>
        /// <param name="axis">The target anchor point axis to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<double> AnchorPoint(AnimationBuilder& builder, Axis axis)
        {
            return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::AnchorPoint(axis), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new anchor point animation for the X and Y axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float2> AnchorPoint(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, L"AnchorPoint", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new opacity animation to the current schedule.
        /// </summary>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> Opacity(AnimationBuilder& builder, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return PropertyAnimationBuilder<double>(builder, L"Opacity", layer);
        }

        /// <summary>
        /// Adds a new translation animation for a single axis to the current schedule.
        /// </summary>
        /// <param name="axis">The target translation axis to animate.</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> Translation(AnimationBuilder& builder, Axis axis, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::Translation(axis), layer);
            }

            return PropertyAnimationBuilder<double>::CreateXamlTransform(builder, AnimationExtensions::Properties::Xaml::Translation(axis));
        }

        /// <summary>
        /// Adds a new composition translation animation for all axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float3> Translation(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float3>(builder, L"Translation", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition translation animation for the X and Y axes to the current schedule.
        /// </summary>
        static PropertyAnimationBuilder<float2> TranslationXY(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, AnimationExtensions::Properties::Composition::TranslationXY(), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition offset animation for a single axis to the current schedule.
        /// </summary>
        /// <param name="axis">The target translation axis to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<double> Offset(AnimationBuilder& builder, Axis axis)
        {
            return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::Offset(axis), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition offset translation animation for all axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float3> Offset(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float3>(builder, L"Offset", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition offset animation for the X and Y axes to the current schedule.
        /// </summary>
        static PropertyAnimationBuilder<float2> OffsetXY(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, AnimationExtensions::Properties::Composition::OffsetXY(), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new scale animation on a specified axis to the current schedule.
        /// </summary>
        /// <param name="axis">The target scale axis to animate.</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> Scale(AnimationBuilder& builder, Axis axis, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::Scale(axis), layer);
            }

            return PropertyAnimationBuilder<double>::CreateXamlTransform(builder, AnimationExtensions::Properties::Xaml::Scale(axis));
        }

        /// <summary>
        /// Adds a new scale animation for all axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float3> Scale(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float3>(builder, L"Scale", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition scale animation for the X and Y axes to the current schedule.
        /// </summary>
        static PropertyAnimationBuilder<float2> ScaleXY(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, AnimationExtensions::Properties::Composition::ScaleXY(), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new center point animation on a specified axis to the current schedule.
        /// </summary>
        /// <param name="axis">The target scale axis to animate.</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> CenterPoint(AnimationBuilder& builder, Axis axis, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::CenterPoint(axis), layer);
            }

            return PropertyAnimationBuilder<double>::CreateXamlTransform(builder, AnimationExtensions::Properties::Xaml::CenterPoint(axis));
        }

        /// <summary>
        /// Adds a new center point animation for all axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float3> CenterPoint(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float3>(builder, L"CenterPoint", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new composition center point animation for the X and Y axes to the current schedule.
        /// </summary>
        static PropertyAnimationBuilder<float2> CenterPointXY(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, AnimationExtensions::Properties::Composition::CenterPointXY(), FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new rotation animation to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<double> Rotation(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<double>(builder, L"RotationAngle", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new rotation animation in degrees to the current schedule.
        /// </summary>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> RotationInDegrees(AnimationBuilder& builder, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return PropertyAnimationBuilder<double>(builder, L"RotationAngleInDegrees", layer);
            }

            return PropertyAnimationBuilder<double>::CreateXamlTransform(builder, L"Rotation");
        }

        /// <summary>
        /// Adds a new rotation axis animation to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float3> RotationAxis(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float3>(builder, L"RotationAxis", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new orientation animation to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<quaternion> Orientation(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<quaternion>(builder, L"Orientation", FrameworkLayer::Composition);
        }

        /// <summary>
        /// Adds a new clip animation to the current schedule.
        /// </summary>
        /// <param name="side">The clip size to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<double> Clip(AnimationBuilder& builder, Side side)
        {
            return PropertyAnimationBuilder<double>::CreateCompositionClip(builder, AnimationExtensions::Properties::Composition::Clip(side));
        }

        /// <summary>
        /// Adds a new size animation for a single axis to the current schedule.
        /// </summary>
        /// <param name="axis">The target size axis to animate.</param>
        /// <param name="layer">The target framework layer to animate.</param>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        static PropertyAnimationBuilder<double> Size(AnimationBuilder& builder, Axis axis, FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Composition::Size(axis), layer);
            }

            return PropertyAnimationBuilder<double>(builder, AnimationExtensions::Properties::Xaml::Size(axis), layer);
        }

        /// <summary>
        /// Adds a new composition size translation animation for all axes to the current schedule.
        /// </summary>
        /// <returns>An <see cref="IPropertyAnimationBuilder{T}"/> instance to configure the animation.</returns>
        /// <remarks>This animation is only available on the composition layer.</remarks>
        static PropertyAnimationBuilder<float2> Size(AnimationBuilder& builder)
        {
            return PropertyAnimationBuilder<float2>(builder, AnimationExtensions::Properties::Composition::Size(), FrameworkLayer::Composition);
        }
    };
}
