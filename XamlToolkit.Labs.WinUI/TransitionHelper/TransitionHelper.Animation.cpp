#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"
#include "../XamlToolkit.WinUI.Animations/Extensions/AnimationExtensions.h"
#include "../XamlToolkit.WinUI.Animations/Extensions/CompositorExtensions.h"

namespace winrt
{
	using namespace ::winrt::XamlToolkit::WinUI::Animations;
	using namespace ::winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	winrt::CompositionEasingFunction TransitionHelper::EasingFunctionFactory::GetEasingFunction(winrt::Compositor const& compositor, bool inverse) const
	{
		return TransitionHelper::GetEasingFunction(compositor, Type, Mode, Inverse ^ inverse);
	}

	winrt::CompositionEasingFunction TransitionHelper::GetEasingFunction(
		winrt::Compositor const& compositor,
		winrt::XamlToolkit::WinUI::Animations::EasingType type,
		winrt::EasingMode mode,
		bool inverse)
	{
		if (type == winrt::EasingType::Linear)
		{
			return compositor.CreateLinearEasingFunction();
		}

		if (type == winrt::EasingType::Default && mode == winrt::EasingMode::EaseInOut)
		{
			return inverse ? compositor.CreateCubicBezierEasingFunction({ 1.f, 0.06f }, { 0.59f, 0.48f }) : nullptr;
		}

		auto it = AnimationExtensions::EasingMaps.find({ type, mode });
		if (it == AnimationExtensions::EasingMaps.end())
		{
			return nullptr;
		}

		const auto& [a, b] = it->second;

		if (inverse)
		{
			return compositor.CreateCubicBezierEasingFunction({ 1.f - b.x, 1.f - b.y }, { 1.f - a.x, 1.f - a.y });
		}

		return compositor.CreateCubicBezierEasingFunction(a, b);
	}

	std::array<TransitionHelper::ClipScalarAnimationFactory, 4> TransitionHelper::Clip(
		winrt::float4 to, std::optional<EasingFunctionFactory> easingFactory, std::optional<winrt::float4> from,
		std::optional<winrt::TimeSpan> delay, std::optional<winrt::TimeSpan> duration) const
	{
		return
		{
			ClipScalarAnimationFactory(L"LeftInset",   to.x, from ? std::optional{from->x} : std::nullopt, delay, duration, easingFactory),
			ClipScalarAnimationFactory(L"TopInset",    to.y, from ? std::optional{from->y} : std::nullopt, delay, duration, easingFactory),
			ClipScalarAnimationFactory(L"RightInset",  to.z, from ? std::optional{from->z} : std::nullopt, delay, duration, easingFactory),
			ClipScalarAnimationFactory(L"BottomInset", to.w, from ? std::optional{from->w} : std::nullopt, delay, duration, easingFactory)
		};
	}

	TransitionHelper::KeyFrameAnimationFactory<winrt::float2> TransitionHelper::Translation(
		winrt::float2 to, std::optional<EasingFunctionFactory> easingFactory, std::optional<winrt::float2> from,
		std::optional<winrt::TimeSpan> delay, std::optional<winrt::TimeSpan> duration) const
	{
		return KeyFrameAnimationFactory<winrt::float2>(L"Translation.XY", to, from, delay, duration, easingFactory);
	}

	TransitionHelper::KeyFrameAnimationFactory<float> TransitionHelper::Opacity(
		float to, std::optional<EasingFunctionFactory> easingFactory, std::optional<float> from,
		std::optional<winrt::TimeSpan> delay, std::optional<winrt::TimeSpan> duration,
		std::optional<std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>>> normalizedKeyFrames,
		std::optional<std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>>> reversedNormalizedKeyFrames) const
	{
		return KeyFrameAnimationFactory<float>(L"Opacity", to, from, delay, duration, easingFactory, normalizedKeyFrames, reversedNormalizedKeyFrames);
	}

	TransitionHelper::KeyFrameAnimationFactory<winrt::float2> TransitionHelper::Scale(
		winrt::float2 to, std::optional<EasingFunctionFactory> easingFactory, std::optional<winrt::float2> from,
		std::optional<winrt::TimeSpan> delay, std::optional<winrt::TimeSpan> duration) const
	{
		return KeyFrameAnimationFactory<winrt::float2>(L"Scale.XY", to, from, delay, duration, easingFactory);
	}

