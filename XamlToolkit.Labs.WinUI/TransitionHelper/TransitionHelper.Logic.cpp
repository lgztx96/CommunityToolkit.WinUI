#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"
#include "TransitionConfig.h"

namespace winrt
{
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	void TransitionHelper::RestoreState(bool isTargetState)
	{
		IsTargetState(isTargetState);
		if (const auto source = Source())
		{
			winrt::Canvas::SetZIndex(source, _sourceZIndex);
			ToggleVisualState(source, SourceToggleMethod, !isTargetState);
			RestoreAnimatedElements(SourceAnimatedElements());
		}

		if (const auto target = Target())
		{
			winrt::Canvas::SetZIndex(target, _targetZIndex);
			ToggleVisualState(target, TargetToggleMethod, isTargetState);
			RestoreAnimatedElements(TargetAnimatedElements());
		}
	}

	winrt::IAsyncAction TransitionHelper::InitControlsStateAsync(bool forceUpdateAnimatedElements)
	{
		auto maxZIndex = std::max(_sourceZIndex, _targetZIndex) + 1;
		winrt::Canvas::SetZIndex(IsTargetState() ? _source : _target, maxZIndex);

		co_await winrt::when_all(InitControlStateAsync(_source), InitControlStateAsync(_target));

		if (forceUpdateAnimatedElements)
		{
			_sourceAnimatedElements.reset();
			_targetAnimatedElements.reset();
		}
	}

	winrt::IAsyncAction TransitionHelper::InitControlStateAsync(winrt::FrameworkElement target)
	{
		if (!target)
		{
			co_return;
		}

		target.IsHitTestVisible(IsHitTestVisibleWhenAnimating());
		if (target.Visibility() == winrt::Visibility::Collapsed)
		{
			target.Visibility(winrt::Visibility::Visible);
			co_await UpdateControlLayout(target);
		}
		else if (target.Opacity() < AlmostZero)
		{
			target.Opacity(1);
		}

		if (auto visual = winrt::ElementCompositionPreview::GetElementVisual(target); !visual.IsVisible())
		{
			visual.IsVisible(true);
		}
	}

	winrt::IAsyncAction TransitionHelper::AnimateControlsAsync(bool reversed, bool forceUpdateAnimatedElements)
	{
		auto strongThis = get_strong();

		IsNotNullAndIsInVisualTree(_source, L"Source");
		IsNotNullAndIsInVisualTree(_target, L"Target");

		if (IsAnimating())
		{
			bool currentIsReverse = _currentAnimationGroupController.has_value() &&
				_currentAnimationGroupController->CurrentDirection.has_value() &&
				(_currentAnimationGroupController->CurrentDirection == winrt::AnimationDirection::Reverse);
			if (currentIsReverse == reversed)
			{
				co_return;
			}

			Stop();
		}
		else if (IsTargetState() == !reversed)
		{
			co_return;
		}
		else
		{
			_currentAnimationGroupController.reset();
			co_await InitControlsStateAsync(forceUpdateAnimatedElements);
		}

		_currentAnimationCancellationTokenSource.emplace();

		auto token = co_await winrt::get_cancellation_token();

		token.callback([tokenSource = *_currentAnimationCancellationTokenSource] { tokenSource.cancel(); });

		auto animationCancelToken = _currentAnimationCancellationTokenSource->get_token();

		co_await AnimateControlsImpAsync(reversed ? ReverseDuration() : Duration(), reversed, animationCancelToken);
		if (animationCancelToken.is_canceled())
		{
			co_return;
		}

		_currentAnimationGroupController.reset();
		RestoreState(!reversed);
	}

