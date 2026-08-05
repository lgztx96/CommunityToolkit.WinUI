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
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <type_traits>
#include <concepts>
#include <wil/resource.h>
#else
import std;
import winrt.XamlToolkit.WinUI.Animations;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Media.Animation;
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Media::Animation;
	using namespace Microsoft::UI::Xaml::Hosting;
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
        virtual winrt::Timeline GetAnimation(winrt::DependencyObject const& targetHint) = 0;
    };

    /// <summary>
    /// An interface for factories of composition animations.
    /// </summary>
    class ICompositionAnimationFactory
    {
    public:
        virtual ~ICompositionAnimationFactory() = default;
        virtual CompositionAnimation GetAnimation(winrt::CompositionObject const& targetHint, winrt::CompositionObject& target) = 0;
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
            std::optional<winrt::TimeSpan> delay,
            std::optional<winrt::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            winrt::EasingMode easingMode);

        template<typename T>
        AnimationBuilder& AddXamlAnimationFactory(
            winrt::hstring const& property,
            T const& to,
            std::optional<T> from,
            std::optional<winrt::TimeSpan> delay,
            std::optional<winrt::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            winrt::EasingMode easingMode);

        AnimationBuilder& AddXamlTransformDoubleAnimationFactory(
            winrt::hstring const& property,
            double to,
            std::optional<double> from,
            std::optional<winrt::TimeSpan> delay,
            std::optional<winrt::TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            winrt::EasingMode easingMode);

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
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new anchor point animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& AnchorPoint(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new opacity animation to the current schedule.
        /// </summary>
        AnimationBuilder& Opacity(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new translation animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new composition translation animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Translation(
            winrt::float3 const& to,
            std::optional<winrt::float3> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new uniform scale animation to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale animation on a specified axis to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new composition scale animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Scale(
            winrt::float3 const& to,
            std::optional<winrt::float3> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new composition offset animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& Offset(
            winrt::float3 const& to,
            std::optional<winrt::float3> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new center point animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point animation for all axes to the current schedule.
        /// </summary>
        AnimationBuilder& CenterPoint(
            winrt::float3 const& to,
            std::optional<winrt::float3> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new rotation animation to the current schedule.
        /// </summary>
        AnimationBuilder& Rotation(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new rotation animation in degrees to the current schedule.
        /// </summary>
        AnimationBuilder& RotationInDegrees(
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new rotation axis animation to the current schedule.
        /// </summary>
        AnimationBuilder& RotationAxis(
            winrt::float3 const& to,
            std::optional<winrt::float3> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new orientation animation to the current schedule.
        /// </summary>
        AnimationBuilder& Orientation(
            winrt::quaternion const& to,
            std::optional<winrt::quaternion> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new transform animation to the current schedule.
        /// </summary>
        AnimationBuilder& Transform(
            winrt::float4x4 const& to,
            std::optional<winrt::float4x4> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new clip animation to the current schedule.
        /// </summary>
        AnimationBuilder& Clip(
            Side side,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new clip animation to the current schedule.
        /// </summary>
        AnimationBuilder& Clip(
            winrt::Thickness const& to,
            std::optional<winrt::Thickness> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut);

        /// <summary>
        /// Adds a new size animation for a single axis to the current schedule.
        /// </summary>
        AnimationBuilder& Size(
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new size animation for the X and Y axes to the current schedule.
        /// </summary>
        AnimationBuilder& Size(
            winrt::float2 const& to,
            std::optional<winrt::float2> from = std::nullopt,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = EasingType::Default,
            winrt::EasingMode easingMode = winrt::EasingMode::EaseInOut,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new anchor point property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> AnchorPoint(Axis axis);

        /// <summary>
        /// Adds a new anchor point property animation builder for the X and Y axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::float2> AnchorPoint();

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
        PropertyAnimationBuilder<winrt::float3> Translation();

        /// <summary>
        /// Adds a new offset property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Offset(Axis axis);

        /// <summary>
        /// Adds a new offset property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::float3> Offset();

        /// <summary>
        /// Adds a new scale property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> Scale(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new scale property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::float3> Scale();

        /// <summary>
        /// Adds a new center point property animation builder for a single axis.
        /// </summary>
        PropertyAnimationBuilder<double> CenterPoint(Axis axis, FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new center point property animation builder for all axes.
        /// </summary>
        PropertyAnimationBuilder<winrt::float3> CenterPoint();

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
        PropertyAnimationBuilder<winrt::float3> RotationAxis();

        /// <summary>
        /// Adds a new orientation property animation builder.
        /// </summary>
        PropertyAnimationBuilder<winrt::quaternion> Orientation();

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
        PropertyAnimationBuilder<winrt::float2> Size();
        /// <summary>
        /// Adds a custom animation based on normalized keyframes to the current schedule.
        /// </summary>
        template<typename T>
        AnimationBuilder& NormalizedKeyFrames(
            winrt::hstring const& property,
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<winrt::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<winrt::AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a custom animation based on timed keyframes to the current schedule.
        /// </summary>
        template<typename T>
        AnimationBuilder& TimedKeyFrames(
            winrt::hstring const& property,
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            std::optional<winrt::AnimationDelayBehavior> delayBehavior = std::nullopt,
            FrameworkLayer layer = FrameworkLayer::Composition);

        /// <summary>
        /// Adds a new external composition animation to the current schedule.
        /// </summary>
        AnimationBuilder& ExternalAnimation(winrt::CompositionAnimation const& animation);

        /// <summary>
        /// Adds a new external composition animation to the current schedule for a given target.
        /// </summary>
        AnimationBuilder& ExternalAnimation(winrt::CompositionObject const& target, winrt::CompositionAnimation const& animation);

        /// <summary>
        /// Adds a new external XAML animation to the current schedule.
        /// </summary>
        AnimationBuilder& ExternalAnimation(winrt::Timeline const& animation);

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        void Start(winrt::UIElement const& element)
        {
            if (!compositionAnimationFactories.empty())
            {
                winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                winrt::Visual visual = winrt::ElementCompositionPreview::GetElementVisual(element);

                for (const auto& factory : compositionAnimationFactories)
                {
                    winrt::CompositionObject target{ nullptr };
                    auto animation = factory->GetAnimation(visual, target);

                    if (target == nullptr)
                    {
                        visual.StartAnimation(animation.Target(), animation);
                    }
                    else
                    {
                        target.StartAnimation(animation.Target(), animation);
                    }
                }
            }

            if (!xamlAnimationFactories.empty())
            {
                winrt::Storyboard storyboard;
                auto children = storyboard.Children();
                for (const auto& factory : xamlAnimationFactories)
                {
                    children.Append(factory->GetAnimation(element));
                }

                storyboard.Begin();
            }
        }

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        void Start(winrt::UIElement const& element, std::function<void()> callback)
        {
            // The point of this overload is to allow consumers to invoke a callback when an animation
            // completes, without having to create an async state machine. There are three different possible
            // scenarios to handle, and each can have a specialized code path to ensure the implementation
            // is as lean and efficient as possible. Specifically, for a given AnimationBuilder instance:
            //   1) There are only Composition animations
            //   2) There are only XAML animations
            //   3) There are both Composition and XAML animations
            // The implementation details of each of these paths is described below.
            if (!compositionAnimationFactories.empty())
            {
                if (xamlAnimationFactories.empty())
                {
                    // Only Composition animations
                    winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                    winrt::Visual visual = winrt::ElementCompositionPreview::GetElementVisual(element);
                    winrt::CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

                    batch.Completed([callback](auto&&, auto&&) { callback(); });

                    for (const auto& factory : compositionAnimationFactories)
                    {
                        winrt::CompositionObject target{ nullptr };
                        auto animation = factory->GetAnimation(visual, target);

                        if (target == nullptr)
                        {
                            visual.StartAnimation(animation.Target(), animation);
                        }
                        else
                        {
                            target.StartAnimation(animation.Target(), animation);
                        }
                    }

                    batch.End();
                }
                else
                {
                    // In this case we need to wait for both the Composition and XAML animation groups to complete. These two
                    // groups use different APIs and can have a different duration, so we need to synchronize between them
                    // without creating an async state machine (as that'd defeat the point of this separate overload).
                    //
                    // The code below relies on a mutable boxed counter that's shared across the two closures for the Completed
                    // events for both the Composition scoped batch and the XAML Storyboard. The counter is initialized to 2, and
                    // when each group completes, the counter is decremented (we don't need an interlocked decrement as the delegates
                    // will already be invoked on the current DispatcherQueue instance, which acts as the synchronization context here.
                    // The handlers for the Composition batch and the Storyboard will never execute concurrently). If the counter has
                    // reached zero, it means that both groups have completed, so the user-provided callback is triggered, otherwise
                    // the handler just does nothing. This ensures that the callback is executed exactly once when all the animation
                    // complete, but without the need to create TaskCompletionSource-s and an async state machine to await for that.
                    //
                    // Note: we're using StrongBox<T> here because that exposes a mutable field of the type we need (int).
                    // We can't just mutate a boxed int in-place with Unsafe.Unbox<T> as that's against the ECMA spec, since
                    // that API uses the unbox IL opcode (§III.4.32) which returns a "controlled-mutability managed pointer"
                    // (§III.1.8.1.2.2), which is not "verifier-assignable-to" (ie. directly assigning to it is not legal).
                    auto counter = std::make_shared<int>(2);

                    winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                    winrt::Visual visual = winrt::ElementCompositionPreview::GetElementVisual(element);
                    winrt::CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

                    batch.Completed([counter, callback](auto&&, auto&&)
                    {
                        if (--(*counter) == 0)
                        {
                            callback();
                        }
                    });

                    for (const auto& factory : compositionAnimationFactories)
                    {
                        winrt::CompositionObject target{ nullptr };
                        auto animation = factory->GetAnimation(visual, target);

                        if (target == nullptr)
                        {
                            visual.StartAnimation(animation.Target(), animation);
                        }
                        else
                        {
                            target.StartAnimation(animation.Target(), animation);
                        }
                    }

                    batch.End();

                    winrt::Storyboard storyboard;
                    auto children = storyboard.Children();
                    for (const auto& factory : xamlAnimationFactories)
                    {
                        children.Append(factory->GetAnimation(element));
                    }

                    storyboard.Completed([counter, callback](auto&&, auto&&) 
                    {
                        if (--(*counter) == 0)
                        {
                            callback();
                        }
                    });
                    storyboard.Begin();
                }
            }
            else
            {
                // There are only XAML animations. This case is extremely similar to that where we only have Composition
                // animations, with the main difference being that the Completed event is directly exposed from the
                // Storyboard type, so we don't need a separate type to track the animation completion. The same
                // considerations regarding the closure to capture the provided callback apply here as well.
                winrt::Storyboard storyboard;
                auto children = storyboard.Children();
                for (const auto& factory : xamlAnimationFactories)
                {
                    children.Append(factory->GetAnimation(element));
                }

                storyboard.Completed([callback](auto&&, auto&&) { callback(); });
                storyboard.Begin();
            }
        }

        /// <summary>
        /// Starts the animations present in the current AnimationBuilder instance.
        /// </summary>
        winrt::IAsyncAction StartAsync(winrt::UIElement const& element)
        {
            winrt::IAsyncAction compositionTask{ nullptr };
            winrt::IAsyncAction xamlTask{ nullptr };
            auto cancelation_token{ co_await winrt::get_cancellation_token() };

            std::vector<std::tuple<winrt::CompositionObject, winrt::hstring>> compositionAnimations;

            if (!compositionAnimationFactories.empty())
            {
                wil::shared_event completionEvent(wil::EventOptions::ManualReset);

                winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                winrt::Visual visual = winrt::ElementCompositionPreview::GetElementVisual(element);
                winrt::CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

                batch.Completed([completionEvent](auto&&, auto&&)
                {
                    completionEvent.SetEvent();
                });

                for (const auto& factory : compositionAnimationFactories)
                {
                    winrt::CompositionObject target{ nullptr };
                    auto animation = factory->GetAnimation(visual, target);

                    if (target == nullptr)
                    {
                        visual.StartAnimation(animation.Target(), animation);
                    }
                    else
                    {
                        target.StartAnimation(animation.Target(), animation);
                    }

                    compositionAnimations.emplace_back(target ? target : visual, animation.Target());
                }

                batch.End();

                compositionTask = [completionEvent]() -> winrt::IAsyncAction
                {
                    co_await winrt::resume_on_signal(completionEvent.get());
                }();
            }

            winrt::Storyboard storyboard{ nullptr };

            if (!xamlAnimationFactories.empty())
            {
                storyboard = winrt::Storyboard();
                wil::shared_event completionEvent(wil::EventOptions::ManualReset);

                auto children = storyboard.Children();
                for (const auto& factory : xamlAnimationFactories)
                {
                    children.Append(factory->GetAnimation(element));
                }

                storyboard.Completed([completionEvent](auto&&, auto&&)
                {
                     completionEvent.SetEvent();
                });

                storyboard.Begin();

                xamlTask = [completionEvent]() -> winrt::IAsyncAction
                {
                     co_await winrt::resume_on_signal(completionEvent.get());
                }();
            }

            cancelation_token.callback([=]
            {
                for (const auto& [target, path] : compositionAnimations)
                {
                    target.StopAnimation(path);
                }

                if (storyboard) storyboard.Stop();
            });

            if (compositionTask) co_await compositionTask;
            if (xamlTask) co_await xamlTask;
        }

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
        AnimationBuilder& AddCompositionAnimationFactory(T&& factory, winrt::CompositionObject const&)
        {
            return AddCompositionAnimationFactory(std::forward<T>(factory));
        }

        AnimationBuilder& AddCompositionAnimationFactory(winrt::CompositionAnimation const& animation, winrt::CompositionObject const& target);

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

        AnimationBuilder& AddXamlAnimationFactory(winrt::Timeline const& animation);
    };
}

#include "AnimationBuilder.Default.h"
#include "AnimationBuilder.External.h"
#include "AnimationBuilder.Factories.h"
#include "AnimationBuilder.KeyFrames.h"
#include "AnimationBuilder.PropertyBuilders.h"
#include "AnimationBuilder.Setup.h"