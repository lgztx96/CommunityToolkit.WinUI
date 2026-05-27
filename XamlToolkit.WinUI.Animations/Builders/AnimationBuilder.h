// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../Enums/Side.h"
#include "../Enums/Axis.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <type_traits>
#include <concepts>
#endif

import winrt.XamlToolkit.WinUI.Animations;

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An interface for factories of XAML animations.
    /// </summary>
    class IXamlAnimationFactory
    {
    public:
        virtual ~IXamlAnimationFactory() = default;
        virtual Timeline GetAnimation(DependencyObject const& targetHint) = 0;
    };

    /// <summary>
    /// An interface for factories of composition animations.
    /// </summary>
    class ICompositionAnimationFactory
    {
    public:
        virtual ~ICompositionAnimationFactory() = default;
        virtual CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) = 0;
    };

    template<typename T>
    class PropertyAnimationBuilder;

    template<typename T>
    class INormalizedKeyFrameAnimationBuilder;

    template<typename T>
    class ITimedKeyFrameAnimationBuilder;

    /// <summary>
    /// A class that allows to build custom animations targeting both the XAML and composition layers.
    /// </summary>
    class AnimationBuilder
    {
    private:
        std::vector<std::unique_ptr<ICompositionAnimationFactory>> compositionAnimationFactories;
        std::vector<std::unique_ptr<IXamlAnimationFactory>> xamlAnimationFactories;

        AnimationBuilder() = default;

        template<typename T>
        AnimationBuilder& AddCompositionAnimationFactory(
            winrt::hstring const& property,
            T const& to,
            std::optional<T> from,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            EasingMode easingMode);

        template<typename T>
        AnimationBuilder& AddXamlAnimationFactory(
            winrt::hstring const& property,
            T const& to,
            std::optional<T> from,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            EasingMode easingMode);

        AnimationBuilder& AddCompositionClipAnimationFactory(
            winrt::hstring const& property,
            float to,
            std::optional<float> from,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            EasingMode easingMode);

        AnimationBuilder& AddXamlTransformDoubleAnimationFactory(
            winrt::hstring const& property,
            double to,
            std::optional<double> from,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            EasingMode easingMode);

    public:
        /// <summary>
        /// Creates a new AnimationBuilder instance to setup an animation schedule.
        /// </summary>
        static AnimationBuilder Create() { return AnimationBuilder{}; }

        /// <summary>
        /// Adds a new anchor point animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& AnchorPoint(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new anchor point animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& AnchorPoint(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new opacity animation to the current schedule.
        /// </summary>
        AnimationBuilder& Opacity(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new composition translation animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            winrt::Windows::Foundation::Numerics::float3 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float3> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new uniform scale animation to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale animation on a specified axis to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new composition scale animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            winrt::Windows::Foundation::Numerics::float3 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float3> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            winrt::Windows::Foundation::Numerics::float3 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float3> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new center point animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            winrt::Windows::Foundation::Numerics::float3 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float3> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new rotation animation to the current schedule.
        /// </summary>
        AnimationBuilder& Rotation(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new rotation animation in degrees to the current schedule.
        /// </summary>
        AnimationBuilder& RotationInDegrees(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new rotation axis animation to the current schedule.
        /// </summary>
        AnimationBuilder& RotationAxis(
            winrt::Windows::Foundation::Numerics::float3 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float3> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new orientation animation to the current schedule.
        /// </summary>
        AnimationBuilder& Orientation(
            winrt::Windows::Foundation::Numerics::quaternion const& to,
            std::optional<winrt::Windows::Foundation::Numerics::quaternion> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new transform animation to the current schedule.
        /// </summary>
        AnimationBuilder& Transform(
            winrt::Windows::Foundation::Numerics::float4x4 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float4x4> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new clip animation to the current schedule.
        /// </summary>
        AnimationBuilder& Clip(
            Side side,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new clip animation to the current schedule.
        /// </summary>
        AnimationBuilder& Clip(
            winrt::Microsoft::UI::Xaml::Thickness const& to,
            std::optional<winrt::Microsoft::UI::Xaml::Thickness> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new size animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Size(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new size animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Size(
            winrt::Windows::Foundation::Numerics::float2 const& to,
            std::optional<winrt::Windows::Foundation::Numerics::float2> from = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            EasingMode easingMode = EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new anchor point property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> AnchorPoint(Axis axis);

        /// <summary>
        /// Adds a new anchor point property animation builder for the X and Y axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float2> AnchorPoint();

        /// <summary>
        /// Adds a new opacity property animation builder.
        /// </summary>
        PropertyAnimationBuilder<double> Opacity(FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Translation(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float3> Translation();

        /// <summary>
        /// Adds a new offset property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Offset(Axis axis);

        /// <summary>
        /// Adds a new offset property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float3> Offset();

        /// <summary>
        /// Adds a new scale property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Scale(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float3> Scale();

        /// <summary>
        /// Adds a new center point property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> CenterPoint(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float3> CenterPoint();

        /// <summary>
        /// Adds a new rotation property animation builder.
        /// </summary>
        PropertyAnimationBuilder<double> Rotation();

        /// <summary>
        /// Adds a new rotation-in-degrees property animation builder.
        /// </summary>
        PropertyAnimationBuilder<double> RotationInDegrees(FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new rotation axis property animation builder.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float3> RotationAxis();

        /// <summary>
        /// Adds a new orientation property animation builder.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::quaternion> Orientation();

        /// <summary>
        /// Adds a new clip property animation builder.
        /// </summary>
        PropertyAnimationBuilder<double> Clip(Side side);

        /// <summary>
        /// Adds a new size property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Size(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new size property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::Windows::Foundation::Numerics::float2> Size();
        /// <summary>
        /// Adds a custom animation based on normalized keyframes to the current schedule.
        /// </summary>
        template<typename T>
        AnimationBuilder& NormalizedKeyFrames(
            winrt::hstring const& property,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a custom animation based on normalized keyframes to the current schedule.
        /// </summary>
        template<typename T, typename TState>
        AnimationBuilder& NormalizedKeyFrames(
            winrt::hstring const& property,
            TState state,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        template<typename T>
        AnimationBuilder& TimedKeyFrames(
            winrt::hstring const& property,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        template<typename T, typename TState>
        AnimationBuilder& TimedKeyFrames(
            winrt::hstring const& property,
            TState state,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&, TState)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new external composition animation to the current schedule.
        /// </summary>
        AnimationBuilder& ExternalAnimation(CompositionAnimation const& animation);

        /// <summary>
        /// Adds a new external composition animation to the current schedule for a given target.
        /// </summary>
        AnimationBuilder& ExternalAnimation(CompositionObject const& target, CompositionAnimation const& animation);

        /// <summary>
        /// Adds a new external XAML animation to the current schedule.
        /// </summary>
        AnimationBuilder& ExternalAnimation(Timeline const& animation);

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        void Start(UIElement const& element);

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        void Start(UIElement const& element, std::function<void()> callback);

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        winrt::Windows::Foundation::IAsyncAction StartAsync(UIElement const& element);

        /// <summary>
        /// Adds a composition animation factory.
        /// </summary>
        AnimationBuilder& AddCompositionAnimationFactory(std::unique_ptr<ICompositionAnimationFactory> factory)
        {
            compositionAnimationFactories.push_back(std::move(factory));
            return *this;
        }

        template<typename T>
            requires std::is_base_of_v<ICompositionAnimationFactory, std::remove_cvref_t<T>>
        AnimationBuilder& AddCompositionAnimationFactory(T&& factory)
        {
            using FactoryType = std::remove_cvref_t<T>;
            return AddCompositionAnimationFactory(std::make_unique<FactoryType>(std::forward<T>(factory)));
        }

        template<typename T>
            requires std::is_base_of_v<ICompositionAnimationFactory, std::remove_cvref_t<T>>
        AnimationBuilder& AddCompositionAnimationFactory(T&& factory, CompositionObject const&)
        {
            return AddCompositionAnimationFactory(std::forward<T>(factory));
        }

        AnimationBuilder& AddCompositionAnimationFactory(CompositionAnimation const& animation, CompositionObject const& target);

        /// <summary>
        /// Adds a XAML animation factory.
        /// </summary>
        AnimationBuilder& AddXamlAnimationFactory(std::unique_ptr<IXamlAnimationFactory> factory)
        {
            xamlAnimationFactories.push_back(std::move(factory));
            return *this;
        }

        template<typename T>
            requires std::is_base_of_v<IXamlAnimationFactory, std::remove_cvref_t<T>>
        AnimationBuilder& AddXamlAnimationFactory(T&& factory)
        {
            using FactoryType = std::remove_cvref_t<T>;
            return AddXamlAnimationFactory(std::make_unique<FactoryType>(std::forward<T>(factory)));
        }

        AnimationBuilder& AddXamlAnimationFactory(Timeline const& animation);
    };
}

#include "AnimationBuilder.KeyFrames.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    template<typename T>
    AnimationBuilder& AnimationBuilder::NormalizedKeyFrames(
        winrt::hstring const& property,
        std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeatOption,
        std::optional<AnimationDelayBehavior> delayBehavior,
        FrameworkLayer layer)
    {
        return AnimationBuilderKeyFrames::NormalizedKeyFrames<T>(*this, property, std::move(build), delay, duration, repeatOption, delayBehavior, layer);
    }

    template<typename T, typename TState>
    AnimationBuilder& AnimationBuilder::NormalizedKeyFrames(
        winrt::hstring const& property,
        TState state,
        std::function<void(INormalizedKeyFrameAnimationBuilder<T>&, TState)> build,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeatOption,
        std::optional<AnimationDelayBehavior> delayBehavior,
        FrameworkLayer layer)
    {
        return AnimationBuilderKeyFrames::NormalizedKeyFrames<T, TState>(*this, property, state, std::move(build), delay, duration, repeatOption, delayBehavior, layer);
    }

    template<typename T>
    AnimationBuilder& AnimationBuilder::TimedKeyFrames(
        winrt::hstring const& property,
        std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<RepeatOption> repeat,
        std::optional<AnimationDelayBehavior> delayBehavior,
        FrameworkLayer layer)
    {
        return AnimationBuilderKeyFrames::TimedKeyFrames<T>(*this, property, std::move(build), delay, repeat, delayBehavior, layer);
    }

    template<typename T, typename TState>
    AnimationBuilder& AnimationBuilder::TimedKeyFrames(
        winrt::hstring const& property,
        TState state,
        std::function<void(ITimedKeyFrameAnimationBuilder<T>&, TState)> build,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<RepeatOption> repeat,
        std::optional<AnimationDelayBehavior> delayBehavior,
        FrameworkLayer layer)
    {
        return AnimationBuilderKeyFrames::TimedKeyFrames<T, TState>(*this, property, state, std::move(build), delay, repeat, delayBehavior, layer);
    }
}
