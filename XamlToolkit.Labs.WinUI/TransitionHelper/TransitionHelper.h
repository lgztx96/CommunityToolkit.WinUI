#pragma once

#include "TransitionHelper.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <wil/resource.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <queue>
#include <ppl.h>
#endif

namespace winrt
{
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Media::Animation;
	using namespace ::winrt::Microsoft::UI::Composition;
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::Foundation::Collections;
	using namespace ::winrt::Windows::Foundation::Numerics;
	using AnimationDirection = ::winrt::Microsoft::UI::Composition::AnimationDirection;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TransitionHelper : TransitionHelperT<TransitionHelper>
	{
		static constexpr float AlmostZero = 0.01f;
		static constexpr std::wstring_view TranslationPropertyName = L"Translation";
		static constexpr std::wstring_view TranslationXYPropertyName = L"Translation.XY";
		static constexpr std::wstring_view ScaleXYPropertyName = L"Scale.XY";

		template<typename T>
		struct AnimatedElements
		{
			std::unordered_map<winrt::hstring, T> ConnectedElements;
			std::unordered_map<winrt::hstring, std::vector<T>> CoordinatedElements;
			std::vector<T> IndependentElements;
		};

		TransitionHelper() = default;

		winrt::XamlToolkit::Labs::WinUI::TransitionConfig DefaultConfig() const;

		winrt::IAsyncAction StartAsync();
		winrt::IAsyncAction StartAsync(bool forceUpdateAnimatedElements);

		winrt::IAsyncAction ReverseAsync();
		winrt::IAsyncAction ReverseAsync(bool forceUpdateAnimatedElements);

		void Stop();
		void Reset(bool toInitialState = true);

#pragma region Attached Property
		static const wil::single_threaded_property<winrt::DependencyProperty> IdProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> IsIndependentProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> IndependentTranslationProperty;
		static const wil::single_threaded_property<winrt::DependencyProperty> CoordinatedTargetProperty;

		static winrt::hstring GetId(winrt::DependencyObject const& obj);
		static void SetId(winrt::DependencyObject const& obj, winrt::hstring const& value);
		static bool GetIsIndependent(winrt::DependencyObject const& obj);
		static void SetIsIndependent(winrt::DependencyObject const& obj, bool value);
		static winrt::IReference<winrt::Point> GetIndependentTranslation(winrt::DependencyObject const& obj);
		static void SetIndependentTranslation(winrt::DependencyObject const& obj, winrt::IReference<winrt::Point> const& value);
		static winrt::hstring GetCoordinatedTarget(winrt::DependencyObject const& obj);
		static void SetCoordinatedTarget(winrt::DependencyObject const& obj, winrt::hstring const& value);
#pragma endregion

#pragma region Properties
		winrt::FrameworkElement Source() const;
		void Source(winrt::FrameworkElement const& value);

		winrt::FrameworkElement Target() const;
		void Target(winrt::FrameworkElement const& value);

		bool IsTargetState() const { return _isTargetState; }

		void IsTargetState(bool isTargetState) { _isTargetState = isTargetState; }

		bool IsAnimating() const { return _currentAnimationCancellationTokenSource.has_value() && _currentAnimationGroupController.has_value(); }

		wil::single_threaded_rw_property<winrt::IVector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig>> Configs{ winrt::single_threaded_vector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig>() };
		wil::single_threaded_rw_property<bool> IsHitTestVisibleWhenAnimating{};
		wil::single_threaded_rw_property<enum VisualStateToggleMethod> SourceToggleMethod{ VisualStateToggleMethod::ByVisibility };
		wil::single_threaded_rw_property<enum VisualStateToggleMethod> TargetToggleMethod{ VisualStateToggleMethod::ByVisibility };
		wil::single_threaded_rw_property<winrt::TimeSpan> Duration{ std::chrono::milliseconds(600) };
		wil::single_threaded_rw_property<winrt::TimeSpan> ReverseDuration{ std::chrono::milliseconds(600) };
		wil::single_threaded_rw_property<bool> InverseEasingFunctionWhenReversing{ true };
		wil::single_threaded_rw_property<winrt::TimeSpan> IndependentElementShowDuration{ std::chrono::milliseconds(200) };
		wil::single_threaded_rw_property<winrt::TimeSpan> IndependentElementShowDelay{ std::chrono::milliseconds(300) };
		wil::single_threaded_rw_property<winrt::TimeSpan> IndependentElementShowInterval{ std::chrono::milliseconds(50) };
		wil::single_threaded_rw_property<winrt::TimeSpan> IndependentElementHideDuration{ std::chrono::milliseconds(100) };
		wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Animations::EasingType> DefaultEasingType{ winrt::XamlToolkit::WinUI::Animations::EasingType::Default };
		wil::single_threaded_rw_property<winrt::EasingMode> DefaultEasingMode{ winrt::EasingMode::EaseInOut };
		wil::single_threaded_rw_property<winrt::Point> DefaultIndependentTranslation{ winrt::Point{ 0, 20 } };
		wil::single_threaded_rw_property<winrt::Point> DefaultOpacityTransitionProgressKey{ winrt::Point{ .3f, .3f } };
		wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Animations::EasingType> IndependentElementEasingType{ winrt::XamlToolkit::WinUI::Animations::EasingType::Default };
		wil::single_threaded_rw_property<winrt::EasingMode> IndependentElementEasingMode{ winrt::EasingMode::EaseInOut };
#pragma endregion

	private:
#pragma region Animation
		struct EasingFunctionFactory
		{
			winrt::XamlToolkit::WinUI::Animations::EasingType Type = winrt::XamlToolkit::WinUI::Animations::EasingType::Default;
			winrt::EasingMode Mode = winrt::EasingMode::EaseInOut;
			bool Inverse = false;
			winrt::CompositionEasingFunction GetEasingFunction(winrt::Compositor const& compositor, bool inverse) const;
		};

		struct IKeyFrameCompositionAnimationFactory
		{
			virtual ~IKeyFrameCompositionAnimationFactory() = default;
			virtual winrt::KeyFrameAnimation GetAnimation(
				winrt::Compositor const& compositor,
				bool reversed,
				bool useReversedKeyframes,
				bool inverseEasingFunction,
				winrt::CompositionObject& target) const = 0;
		};

		template<typename T>
		struct KeyFrameAnimationFactory : IKeyFrameCompositionAnimationFactory
		{
			winrt::hstring Property;
			T To;
			std::optional<T> From;
			std::optional<winrt::TimeSpan> Delay;
			std::optional<winrt::TimeSpan> Duration;
			std::optional<EasingFunctionFactory> EasingFactory;
			std::optional<std::unordered_map<float, std::pair<T, std::optional<EasingFunctionFactory>>>> NormalizedKeyFrames;
			std::optional<std::unordered_map<float, std::pair<T, std::optional<EasingFunctionFactory>>>> ReversedNormalizedKeyFrames;

			KeyFrameAnimationFactory() = default;
			KeyFrameAnimationFactory(
				winrt::hstring property, T to, std::optional<T> from, std::optional<winrt::TimeSpan> delay,
				std::optional<winrt::TimeSpan> duration, std::optional<EasingFunctionFactory> easingFactory,
				std::optional<std::unordered_map<float, std::pair<T, std::optional<EasingFunctionFactory>>>> nkf = std::nullopt,
				std::optional<std::unordered_map<float, std::pair<T, std::optional<EasingFunctionFactory>>>> rnkf = std::nullopt)
				: Property(std::move(property)), To(to), From(from), Delay(delay), Duration(duration)
				, EasingFactory(easingFactory), NormalizedKeyFrames(nkf), ReversedNormalizedKeyFrames(rnkf) {}

			winrt::KeyFrameAnimation GetAnimation(winrt::Compositor const& compositor, bool reversed, bool useReversedKeyframes, bool inverseEasingFunction, winrt::CompositionObject& target) const override;
		};

		struct ClipScalarAnimationFactory : IKeyFrameCompositionAnimationFactory
		{
			winrt::hstring Property;
			float To;
			std::optional<float> From;
			std::optional<winrt::TimeSpan> Delay;
			std::optional<winrt::TimeSpan> Duration;
			std::optional<EasingFunctionFactory> EasingFactory;

			ClipScalarAnimationFactory() = default;
			ClipScalarAnimationFactory(
				winrt::hstring property, float to, std::optional<float> from, std::optional<winrt::TimeSpan> delay,
				std::optional<winrt::TimeSpan> duration, std::optional<EasingFunctionFactory> easingFactory)
				: Property(std::move(property)), To(to), From(from), Delay(delay), Duration(duration), EasingFactory(easingFactory) {}

			winrt::KeyFrameAnimation GetAnimation(winrt::Compositor const& compositor, bool reversed, bool useReversedKeyframes, bool inverseEasingFunction, winrt::CompositionObject& target) const override;
		};

		using AnimationFactory = std::unique_ptr<IKeyFrameCompositionAnimationFactory>;
		using TranslationScaleAnimations = std::tuple<AnimationFactory, AnimationFactory, winrt::float2>;
		using ScaleAnimations = std::pair<AnimationFactory, AnimationFactory>;
		using OpacityAnimations = std::pair<AnimationFactory, AnimationFactory>;

		struct KeyFrameAnimationGroupController
		{
			std::optional<float> LastStopProgress;
			std::optional<winrt::AnimationDirection> CurrentDirection;

			void AddAnimationFor(winrt::UIElement const& target, AnimationFactory factory);

			template<typename... T>
			void AddAnimationGroupFor(winrt::UIElement const& target, T&&... factories)
			{
				(AddAnimationFor(target, std::forward<T>(factories)), ...);
			}

			winrt::IAsyncAction StartAsync(concurrency::cancellation_token token, std::optional<winrt::TimeSpan> duration);
			winrt::IAsyncAction ReverseAsync(concurrency::cancellation_token token, bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration);

		private:
			winrt::IAsyncAction AnimateAsync(bool reversed, bool useReversedKeyframes, concurrency::cancellation_token token, bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration, std::optional<float> startProgress);
			winrt::IAsyncAction StartAnimation(winrt::UIElement const& element, bool reversed, bool useReversedKeyframes, bool inverseEasingFunction, std::optional<winrt::TimeSpan> duration, std::optional<float> startProgress, std::vector<std::pair<winrt::CompositionObject, winrt::hstring>>& compositionAnimations);

			struct UIElementHash { size_t operator()(winrt::UIElement const& e) const { return std::hash<void*>{}(winrt::get_abi(e)); } };
			struct UIElementEq { bool operator()(winrt::UIElement const& a, winrt::UIElement const& b) const { return winrt::get_abi(a) == winrt::get_abi(b); } };
			std::unordered_map<winrt::UIElement, std::vector<AnimationFactory>, UIElementHash, UIElementEq> _animationFactories;
			bool _lastInverseEasingFunction = false;
			bool _lastStartInNormalDirection = true;
		};

		static winrt::CompositionEasingFunction GetEasingFunction(
			winrt::Compositor const& compositor,
			winrt::XamlToolkit::WinUI::Animations::EasingType type,
			winrt::EasingMode mode,
			bool inverse = false);

		KeyFrameAnimationFactory<float> Opacity(
			float to,
			std::optional<EasingFunctionFactory> easingFunctionFactory,
			std::optional<float> from = std::nullopt,
			std::optional<winrt::TimeSpan> delay = std::nullopt,
			std::optional<winrt::TimeSpan> duration = std::nullopt,
			std::optional<std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>>> normalizedKeyFrames = std::nullopt,
			std::optional<std::unordered_map<float, std::pair<float, std::optional<EasingFunctionFactory>>>> reversedNormalizedKeyFrames = std::nullopt) const;

		KeyFrameAnimationFactory<winrt::float2> Translation(
			winrt::float2 to,
			std::optional<EasingFunctionFactory> easingFunctionFactory,
			std::optional<winrt::float2> from = std::nullopt,
			std::optional<winrt::TimeSpan> delay = std::nullopt,
			std::optional<winrt::TimeSpan> duration = std::nullopt) const;

		KeyFrameAnimationFactory<winrt::float2> Scale(
			winrt::float2 to,
			std::optional<EasingFunctionFactory> easingFunctionFactory,
			std::optional<winrt::float2> from = std::nullopt,
			std::optional<winrt::TimeSpan> delay = std::nullopt,
			std::optional<winrt::TimeSpan> duration = std::nullopt) const;

		std::array<ClipScalarAnimationFactory, 4> Clip(
			winrt::float4 to,
			std::optional<EasingFunctionFactory> easingFunctionFactory,
			std::optional<winrt::float4> from = std::nullopt,
			std::optional<winrt::TimeSpan> delay = std::nullopt,
			std::optional<winrt::TimeSpan> duration = std::nullopt) const;
#pragma endregion

#pragma region Logic
		void RestoreState(bool isTargetState);
		winrt::IAsyncAction InitControlsStateAsync(bool forceUpdateAnimatedElements = false);
		winrt::IAsyncAction InitControlStateAsync(winrt::FrameworkElement element);
		winrt::IAsyncAction AnimateControlsAsync(bool reversed, bool forceUpdateAnimatedElements);
		winrt::IAsyncAction AnimateControlsImpAsync(winrt::TimeSpan duration, bool reversed, concurrency::cancellation_token token);

		void BuildConnectedAnimationController(
			KeyFrameAnimationGroupController& controller,
			winrt::UIElement const& source,
			winrt::UIElement const& target,
			std::span<winrt::UIElement> sourceCoordinated,
			std::span<winrt::UIElement> targetCoordinated,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::Labs::WinUI::TransitionConfig const& config);

		winrt::IAsyncAction AnimateIndependentElements(
			std::span<winrt::UIElement> elements,
			bool isShow,
			concurrency::cancellation_token token,
			std::optional<winrt::TimeSpan> startTime,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		TranslationScaleAnimations AnimateTranslation(
			winrt::UIElement const& source,
			winrt::UIElement const& target,
			winrt::float2 const& sourceCenterPoint,
			winrt::float2 const& targetCenterPoint,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		TranslationScaleAnimations AnimateScale(
			winrt::float2 const& sourceActualSize,
			winrt::float2 const& targetActualSize,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		TranslationScaleAnimations AnimateScaleX(
			winrt::float2 const& sourceActualSize,
			winrt::float2 const& targetActualSize,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		TranslationScaleAnimations AnimateScaleY(
			winrt::float2 const& sourceActualSize,
			winrt::float2 const& targetActualSize,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		TranslationScaleAnimations AnimateScaleWithScaleCalculator(
			winrt::UIElement const& source,
			winrt::UIElement const& target,
			winrt::XamlToolkit::Labs::WinUI::IScalingCalculator const& calculator,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		ScaleAnimations AnimateScaleImp(
			winrt::float2 targetScale,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;

		OpacityAnimations AnimateOpacity(winrt::TimeSpan duration, winrt::Point opacityTransitionProgressKey) const;

		std::pair<std::optional<std::array<ClipScalarAnimationFactory, 4>>, std::optional<std::array<ClipScalarAnimationFactory, 4>>> AnimateClip(
			winrt::float2 const& sourceActualSize,
			winrt::float2 const& targetActualSize,
			winrt::float2 const& sourceCenterPoint,
			winrt::float2 const& targetCenterPoint,
			winrt::float2 const& sourceTargetScale,
			winrt::TimeSpan duration,
			winrt::XamlToolkit::WinUI::Animations::EasingType easingType,
			winrt::EasingMode easingMode) const;
#pragma endregion

#pragma region Helpers
		static AnimatedElements<winrt::UIElement> GetAnimatedElements(winrt::DependencyObject const& parent);

		static void ToggleVisualState(winrt::UIElement const& element, VisualStateToggleMethod method, bool visible);

		static void RestoreAnimatedElements(AnimatedElements<winrt::UIElement> const& elements);

		static void RestoreElement(winrt::UIElement const& animatedElement);

		static void IsNotNullAndIsInVisualTree(winrt::FrameworkElement const& target, std::wstring_view name);

		static winrt::IAsyncAction UpdateControlLayout(winrt::FrameworkElement const& target);

		static winrt::float2 GetInverseScale(winrt::float2 const& scale);

		static winrt::float4 GetFixedThickness(float left, float top, float right, float bottom, float defaultValue = 0);

		static winrt::Rect GetTransformedBounds(winrt::float2 initialLocation, winrt::float2 initialSize, winrt::float2 centerPoint, winrt::float2 targetScale);

		static std::optional<winrt::float4> GetTargetClip(
			winrt::float2 const& initialLocation,
			winrt::float2 const& initialSize,
			winrt::float2 const& centerPoint,
			winrt::float2 const& targetScale,
			winrt::float2 const& translation,
			winrt::Rect const& targetParentBounds);

		static EasingFunctionFactory GetEasingFunctionFactory(
			winrt::XamlToolkit::WinUI::Animations::EasingType type = winrt::XamlToolkit::WinUI::Animations::EasingType::Default,
			winrt::EasingMode mode = winrt::EasingMode::EaseInOut,
			bool inverse = false);

		static float GetOpacityTransitionStartKey(float normalizedKey, float halfTransitionNormalizedDuration = 0.1f);

		static float GetOpacityTransitionEndKey(float normalizedKey, float halfTransitionNormalizedDuration = 0.1f);
#pragma endregion

	private:
		const AnimatedElements<UIElement>& SourceAnimatedElements()
		{
			if (!_sourceAnimatedElements)
			{
				_sourceAnimatedElements.emplace(GetAnimatedElements(Source()));
			}

			return *_sourceAnimatedElements;
		}

		const AnimatedElements<UIElement>& TargetAnimatedElements()
		{
			if (!_targetAnimatedElements)
			{
				_targetAnimatedElements.emplace(GetAnimatedElements(Target()));
			}

			return *_targetAnimatedElements;
		}

		winrt::FrameworkElement _source{ nullptr };
		winrt::FrameworkElement _target{ nullptr };
		int32_t _sourceZIndex{ -1 };
		int32_t _targetZIndex{ -1 };
		std::optional<AnimatedElements<winrt::UIElement>> _sourceAnimatedElements;
		std::optional<AnimatedElements<winrt::UIElement>> _targetAnimatedElements;

		std::optional<concurrency::cancellation_token_source> _currentAnimationCancellationTokenSource;
		std::optional<KeyFrameAnimationGroupController> _currentAnimationGroupController;
		bool _isTargetState{};
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TransitionHelper : TransitionHelperT<TransitionHelper, implementation::TransitionHelper> {};
}