	winrt::IAsyncAction TransitionHelper::AnimateControlsImpAsync(winrt::TimeSpan duration, bool reversed, concurrency::cancellation_token token)
	{
		if (!_currentAnimationGroupController.has_value())
		{
			_currentAnimationGroupController.emplace();
		}

		for (const auto& [id, sourceElement] : SourceAnimatedElements().ConnectedElements)
		{
			auto targetIt = TargetAnimatedElements().ConnectedElements.find(id);
			if (targetIt != TargetAnimatedElements().ConnectedElements.end())
			{
				auto animationConfig = DefaultConfig();
				for (const auto& item : Configs())
				{
					if (item.Id() == id)
					{
						animationConfig = item;
						break;
					}
				}

				auto sourceCoord = _sourceAnimatedElements->CoordinatedElements.find(id);
				auto targetCoord = _targetAnimatedElements->CoordinatedElements.find(id);
				auto sourceCoordinated = sourceCoord != _sourceAnimatedElements->CoordinatedElements.end()
					? std::span{ sourceCoord->second } : std::span<winrt::UIElement>{};

				auto targetCoordinated = targetCoord != _targetAnimatedElements->CoordinatedElements.end()
					? std::span{ targetCoord->second } : std::span<winrt::UIElement>{};

				BuildConnectedAnimationController(
					_currentAnimationGroupController.value(),
					sourceElement, targetIt->second,
					sourceCoordinated, targetCoordinated,
					duration, animationConfig);
			}
		}

		std::vector<winrt::UIElement> sourceIndependent = _sourceAnimatedElements->IndependentElements;
		std::vector<winrt::UIElement> targetIndependent = _targetAnimatedElements->IndependentElements;
		for (const auto& [id, element] : _sourceAnimatedElements->ConnectedElements)
		{
			if (!_targetAnimatedElements->ConnectedElements.contains(id))
			{
				sourceIndependent.push_back(element);
				auto coordIt = _sourceAnimatedElements->CoordinatedElements.find(id);
				if (coordIt != _sourceAnimatedElements->CoordinatedElements.end())
				{
					sourceIndependent.insert(sourceIndependent.end(), coordIt->second.begin(), coordIt->second.end());
				}
			}
		}
		for (const auto& [id, element] : _targetAnimatedElements->ConnectedElements)
		{
			if (!_sourceAnimatedElements->ConnectedElements.contains(id))
			{
				targetIndependent.push_back(element);
				auto coordIt = _targetAnimatedElements->CoordinatedElements.find(id);
				if (coordIt != _targetAnimatedElements->CoordinatedElements.end())
				{
					targetIndependent.insert(targetIndependent.end(), coordIt->second.begin(), coordIt->second.end());
				}
			}
		}

		std::optional<winrt::TimeSpan> independentStartTime;
		if (_currentAnimationGroupController->LastStopProgress.has_value())
		{
			auto startProgress = reversed
				? (1.f - _currentAnimationGroupController->LastStopProgress.value())
				: _currentAnimationGroupController->LastStopProgress.value();
			independentStartTime = winrt::TimeSpan{ std::chrono::milliseconds(static_cast<int64_t>(startProgress * std::chrono::duration_cast<std::chrono::milliseconds>(duration).count())) };
		}

		auto indepEasingType = IndependentElementEasingType();
		auto indepEasingMode = IndependentElementEasingMode();

		std::vector<winrt::IAsyncAction> tasks;
		tasks.push_back(AnimateIndependentElements(sourceIndependent, reversed, token, independentStartTime, indepEasingType, indepEasingMode));
		tasks.push_back(AnimateIndependentElements(targetIndependent, !reversed, token, independentStartTime, indepEasingType, indepEasingMode));
		tasks.push_back(reversed
			? _currentAnimationGroupController->ReverseAsync(token, InverseEasingFunctionWhenReversing(), duration)
			: _currentAnimationGroupController->StartAsync(token, duration));

		for (const auto& t : tasks) co_await t;
	}

