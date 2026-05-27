#pragma once
#include "../Options/RepeatOptionHelper.h"
#include "AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    using TimeSpan = winrt::Windows::Foundation::TimeSpan;

	class CompositorExtensions {
	public:
		/// <summary>
		/// Creates the appropriate <see cref="CompositionEasingFunction"/> from the given easing type and mode.
		/// </summary>
		/// <param name="compositor">The source <see cref="Compositor"/> used to create the easing function.</param>
		/// <param name="easingType">The target easing function to use.</param>
		/// <param name="easingMode">The target easing mode to use.</param>
		/// <returns>
		/// A <see cref="CompositionEasingFunction"/> instance with the specified easing, or <see langword="nullptr"/>
		/// when the input parameters refer to the built-in easing, which means no instance is needed.
		/// </returns>
		static CompositionEasingFunction TryCreateEasingFunction(Compositor const& compositor, EasingType easingType = AnimationExtensions::DefaultEasingType(), EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
		{
			if (easingType == AnimationExtensions::DefaultEasingType() && easingMode == AnimationExtensions::DefaultEasingMode())
			{
				return nullptr;
			}

			if (easingType == EasingType::Linear)
			{
				return compositor.CreateLinearEasingFunction();
			}

			float2 a, b;
			std::tuple<EasingType, EasingMode> key(easingType, easingMode);
			std::tie(a, b) = AnimationExtensions::EasingMaps.at(key);

			return compositor.CreateCubicBezierEasingFunction(a, b);
		}

		/// <summary>
		/// Creates a <see cref="CubicBezierEasingFunction"/> from the input control points.
		/// </summary>
		/// <param name="compositor">The source <see cref="CompositionObject"/> used to create the easing function.</param>
		/// <param name="x1">The X coordinate of the first control point.</param>
		/// <param name="y1">The Y coordinate of the first control point.</param>
		/// <param name="x2">The X coordinate of the second control point.</param>
		/// <param name="y2">The Y coordinate of the second control point.</param>
		/// <returns>A <see cref="CubicBezierEasingFunction"/> instance with the given control points.</returns>
		static CubicBezierEasingFunction CreateCubicBezierEasingFunction(Compositor const& compositor, float x1, float y1, float x2, float y2)
		{
			return compositor.CreateCubicBezierEasingFunction(float2(x1, y1), float2(x2, y2));
		}

		/// <summary>
		/// Creates a <see cref="BooleanKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="BooleanKeyFrameAnimation"/> instance with the specified parameters.</returns>
		static BooleanKeyFrameAnimation CreateBooleanKeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			bool to,
			std::optional<bool> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			BooleanKeyFrameAnimation animation = compositor.CreateBooleanKeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			animation.InsertKeyFrame(1, to);

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="ScalarKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="ScalarKeyFrameAnimation"/> instance with the specified parameters.</returns>
		static ScalarKeyFrameAnimation CreateScalarKeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			float to,
			std::optional<float> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			ScalarKeyFrameAnimation animation = compositor.CreateScalarKeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="ScalarKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="Vector2KeyFrameAnimation"/> instance with the specified parameters.</returns>
		static Vector2KeyFrameAnimation CreateVector2KeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			float2 to,
			std::optional<float2> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			Vector2KeyFrameAnimation animation = compositor.CreateVector2KeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="ScalarKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="Vector3KeyFrameAnimation"/> instance with the specified parameters.</returns>
		static Vector3KeyFrameAnimation CreateVector3KeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			float3 to,
			std::optional<float3> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			Vector3KeyFrameAnimation animation = compositor.CreateVector3KeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="Vector4KeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="Vector4KeyFrameAnimation"/> instance with the specified parameters.</returns>
		static Vector4KeyFrameAnimation CreateVector4KeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			float4 to,
			std::optional<float4> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			Vector4KeyFrameAnimation animation = compositor.CreateVector4KeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="ColorKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="ColorKeyFrameAnimation"/> instance with the specified parameters.</returns>
		static ColorKeyFrameAnimation CreateColorKeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			Windows::UI::Color to,
			std::optional<Windows::UI::Color> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			ColorKeyFrameAnimation animation = compositor.CreateColorKeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		}

		/// <summary>
		/// Creates a <see cref="QuaternionKeyFrameAnimation"/> instance with the given parameters to on a target element.
		/// </summary>
		/// <param name="compositor">The current <see cref="Compositor"/> instance used to create the animation.</param>
		/// <param name="target">The optional target property to animate.</param>
		/// <param name="to">The final value for the animation.</param>
		/// <param name="from">The optional starting value for the animation.</param>
		/// <param name="delay">The optional initial delay for the animation.</param>
		/// <param name="duration">The optional animation duration.</param>
		/// <param name="easing">The optional easing function for the animation.</param>
		/// <param name="delayBehavior">The delay behavior to use for the animation.</param>
		/// <param name="direction">The direction to use when playing the animation.</param>
		/// <param name="iterationBehavior">The iteration behavior to use for the animation.</param>
		/// <param name="iterationCount">The iteration count to use for the animation.</param>
		/// <returns>A <see cref="QuaternionKeyFrameAnimation"/> instance with the specified parameters.</returns>
		static QuaternionKeyFrameAnimation CreateQuaternionKeyFrameAnimation(
			Compositor const& compositor,
			std::optional<winrt::hstring> target,
			quaternion to,
			std::optional<quaternion> from = std::nullopt,
			std::optional<TimeSpan> delay = std::nullopt,
			std::optional<TimeSpan> duration = std::nullopt,
			CompositionEasingFunction const& easing = nullptr,
			AnimationDelayBehavior delayBehavior = AnimationDelayBehavior::SetInitialValueBeforeDelay,
			AnimationDirection direction = AnimationDirection::Normal,
			AnimationIterationBehavior iterationBehavior = AnimationIterationBehavior::Count,
			int iterationCount = 1)
		{
			QuaternionKeyFrameAnimation animation = compositor.CreateQuaternionKeyFrameAnimation();

			animation.Duration(duration ? *duration : AnimationExtensions::DefaultDuration());

			if (delay.has_value())
			{
				animation.DelayTime(delay.value());
				animation.DelayBehavior(delayBehavior);
			}

			if (easing == nullptr)
			{
				animation.InsertKeyFrame(1, to);
			}
			else
			{
				animation.InsertKeyFrame(1, to, easing);
			}

			if (from.has_value())
			{
				animation.InsertKeyFrame(0, from.value());
			}

			if (target)
			{
				animation.Target(*target);
			}

			animation.Direction(direction);
			animation.IterationBehavior(iterationBehavior);
			animation.IterationCount(iterationCount);

			return animation;
		};
	};
}
