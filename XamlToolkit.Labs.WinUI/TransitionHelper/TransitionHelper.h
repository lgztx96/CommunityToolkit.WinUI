#pragma once

#include "TransitionHelper.g.h"
#include <unordered_map>
#include <vector>
#include <optional>

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TransitionHelper : TransitionHelperT<TransitionHelper>
	{
		TransitionHelper();

		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IdProperty;
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsIndependentProperty;
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IndependentTranslationProperty;
		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CoordinatedTargetProperty;
		static winrt::hstring GetId(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
		static void SetId(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::hstring const& value);
		static bool GetIsIndependent(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
		static void SetIsIndependent(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, bool value);
		static winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::Point> GetIndependentTranslation(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
		static void SetIndependentTranslation(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::Point> const& value);
		static winrt::hstring GetCoordinatedTarget(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
		static void SetCoordinatedTarget(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::hstring const& value);

		winrt::Microsoft::UI::Xaml::FrameworkElement Source() const
		{
			return m_source;
		}
		void Source(winrt::Microsoft::UI::Xaml::FrameworkElement const& value);
		winrt::Microsoft::UI::Xaml::FrameworkElement Target() const
		{
			return m_target;
		}
		void Target(winrt::Microsoft::UI::Xaml::FrameworkElement const& value);
		winrt::Windows::Foundation::Collections::IVector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig> Configs() const
		{
			return m_configs;
		}
		void Configs(winrt::Windows::Foundation::Collections::IVector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig> const& value)
		{
			m_configs = value;
		}
		bool IsTargetState() const
		{
			return m_isTargetState;
		}
		bool IsAnimating() const
		{
			return m_isAnimating;
		}
		bool IsHitTestVisibleWhenAnimating() const
		{
			return m_hitTestVisible;
		}
		void IsHitTestVisibleWhenAnimating(bool value)
		{
			m_hitTestVisible = value;
		}
		VisualStateToggleMethod SourceToggleMethod() const
		{
			return m_sourceToggle;
		}
		void SourceToggleMethod(enum VisualStateToggleMethod value)
		{
			m_sourceToggle = value;
		}
		VisualStateToggleMethod TargetToggleMethod() const
		{
			return m_targetToggle;
		}
		void TargetToggleMethod(enum VisualStateToggleMethod value)
		{
			m_targetToggle = value;
		}
		winrt::Windows::Foundation::TimeSpan Duration() const
		{
			return m_duration;
		}
		void Duration(winrt::Windows::Foundation::TimeSpan value)
		{
			m_duration = value;
		}
		winrt::Windows::Foundation::TimeSpan ReverseDuration() const
		{
			return m_reverseDuration;
		}
		void ReverseDuration(winrt::Windows::Foundation::TimeSpan value)
		{
			m_reverseDuration = value;
		}
		bool InverseEasingFunctionWhenReversing() const
		{
			return m_inverseEasing;
		}
		void InverseEasingFunctionWhenReversing(bool value)
		{
			m_inverseEasing = value;
		}
		winrt::Windows::Foundation::TimeSpan IndependentElementShowDuration() const
		{
			return m_showDuration;
		}
		void IndependentElementShowDuration(winrt::Windows::Foundation::TimeSpan value)
		{
			m_showDuration = value;
		}
		winrt::Windows::Foundation::TimeSpan IndependentElementShowDelay() const
		{
			return m_showDelay;
		}
		void IndependentElementShowDelay(winrt::Windows::Foundation::TimeSpan value)
		{
			m_showDelay = value;
		}
		winrt::Windows::Foundation::TimeSpan IndependentElementShowInterval() const
		{
			return m_showInterval;
		}
		void IndependentElementShowInterval(winrt::Windows::Foundation::TimeSpan value)
		{
			m_showInterval = value;
		}
		winrt::Windows::Foundation::TimeSpan IndependentElementHideDuration() const
		{
			return m_hideDuration;
		}
		void IndependentElementHideDuration(winrt::Windows::Foundation::TimeSpan value)
		{
			m_hideDuration = value;
		}
		winrt::XamlToolkit::WinUI::Animations::EasingType DefaultEasingType() const
		{
			return m_defaultEasingType;
		}
		void DefaultEasingType(winrt::XamlToolkit::WinUI::Animations::EasingType value)
		{
			m_defaultEasingType = value;
		}
		winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode DefaultEasingMode() const
		{
			return m_defaultEasingMode;
		}
		void DefaultEasingMode(winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode value)
		{
			m_defaultEasingMode = value;
		}
		winrt::Windows::Foundation::Point DefaultIndependentTranslation() const
		{
			return m_defaultTranslation;
		}
		void DefaultIndependentTranslation(winrt::Windows::Foundation::Point value)
		{
			m_defaultTranslation = value;
		}
		winrt::Windows::Foundation::Point DefaultOpacityTransitionProgressKey() const
		{
			return m_defaultOpacityKey;
		}
		void DefaultOpacityTransitionProgressKey(winrt::Windows::Foundation::Point value)
		{
			m_defaultOpacityKey = value;
		}
		winrt::XamlToolkit::WinUI::Animations::EasingType IndependentElementEasingType() const
		{
			return m_independentEasingType;
		}
		void IndependentElementEasingType(winrt::XamlToolkit::WinUI::Animations::EasingType value)
		{
			m_independentEasingType = value;
		}
		winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode IndependentElementEasingMode() const
		{
			return m_independentEasingMode;
		}
		void IndependentElementEasingMode(winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode value)
		{
			m_independentEasingMode = value;
		}

		winrt::Windows::Foundation::IAsyncAction StartAsync();
		winrt::Windows::Foundation::IAsyncAction StartAsync(bool forceUpdateAnimatedElements);
		winrt::Windows::Foundation::IAsyncAction ReverseAsync();
		winrt::Windows::Foundation::IAsyncAction ReverseAsync(bool forceUpdateAnimatedElements);
		void Stop();
		void Reset(bool toInitialState = true);

	private:
		struct ElementSet
		{
			std::unordered_map<winrt::hstring, winrt::Microsoft::UI::Xaml::UIElement> connected;
			std::unordered_map<winrt::hstring, std::vector<winrt::Microsoft::UI::Xaml::UIElement>> coordinated;
			std::vector<winrt::Microsoft::UI::Xaml::UIElement> independent;
		};
		ElementSet FindElements(winrt::Microsoft::UI::Xaml::DependencyObject const& root) const;
		void FindElementsRecursive(winrt::Microsoft::UI::Xaml::DependencyObject const& root, ElementSet& result) const;
		void RestoreElementSet(ElementSet const& elements) const;
		void InitializeControl(winrt::Microsoft::UI::Xaml::FrameworkElement const& element) const;
		winrt::Windows::Foundation::IAsyncAction Animate(bool reverse, bool forceUpdateAnimatedElements);
		static void StopVisual(winrt::Microsoft::UI::Xaml::UIElement const& element);
		static winrt::Microsoft::UI::Composition::CompositionEasingFunction Easing(winrt::Microsoft::UI::Composition::Compositor const& compositor, winrt::XamlToolkit::WinUI::Animations::EasingType type, winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode mode);
		void SetVisible(winrt::Microsoft::UI::Xaml::UIElement const& element, enum VisualStateToggleMethod method, bool visible) const;
		void AnimatePair(
			winrt::Microsoft::UI::Xaml::UIElement const& source,
			winrt::Microsoft::UI::Xaml::UIElement const& target,
			std::vector<winrt::Microsoft::UI::Xaml::UIElement> const& sourceCoordinated,
			std::vector<winrt::Microsoft::UI::Xaml::UIElement> const& targetCoordinated,
			winrt::XamlToolkit::Labs::WinUI::TransitionConfig const& config,
			bool reverse,
			winrt::Windows::Foundation::TimeSpan duration);
		void AnimateIndependent(winrt::Microsoft::UI::Xaml::UIElement const& element, bool show, winrt::Windows::Foundation::TimeSpan delay) const;

		winrt::Microsoft::UI::Xaml::FrameworkElement m_source{ nullptr }, m_target{ nullptr };
		int32_t m_sourceZIndex{}, m_targetZIndex{};
		ElementSet m_sourceElements, m_targetElements;
		bool m_sourceElementsValid{}, m_targetElementsValid{};
		uint64_t m_animationGeneration{};
		winrt::Windows::Foundation::Collections::IVector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig> m_configs{ winrt::single_threaded_vector<winrt::XamlToolkit::Labs::WinUI::TransitionConfig>() };
		bool m_isTargetState{}, m_isAnimating{}, m_hitTestVisible{};
		enum VisualStateToggleMethod m_sourceToggle
		{
			VisualStateToggleMethod::ByVisibility
		}, m_targetToggle{ VisualStateToggleMethod::ByVisibility };
		winrt::Windows::Foundation::TimeSpan m_duration{ std::chrono::milliseconds(600) }, m_reverseDuration{ std::chrono::milliseconds(600) };
		bool m_inverseEasing{ true };
		winrt::Windows::Foundation::TimeSpan m_showDuration{ std::chrono::milliseconds(200) }, m_showDelay{ std::chrono::milliseconds(300) }, m_showInterval{ std::chrono::milliseconds(50) }, m_hideDuration{ std::chrono::milliseconds(100) };
		winrt::XamlToolkit::WinUI::Animations::EasingType m_defaultEasingType{ winrt::XamlToolkit::WinUI::Animations::EasingType::Default }, m_independentEasingType{ winrt::XamlToolkit::WinUI::Animations::EasingType::Default };
		winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode m_defaultEasingMode{ winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseInOut }, m_independentEasingMode{ winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseInOut };
		winrt::Windows::Foundation::Point m_defaultTranslation{ 0, 20 }, m_defaultOpacityKey{ .3f, .3f };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TransitionHelper : TransitionHelperT<TransitionHelper, implementation::TransitionHelper>
	{
	};
}