	void TransitionHelper::BuildConnectedAnimationController(
		KeyFrameAnimationGroupController& controller,
		winrt::UIElement const& source,
		winrt::UIElement const& target,
		std::span<winrt::UIElement> sourceCoordinated,
		std::span<winrt::UIElement> targetCoordinated,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::Labs::WinUI::TransitionConfig const& config)
	{
		auto sourceElement = source.try_as<winrt::FrameworkElement>();
		auto sourceActualSize = sourceElement
			? winrt::float2{ static_cast<float>(sourceElement.ActualWidth()), static_cast<float>(sourceElement.ActualHeight()) }
		: source.ActualSize();

		auto targetElement = target.try_as<winrt::FrameworkElement>();
		auto targetActualSize = targetElement
			? winrt::float2{ static_cast<float>(targetElement.ActualWidth()), static_cast<float>(targetElement.ActualHeight()) }
		: target.ActualSize();

		auto normalizedCenter = config.NormalizedCenterPoint();
		auto sourceCenterPoint = winrt::float2{ sourceActualSize.x * normalizedCenter.X, sourceActualSize.y * normalizedCenter.Y };
		auto targetCenterPoint = winrt::float2{ targetActualSize.x * normalizedCenter.X, targetActualSize.y * normalizedCenter.Y };

		winrt::ElementCompositionPreview::SetIsTranslationEnabled(source, true);
		winrt::ElementCompositionPreview::SetIsTranslationEnabled(target, true);
		auto sourceVisual = winrt::ElementCompositionPreview::GetElementVisual(source);
		auto targetVisual = winrt::ElementCompositionPreview::GetElementVisual(target);

		sourceVisual.CenterPoint({ sourceCenterPoint.x, sourceCenterPoint.y, 0 });
		targetVisual.CenterPoint({ targetCenterPoint.x, targetCenterPoint.y, 0 });

		auto configEasingType = config.EasingType();
		auto configEasingMode = config.EasingMode();
		auto easingType = configEasingType ? configEasingType.Value() : DefaultEasingType();
		auto easingMode = configEasingMode ? configEasingMode.Value() : DefaultEasingMode();
		auto easingFactory = GetEasingFunctionFactory(easingType, easingMode);

		auto [sourceTranslationFactory, targetTranslationFactory, translation] = AnimateTranslation(source, target, sourceCenterPoint, targetCenterPoint, duration, easingType, easingMode);
		auto [sourceScaleFactory, targetScaleFactory, sourceTargetScale] = [&]() -> TranslationScaleAnimations
		{
			switch (config.ScaleMode())
			{
				case ScaleMode::None: return { nullptr, nullptr, winrt::float2::one() };
				case ScaleMode::Scale: return AnimateScale(sourceActualSize, targetActualSize, duration, easingType, easingMode);
				case ScaleMode::ScaleX: return AnimateScaleX(sourceActualSize, targetActualSize, duration, easingType, easingMode);
				case ScaleMode::ScaleY: return AnimateScaleY(sourceActualSize, targetActualSize, duration, easingType, easingMode);
				case ScaleMode::Custom:
				{
					auto [s, t, sc] = AnimateScaleWithScaleCalculator(source, target, config.CustomScalingCalculator(), duration, easingType, easingMode);
					return { std::move(s), std::move(t), sc };
				}
				default: return { nullptr, nullptr, winrt::float2::one() };
			}
		}();

		auto opacityKeyReference = config.OpacityTransitionProgressKey();
		auto opacityKey = opacityKeyReference ? opacityKeyReference.Value() : DefaultOpacityTransitionProgressKey();
		auto [sourceOpacityFactory, targetOpacityFactory] = AnimateOpacity(duration, opacityKey);

		controller.AddAnimationGroupFor(source,
			std::move(sourceTranslationFactory),
			std::move(sourceScaleFactory),
			std::move(sourceOpacityFactory));

		controller.AddAnimationGroupFor(target,
			std::move(targetTranslationFactory),
			std::move(targetScaleFactory),
			std::move(targetOpacityFactory));

		if (!sourceCoordinated.empty() && _target)
		{
			auto targetControlBounds = winrt::Rect{ 0, 0, static_cast<float>(_target.ActualWidth()), static_cast<float>(_target.ActualHeight()) };
			auto targetTransformedBounds = _target.TransformToVisual(_source).TransformBounds(targetControlBounds);

			for (const auto& element : sourceCoordinated)
			{
				auto frameworkElement = element.try_as<winrt::FrameworkElement>();
				auto coordinatedSize = frameworkElement
					? winrt::float2{ static_cast<float>(frameworkElement.ActualWidth()), static_cast<float>(frameworkElement.ActualHeight()) }
				: element.ActualSize();
				auto coordinatedCenterPoint = winrt::float2{ coordinatedSize.x * normalizedCenter.X, coordinatedSize.y * normalizedCenter.Y };
				winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
				auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
				visual.CenterPoint({ coordinatedCenterPoint.x, coordinatedCenterPoint.y, 0 });

				auto sourceTranslationCopy = std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(*static_cast<KeyFrameAnimationFactory<winrt::float2>*>(sourceTranslationFactory.get()));
				auto sourceScaleCopy = sourceScaleFactory ? std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(*static_cast<KeyFrameAnimationFactory<winrt::float2>*>(sourceScaleFactory.get())) : nullptr;
				auto sourceOpacityCopy = std::make_unique<KeyFrameAnimationFactory<float>>(*static_cast<KeyFrameAnimationFactory<float>*>(sourceOpacityFactory.get()));

				controller.AddAnimationGroupFor(element,
					std::move(sourceTranslationCopy),
					std::move(sourceScaleCopy),
					std::move(sourceOpacityCopy));

				auto initialLocation = element.TransformToVisual(_source).TransformPoint({});
				auto clipResult = GetTargetClip(
					winrt::float2{ static_cast<float>(initialLocation.X), static_cast<float>(initialLocation.Y) },
					coordinatedSize, coordinatedCenterPoint,
					sourceTargetScale, translation, targetTransformedBounds);

				if (clipResult.has_value())
				{
					for (const auto& cf : Clip(clipResult.value(), easingFactory, std::nullopt, std::nullopt, duration))
					{
						controller.AddAnimationFor(element, std::make_unique<ClipScalarAnimationFactory>(cf));
					}
				}
			}
		}

		if (!targetCoordinated.empty() && _source)
		{
			auto sourceControlBounds = winrt::Rect{ 0, 0, static_cast<float>(_source.ActualWidth()), static_cast<float>(_source.ActualHeight()) };
			auto sourceTransformedBounds = _source.TransformToVisual(_target).TransformBounds(sourceControlBounds);
			auto targetScaleInverse = GetInverseScale(sourceTargetScale);

			for (const auto& element : targetCoordinated)
			{
				auto frameworkElement = element.try_as<winrt::FrameworkElement>();
				auto coordinatedSize = frameworkElement
					? winrt::float2{ static_cast<float>(frameworkElement.ActualWidth()), static_cast<float>(frameworkElement.ActualHeight()) }
				: element.ActualSize();
				auto coordinatedCenterPoint = winrt::float2{ coordinatedSize.x * normalizedCenter.X, coordinatedSize.y * normalizedCenter.Y };
				winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
				auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
				visual.CenterPoint({ coordinatedCenterPoint.x, coordinatedCenterPoint.y, 0 });

				auto targetTranslationCopy = std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(*static_cast<KeyFrameAnimationFactory<winrt::float2>*>(targetTranslationFactory.get()));
				auto targetScaleCopy = targetScaleFactory ? std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(*static_cast<KeyFrameAnimationFactory<winrt::float2>*>(targetScaleFactory.get())) : nullptr;
				auto targetOpacityCopy = std::make_unique<KeyFrameAnimationFactory<float>>(*static_cast<KeyFrameAnimationFactory<float>*>(targetOpacityFactory.get()));

				controller.AddAnimationGroupFor(element,
					std::move(targetTranslationCopy),
					std::move(targetScaleCopy),
					std::move(targetOpacityCopy));

				auto initialLocation = element.TransformToVisual(_target).TransformPoint({});
				auto clipResult = GetTargetClip(
					winrt::float2{ static_cast<float>(initialLocation.X), static_cast<float>(initialLocation.Y) },
					coordinatedSize, coordinatedCenterPoint,
					targetScaleInverse, -translation, sourceTransformedBounds);

				if (clipResult.has_value())
				{
					for (auto& cf : Clip(winrt::float4{}, easingFactory, clipResult.value(), std::nullopt, duration))
					{
						controller.AddAnimationFor(element, std::make_unique<ClipScalarAnimationFactory>(cf));
					}
				}
			}
		}

		if (config.EnableClipAnimation())
		{
			auto [sourceClipFactories, targetClipFactories] = AnimateClip(sourceActualSize, targetActualSize, sourceCenterPoint, targetCenterPoint, sourceTargetScale, duration, easingType, easingMode);
			if (sourceClipFactories.has_value())
			{
				for (auto& cf : sourceClipFactories.value())
				{
					controller.AddAnimationFor(source, std::make_unique<ClipScalarAnimationFactory>(cf));
				}
			}

			if (targetClipFactories.has_value())
			{
				for (auto& cf : targetClipFactories.value())
				{
					controller.AddAnimationFor(target, std::make_unique<ClipScalarAnimationFactory>(cf));
				}
			}
		}
	}