	template<>
	winrt::KeyFrameAnimation TransitionHelper::KeyFrameAnimationFactory<float>::GetAnimation(
		winrt::Compositor const& compositor, bool reversed, bool useReversedKeyframes, bool inverseEasingFunction, winrt::CompositionObject& target) const
	{
		target = nullptr;

		auto direction = reversed ? winrt::AnimationDirection::Reverse : winrt::AnimationDirection::Normal;
		auto easing = EasingFactory ? EasingFactory->GetEasingFunction(compositor, inverseEasingFunction) : nullptr;

		auto scalarAnimation = winrt::CompositorExtensions::CreateScalarKeyFrameAnimation(
			compositor,
			Property,
			To,
			From,
			Delay,
			Duration,
			easing,
			winrt::AnimationDelayBehavior::SetInitialValueBeforeDelay,
			direction);

		const auto& keyFrames = (useReversedKeyframes && ReversedNormalizedKeyFrames.has_value())
			? ReversedNormalizedKeyFrames : NormalizedKeyFrames;

		if (keyFrames.has_value())
		{
			for (const auto& [k, p] : keyFrames.value())
			{
				scalarAnimation.InsertKeyFrame(k, p.first, p.second ? p.second->GetEasingFunction(compositor, inverseEasingFunction) : nullptr);
			}
		}

		return scalarAnimation;
	}

	template<>
	winrt::KeyFrameAnimation TransitionHelper::KeyFrameAnimationFactory<winrt::float2>::GetAnimation(
		winrt::Compositor const& compositor, bool reversed, bool useReversedKeyframes, bool inverseEasingFunction, winrt::CompositionObject& target) const
	{
		target = nullptr;

		auto direction = reversed ? winrt::AnimationDirection::Reverse : winrt::AnimationDirection::Normal;
		auto easing = EasingFactory ? EasingFactory->GetEasingFunction(compositor, inverseEasingFunction) : nullptr;

		auto vector2Animation = winrt::CompositorExtensions::CreateVector2KeyFrameAnimation(
			compositor,
			Property,
			To,
			From,
			Delay,
			Duration,
			easing,
			winrt::AnimationDelayBehavior::SetInitialValueBeforeDelay,
			direction);

		const auto& keyFrames = (useReversedKeyframes && ReversedNormalizedKeyFrames.has_value())
			? ReversedNormalizedKeyFrames : NormalizedKeyFrames;

		if (keyFrames.has_value())
		{
			for (const auto& [k, p] : keyFrames.value())
			{
				vector2Animation.InsertKeyFrame(k, p.first, p.second ? p.second->GetEasingFunction(compositor, inverseEasingFunction) : nullptr);
			}
		}

		return vector2Animation;
	}

	winrt::KeyFrameAnimation TransitionHelper::ClipScalarAnimationFactory::GetAnimation(
		winrt::Compositor const& compositor, bool reversed, bool, bool inverseEasingFunction, winrt::CompositionObject& target) const
	{
		auto clip = compositor.CreateInsetClip();
		target = clip;
		auto direction = reversed ? winrt::AnimationDirection::Reverse : winrt::AnimationDirection::Normal;
		auto easing = EasingFactory ? EasingFactory->GetEasingFunction(compositor, inverseEasingFunction) : nullptr;
		return winrt::CompositorExtensions::CreateScalarKeyFrameAnimation(
			compositor,
			Property,
			To,
			From,
			Delay,
			Duration,
			easing,
			winrt::AnimationDelayBehavior::SetInitialValueBeforeDelay,
			direction);
	}

	void TransitionHelper::KeyFrameAnimationGroupController::AddAnimationFor(winrt::UIElement const& target, AnimationFactory factory)
	{
		if (!factory)
		{
			return;
		}

		_animationFactories[target].push_back(std::move(factory));
	}

	winrt::IAsyncAction TransitionHelper::KeyFrameAnimationGroupController::StartAsync(
		concurrency::cancellation_token token, std::optional<winrt::TimeSpan> duration)
	{
		auto start = LastStopProgress;
		bool isInterruptedAnimation = start.has_value();
		if (!isInterruptedAnimation)
		{
			_lastStartInNormalDirection = true;
		}

		auto inverseEasing = isInterruptedAnimation && _lastInverseEasingFunction;
		auto useReversedKeyframes = isInterruptedAnimation && !_lastStartInNormalDirection;
		return AnimateAsync(false, useReversedKeyframes, token, inverseEasing, duration, start);
	}

