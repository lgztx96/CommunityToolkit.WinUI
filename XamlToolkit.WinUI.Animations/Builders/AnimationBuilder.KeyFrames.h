#pragma once

#include "AnimationBuilder.h"
#include "../Extensions/AnimationExtensions.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.Composition.h"
#include "TimedKeyFrameAnimationBuilder{T}.Composition.h"
#include "TimedKeyFrameAnimationBuilder{T}.Xaml.h"
#include "Interfaces/INormalizedKeyFrameAnimationBuilder{T}.h"
#include "Interfaces/ITimedKeyFrameAnimationBuilder{T}.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <functional>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
	using namespace winrt::Microsoft::UI::Composition;
	using namespace winrt::Microsoft::UI::Xaml::Media::Animation;

	inline PropertyAnimationBuilder<double> AnimationBuilder::AnchorPoint(Axis axis)
	{
		return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::AnchorPoint(axis), FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<float2> AnimationBuilder::AnchorPoint()
	{
		return PropertyAnimationBuilder<float2>(*this, L"AnchorPoint", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Opacity(FrameworkLayer layer)
	{
		return PropertyAnimationBuilder<double>(*this, L"Opacity", layer);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Translation(Axis axis, FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::Translation(axis), layer);
		}

		return PropertyAnimationBuilder<double>::CreateXamlTransform(*this, AnimationExtensions::Properties::Xaml::Translation(axis));
	}

	inline PropertyAnimationBuilder<float3> AnimationBuilder::Translation()
	{
		return PropertyAnimationBuilder<float3>(*this, L"Translation", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Offset(Axis axis)
	{
		return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::Offset(axis), FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<float3> AnimationBuilder::Offset()
	{
		return PropertyAnimationBuilder<float3>(*this, L"Offset", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Scale(Axis axis, FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::Scale(axis), layer);
		}

		return PropertyAnimationBuilder<double>::CreateXamlTransform(*this, AnimationExtensions::Properties::Xaml::Scale(axis));
	}

	inline PropertyAnimationBuilder<float3> AnimationBuilder::Scale()
	{
		return PropertyAnimationBuilder<float3>(*this, L"Scale", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::CenterPoint(Axis axis, FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::CenterPoint(axis), layer);
		}

		return PropertyAnimationBuilder<double>::CreateXamlTransform(*this, AnimationExtensions::Properties::Xaml::CenterPoint(axis));
	}

	inline PropertyAnimationBuilder<float3> AnimationBuilder::CenterPoint()
	{
		return PropertyAnimationBuilder<float3>(*this, L"CenterPoint", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Rotation()
	{
		return PropertyAnimationBuilder<double>(*this, L"RotationAngle", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::RotationInDegrees(FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			return PropertyAnimationBuilder<double>(*this, L"RotationAngleInDegrees", layer);
		}

		return PropertyAnimationBuilder<double>::CreateXamlTransform(*this, L"Rotation");
	}

	inline PropertyAnimationBuilder<float3> AnimationBuilder::RotationAxis()
	{
		return PropertyAnimationBuilder<float3>(*this, L"RotationAxis", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<quaternion> AnimationBuilder::Orientation()
	{
		return PropertyAnimationBuilder<quaternion>(*this, L"Orientation", FrameworkLayer::Composition);
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Clip(Side side)
	{
		return PropertyAnimationBuilder<double>::CreateCompositionClip(*this, AnimationExtensions::Properties::Composition::Clip(side));
	}

	inline PropertyAnimationBuilder<double> AnimationBuilder::Size(Axis axis, FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Composition::Size(axis), layer);
		}

		return PropertyAnimationBuilder<double>(*this, AnimationExtensions::Properties::Xaml::Size(axis), layer);
	}

	inline PropertyAnimationBuilder<float2> AnimationBuilder::Size()
	{
		return PropertyAnimationBuilder<float2>(*this, AnimationExtensions::Properties::Composition::Size(), FrameworkLayer::Composition);
	}

	/// <summary>
	/// Adds a custom animation based on normalized keyframes to the current schedule.
	/// </summary>
	/// <typeparam name="T">The type of values to animate.</typeparam>
	/// <param name="builder">The AnimationBuilder instance.</param>
	/// <param name="property">The target property to animate.</param>
	/// <param name="build">The callback to use to construct the custom animation.</param>
	/// <param name="delay">The optional initial delay for the animation.</param>
	/// <param name="duration">The animation duration.</param>
	/// <param name="repeatOption">The repeat option for the animation (defaults to one iteration).</param>
	/// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
	/// <param name="layer">The target framework layer to animate.</param>
	/// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
	template<typename T>
	inline AnimationBuilder& AnimationBuilder::NormalizedKeyFrames(
		winrt::hstring const& property,
		std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
		std::optional<winrt::Windows::Foundation::TimeSpan> delay,
		std::optional<winrt::Windows::Foundation::TimeSpan> duration,
		std::optional<RepeatOption> repeatOption,
		std::optional<AnimationDelayBehavior> delayBehavior,
		FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			auto builder = NormalizedKeyFrameAnimationBuilderComposition<T>(
				property,
				delay,
				duration.value_or(AnimationExtensions::DefaultDuration()),
				repeatOption.value_or(RepeatOptionHelper::Once()),
				delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

			build(builder);

			AddCompositionAnimationFactory(builder);
		}
		else
		{
			auto builder = NormalizedKeyFrameAnimationBuilderXaml<T>(
				property,
				delay,
				duration.value_or(AnimationExtensions::DefaultDuration()),
				repeatOption.value_or(RepeatOptionHelper::Once()));

			build(builder);
			AddXamlAnimationFactory(builder);
		}

		return *this;
	}

	/// <summary>
	/// Adds a custom animation based on normalized keyframes to the current schedule.
	/// </summary>
	/// <typeparam name="T">The type of values to animate.</typeparam>
	/// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
	/// <param name="builder">The AnimationBuilder instance.</param>
	/// <param name="property">The target property to animate.</param>
	/// <param name="state">The state to pass to the builder.</param>
	/// <param name="build">The callback to use to construct the custom animation.</param>
	/// <param name="delay">The optional initial delay for the animation.</param>
	/// <param name="duration">The animation duration.</param>
	/// <param name="repeatOption">The repeat option for the animation (defaults to one iteration).</param>
	/// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
	/// <param name="layer">The target framework layer to animate.</param>
	/// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
	template<typename T, typename TState>
	inline AnimationBuilder& AnimationBuilder::NormalizedKeyFrames(
		winrt::hstring const& property,
		TState state,
		std::function<void(INormalizedKeyFrameAnimationBuilder<T>&, TState)> build,
		std::optional<winrt::Windows::Foundation::TimeSpan> delay,
		std::optional<winrt::Windows::Foundation::TimeSpan> duration,
		std::optional<RepeatOption> repeatOption,
		std::optional<AnimationDelayBehavior> delayBehavior,
		FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			auto builder = NormalizedKeyFrameAnimationBuilderComposition<T>(
				property,
				delay,
				duration.value_or(AnimationExtensions::DefaultDuration()),
				repeatOption.value_or(RepeatOptionHelper::Once()),
				delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

			build(builder, state);

			AddCompositionAnimationFactory(builder);
		}
		else
		{
			auto builder = NormalizedKeyFrameAnimationBuilderXaml<T>(
				property,
				delay,
				duration.value_or(AnimationExtensions::DefaultDuration()),
				repeatOption.value_or(RepeatOptionHelper::Once()));

			build(builder, state);

			AddXamlAnimationFactory(builder);
		}

		return *this;
	}

	/// <summary>
	/// Adds a custom animation based on timed keyframes to the current schedule.
	/// </summary>
	/// <typeparam name="T">The type of values to animate.</typeparam>
	/// <param name="builder">The AnimationBuilder instance.</param>
	/// <param name="property">The target property to animate.</param>
	/// <param name="build">The callback to use to construct the custom animation.</param>
	/// <param name="delay">The optional initial delay for the animation.</param>
	/// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
	/// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
	/// <param name="layer">The target framework layer to animate.</param>
	/// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
	template<typename T>
	inline AnimationBuilder& AnimationBuilder::TimedKeyFrames(
		winrt::hstring const& property,
		std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
		std::optional<winrt::Windows::Foundation::TimeSpan> delay,
		std::optional<RepeatOption> repeatOption,
		std::optional<AnimationDelayBehavior> delayBehavior,
		FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			auto builder = TimedKeyFrameAnimationBuilderComposition<T>(
				property,
				delay,
				repeatOption.value_or(RepeatOptionHelper::Once()),
				delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

			build(builder);

			AddCompositionAnimationFactory(builder);
		}
		else
		{
			auto builder = TimedKeyFrameAnimationBuilderXaml<T>(
				property,
				delay,
				repeatOption.value_or(RepeatOptionHelper::Once()));

			build(builder);

			AddXamlAnimationFactory(builder);
		}

		return *this;
	}

	/// <summary>
	/// Adds a custom animation based on timed keyframes to the current schedule.
	/// </summary>
	/// <typeparam name="T">The type of values to animate.</typeparam>
	/// <typeparam name="TState">The type of state to pass to the builder.</typeparam>
	/// <param name="builder">The AnimationBuilder instance.</param>
	/// <param name="property">The target property to animate.</param>
	/// <param name="state">The state to pass to the builder.</param>
	/// <param name="build">The callback to use to construct the custom animation.</param>
	/// <param name="delay">The optional initial delay for the animation.</param>
	/// <param name="repeat">The repeat option for the animation (defaults to one iteration).</param>
	/// <param name="delayBehavior">The delay behavior to use (ignored if <paramref name="layer"/> is <see cref="FrameworkLayer.Xaml"/>).</param>
	/// <param name="layer">The target framework layer to animate.</param>
	/// <returns>The current <see cref="AnimationBuilder"/> instance.</returns>
	template<typename T, typename TState>
	inline AnimationBuilder& AnimationBuilder::TimedKeyFrames(
		winrt::hstring const& property,
		TState state,
		std::function<void(ITimedKeyFrameAnimationBuilder<T>&, TState)> build,
		std::optional<TimeSpan> delay,
		std::optional<RepeatOption> repeatOption,
		std::optional<AnimationDelayBehavior> delayBehavior,
		FrameworkLayer layer)
	{
		if (layer == FrameworkLayer::Composition)
		{
			auto builder = TimedKeyFrameAnimationBuilderComposition<T>(
				property,
				delay,
				repeatOption.value_or(RepeatOptionHelper::Once()),
				delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

			build(builder, state);

			AddCompositionAnimationFactory(builder);
		}
		else
		{
			auto builder = TimedKeyFrameAnimationBuilderXaml<T>(
				property,
				delay,
				repeatOption.value_or(RepeatOptionHelper::Once()));

			build(builder, state);

			AddXamlAnimationFactory(builder);
		}

		return *this;
	}
}