	winrt::IAsyncAction TransitionHelper::AnimateIndependentElements(
		std::span<winrt::UIElement> elements,
		bool isShow,
		concurrency::cancellation_token token,
		std::optional<winrt::TimeSpan> startTime,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		if (elements.empty()) co_return;

		KeyFrameAnimationGroupController controller;
		auto duration = isShow ? IndependentElementShowDuration() : IndependentElementHideDuration();
		auto delay = isShow ? IndependentElementShowDelay() : winrt::TimeSpan{ 0 };
		auto opacityFrom = isShow ? 0.f : 1.f;
		auto opacityTo = isShow ? 1.f : 0.f;

		for (const auto& item : elements)
		{
			if (startTime.has_value() && startTime->count() > 0 && delay < startTime.value())
			{
				if (isShow)
				{
					RestoreElement(item);
				}
				else
				{
					winrt::ElementCompositionPreview::GetElementVisual(item).Opacity(0);
				}
			}
			else
			{
				auto indTrans = GetIndependentTranslation(item);
				auto t = indTrans ? indTrans.Value() : DefaultIndependentTranslation();
				auto translationFrom = isShow ? winrt::float2{ t.X, t.Y } : winrt::float2::zero();
				auto translationTo = isShow ? winrt::float2::zero() : winrt::float2{ t.X, t.Y };
				auto useDelay = delay - startTime.value_or(winrt::TimeSpan{ 0 });
				auto ef = GetEasingFunctionFactory(easingType, easingMode);

				if (std::abs(t.X) > AlmostZero || std::abs(t.Y) > AlmostZero)
				{
					controller.AddAnimationFor(item, std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(
						Translation(translationTo, ef, startTime.has_value() ? std::optional<winrt::float2>{} : translationFrom, useDelay, duration)));
				}

				controller.AddAnimationFor(item, std::make_unique<KeyFrameAnimationFactory<float>>(
					Opacity(opacityTo, ef, startTime.has_value() ? std::optional<float>{} : opacityFrom, useDelay, duration)));
			}

			if (isShow)
			{
				delay = delay + IndependentElementShowInterval();
			}
		}

		co_await controller.StartAsync(token, std::nullopt);
	}