	winrt::IAsyncAction TransitionHelper::KeyFrameAnimationGroupController::ReverseAsync(
		concurrency::cancellation_token token, bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration)
	{
		std::optional<float> start;
		if (LastStopProgress.has_value())
		{
			start = 1.f - LastStopProgress.value();
		}

		bool isInterruptedAnimation = start.has_value();
		if (!isInterruptedAnimation)
		{
			_lastStartInNormalDirection = false;
		}

		auto inverseEasing = (isInterruptedAnimation && _lastInverseEasingFunction) || (!isInterruptedAnimation && inverseEasingFunction);
		auto useReversedKeyframes = !isInterruptedAnimation || !_lastStartInNormalDirection;
		return AnimateAsync(true, useReversedKeyframes, token, inverseEasing, duration, start);
	}

	winrt::IAsyncAction TransitionHelper::KeyFrameAnimationGroupController::AnimateAsync(
		bool reversed, bool useReversedKeyframes, concurrency::cancellation_token token,
		bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration, std::optional<float> startProgress)
	{
		if (_animationFactories.empty()) co_return;

		std::vector<std::pair<winrt::CompositionObject, winrt::hstring>> compositionAnimations;
		auto animationStartTime = std::chrono::steady_clock::now();
		if (duration.has_value() && startProgress.has_value())
		{
			auto ms = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration.value()).count() * startProgress.value());
			animationStartTime -= std::chrono::milliseconds(ms);
		}

		LastStopProgress.reset();
		CurrentDirection = reversed ? winrt::AnimationDirection::Reverse : winrt::AnimationDirection::Normal;
		_lastInverseEasingFunction = inverseEasingFunction;

		auto stopCb = token.register_callback([this, reversed, duration, animationStartTime, &compositionAnimations]()
		{
			for (const auto& [target, path] : compositionAnimations)
			{
				target.StopAnimation(path);
			}
			if (!duration.has_value()) return;
			auto elapsed = std::chrono::steady_clock::now() - animationStartTime;
			auto ems = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
			auto tms = std::chrono::duration_cast<std::chrono::milliseconds>(duration.value()).count();
			if (tms > 0)
			{
				LastStopProgress = static_cast<float>(reversed ? 1.0 - std::min(1.0, static_cast<double>(ems) / tms) : std::min(1.0, static_cast<double>(ems) / tms));
			}
		});

		std::vector<winrt::IAsyncAction> tasks;
		for (const auto& [element, _] : _animationFactories)
		{
			tasks.push_back(StartAnimation(element, reversed, useReversedKeyframes, inverseEasingFunction, duration, startProgress, compositionAnimations));
		}

		for (const auto& t : tasks) co_await t;
	}

	winrt::IAsyncAction TransitionHelper::KeyFrameAnimationGroupController::StartAnimation(
		winrt::UIElement const& element, bool reversed, bool useReversedKeyframes,
		bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration,
		std::optional<float> startProgress,
		std::vector<std::pair<winrt::CompositionObject, winrt::hstring>>& compositionAnimations)
	{
		const auto it = _animationFactories.find(element);
		if (it == _animationFactories.end()) co_return;

		winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
		auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
		auto compositor = visual.Compositor();
		auto batch = compositor.CreateScopedBatch(winrt::CompositionBatchTypes::Animation);
		wil::shared_event done(wil::EventOptions::ManualReset);
		batch.Completed([done](auto&&, auto&&) { done.SetEvent(); });

		for (const auto& factory : it->second)
		{
			winrt::CompositionObject target{ nullptr };
			auto anim = factory->GetAnimation(compositor, reversed, useReversedKeyframes, inverseEasingFunction, target);
			if (duration.has_value()) anim.Duration(duration.value());
			const auto& startTarget = target ? target : visual;
			startTarget.StartAnimation(anim.Target(), anim);
			if (startProgress.has_value())
			{
				if (auto ctrl = startTarget.TryGetAnimationController(anim.Target()))
				{
					ctrl.Progress(startProgress.value());
				}
			}

			compositionAnimations.emplace_back(startTarget, anim.Target());
		}

		batch.End();
		co_await winrt::resume_on_signal(done.get());
	}
}