	TransitionHelper::TranslationScaleAnimations TransitionHelper::AnimateTranslation(
		winrt::UIElement const& source,
		winrt::UIElement const& target,
		winrt::float2 const& sourceCenterPoint,
		winrt::float2 const& targetCenterPoint,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto point = target.TransformToVisual(source).TransformPoint({});
		auto translation = winrt::float2{ point.X, point.Y } - sourceCenterPoint + targetCenterPoint;
		auto easingFactory = GetEasingFunctionFactory(easingType, easingMode);

		return
		{
			std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(Translation(translation, easingFactory, winrt::float2::zero(), std::nullopt, duration)),
			std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(Translation(winrt::float2::zero(), easingFactory, -translation, std::nullopt, duration)),
			translation
		};
	}

	TransitionHelper::TranslationScaleAnimations TransitionHelper::AnimateScale(
		winrt::float2 const& sourceActualSize,
		winrt::float2 const& targetActualSize,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto scaleX = targetActualSize.x / (sourceActualSize.x == 0 ? 1.f : sourceActualSize.x);
		auto scaleY = targetActualSize.y / (sourceActualSize.y == 0 ? 1.f : sourceActualSize.y);
		auto scale = winrt::float2{ scaleX, scaleY };
		auto [sourceFactory, targetFactory] = AnimateScaleImp(scale, duration, easingType, easingMode);
		return { std::move(sourceFactory), std::move(targetFactory), scale };
	}

	TransitionHelper::TranslationScaleAnimations TransitionHelper::AnimateScaleX(
		winrt::float2 const& sourceActualSize,
		winrt::float2 const& targetActualSize,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto scaleX = targetActualSize.x / (sourceActualSize.x == 0 ? 1.f : sourceActualSize.x);
		auto scale = winrt::float2{ scaleX, scaleX };
		auto [sourceFactory, targetFactory] = AnimateScaleImp(scale, duration, easingType, easingMode);
		return { std::move(sourceFactory), std::move(targetFactory), scale };
	}

	TransitionHelper::TranslationScaleAnimations TransitionHelper::AnimateScaleY(
		winrt::float2 const& sourceActualSize,
		winrt::float2 const& targetActualSize,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto scaleY = targetActualSize.y / (sourceActualSize.y == 0 ? 1.f : sourceActualSize.y);
		auto scale = winrt::float2{ scaleY, scaleY };
		auto [sourceFactory, targetFactory] = AnimateScaleImp(scale, duration, easingType, easingMode);
		return { std::move(sourceFactory), std::move(targetFactory), scale };
	}

	TransitionHelper::TranslationScaleAnimations TransitionHelper::AnimateScaleWithScaleCalculator(
		winrt::UIElement const& source,
		winrt::UIElement const& target,
		winrt::XamlToolkit::Labs::WinUI::IScalingCalculator const& calculator,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		if (!calculator) return { nullptr, nullptr, winrt::float2::one() };
		auto scale = calculator.GetScaling(source, target);
		auto [sourceFactory, targetFactory] = AnimateScaleImp(scale, duration, easingType, easingMode);
		return { std::move(sourceFactory), std::move(targetFactory), scale };
	}

	TransitionHelper::ScaleAnimations TransitionHelper::AnimateScaleImp(
		winrt::float2 targetScale,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto easingFactory = GetEasingFunctionFactory(easingType, easingMode);
		return
		{
			std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(Scale(targetScale, easingFactory, winrt::float2::one(), std::nullopt, duration)),
			std::make_unique<KeyFrameAnimationFactory<winrt::float2>>(Scale(winrt::float2::one(), easingFactory, GetInverseScale(targetScale), std::nullopt, duration))
		};
	}

	TransitionHelper::OpacityAnimations TransitionHelper::AnimateOpacity(
		winrt::TimeSpan duration,
		winrt::Point opacityTransitionProgressKey) const
	{
		auto normalKey = static_cast<float>(opacityTransitionProgressKey.X);
		auto normalKeyForTarget = std::clamp(normalKey - 0.1f, 0.f, 1.f);
		auto reversedKey = 1.f - static_cast<float>(opacityTransitionProgressKey.Y);
		auto reversedKeyForSource = std::clamp(reversedKey + 0.1f, 0.f, 1.f);

		std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>> sourceKeyFrames;
		sourceKeyFrames[GetOpacityTransitionStartKey(normalKey)] = { 1.f, std::nullopt };
		sourceKeyFrames[GetOpacityTransitionEndKey(normalKey)] = { 0.f, GetEasingFunctionFactory(winrt::XamlToolkit::WinUI::Animations::EasingType::Cubic, winrt::EasingMode::EaseIn) };

		std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>> targetKeyFrames;
		targetKeyFrames[GetOpacityTransitionStartKey(normalKeyForTarget)] = { 0.f, std::nullopt };
		targetKeyFrames[GetOpacityTransitionEndKey(normalKeyForTarget)] = { 1.f, GetEasingFunctionFactory(winrt::XamlToolkit::WinUI::Animations::EasingType::Cubic, winrt::EasingMode::EaseOut) };

		std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>> reversedSourceKeyFrames;
		reversedSourceKeyFrames[GetOpacityTransitionStartKey(reversedKeyForSource)] = { 1.f, std::nullopt };
		reversedSourceKeyFrames[GetOpacityTransitionEndKey(reversedKeyForSource)] = { 0.f, GetEasingFunctionFactory(winrt::XamlToolkit::WinUI::Animations::EasingType::Cubic, winrt::EasingMode::EaseIn, true) };

		std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>> reversedTargetKeyFrames;
		reversedTargetKeyFrames[GetOpacityTransitionStartKey(reversedKey)] = { 0.f, std::nullopt };
		reversedTargetKeyFrames[GetOpacityTransitionEndKey(reversedKey)] = { 1.f, GetEasingFunctionFactory(winrt::XamlToolkit::WinUI::Animations::EasingType::Cubic, winrt::EasingMode::EaseOut, true) };

		return
		{
			std::make_unique<KeyFrameAnimationFactory<float>>(Opacity(0.f, std::nullopt, 1.f, std::nullopt, duration, sourceKeyFrames, reversedSourceKeyFrames)),
			std::make_unique<KeyFrameAnimationFactory<float>>(Opacity(1.f, std::nullopt, 0.f, std::nullopt, duration, targetKeyFrames, reversedTargetKeyFrames))
		};
	}

	std::pair<std::optional<std::array<TransitionHelper::ClipScalarAnimationFactory, 4>>, std::optional<std::array<TransitionHelper::ClipScalarAnimationFactory, 4>>> TransitionHelper::AnimateClip(
		winrt::float2 const& sourceActualSize,
		winrt::float2 const& targetActualSize,
		winrt::float2 const& sourceCenterPoint,
		winrt::float2 const& targetCenterPoint,
		winrt::float2 const& sourceTargetScale,
		winrt::TimeSpan duration,
		winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
		winrt::EasingMode easingMode) const
	{
		auto easingFactory = GetEasingFunctionFactory(easingType, easingMode);
		auto sourceToClip = GetTargetClip(
			-sourceCenterPoint, sourceActualSize, sourceCenterPoint, sourceTargetScale, winrt::float2::zero(),
			winrt::Rect{ -targetCenterPoint.x, -targetCenterPoint.y, targetActualSize.x, targetActualSize.y });
		auto targetFromClip = GetTargetClip(
			-targetCenterPoint, targetActualSize, targetCenterPoint, GetInverseScale(sourceTargetScale), winrt::float2::zero(),
			winrt::Rect{ -sourceCenterPoint.x, -sourceCenterPoint.y, sourceActualSize.x, sourceActualSize.y });

		return
		{
			sourceToClip.has_value() ? std::optional{ Clip(sourceToClip.value(), easingFactory, std::nullopt, std::nullopt, duration) } : std::nullopt,
			targetFromClip.has_value() ? std::optional{ Clip(winrt::float4{}, easingFactory, targetFromClip.value(), std::nullopt, duration) } : std::nullopt
		};
	}
}
