#pragma once

#include "Marquee.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct Marquee : MarqueeT<Marquee>
	{
	private:
		static constexpr auto MarqueeContainerPartName = L"MarqueeContainer";
		static constexpr auto Segment1PartName = L"Segment1";
		static constexpr auto Segment2PartName = L"Segment2";
		static constexpr auto MarqueeTransformPartName = L"MarqueeTransform";

		static constexpr auto MarqueeActiveState = L"MarqueeActive";
		static constexpr auto MarqueePausedState = L"MarqueePaused";
		static constexpr auto MarqueeStoppedState = L"MarqueeStopped";

		static constexpr auto DirectionVisualStateGroupName = L"DirectionStateGroup";
		static constexpr auto LeftwardsVisualStateName = L"Leftwards";
		static constexpr auto RightwardsVisualStateName = L"Rightwards";
		static constexpr auto UpwardsVisualStateName = L"Upwards";
		static constexpr auto DownwardsVisualStateName = L"Downwards";

		static constexpr auto BehaviorVisualStateGroupName = L"BehaviorStateGroup";
		static constexpr auto TickerVisualStateName = L"Ticker";
		static constexpr auto LoopingVisualStateName = L"Looping";
		static constexpr auto BouncingVisualStateName = L"Bouncing";

		winrt::Panel _marqueeContainer;
		winrt::ContentPresenter _segment1;
		winrt::ContentPresenter _segment2;
		winrt::TranslateTransform _marqueeTransform;
		winrt::Storyboard _marqueeStoryboard;

		bool _isActive;
		bool _isPaused;

		double _stoppedPosition;
		winrt::DependencyProperty _animationProperty;

	private:
		static std::wstring_view GetVisualStateName(MarqueeDirection direction);

		static std::wstring_view GetVisualStateName(MarqueeBehavior behavior);

	public:
		Marquee();

		void OnApplyTemplate();

		void StartMarquee();

		void RestartMarquee();

		void ResumeMarquee();

		void PauseMarquee();

		void StopMarquee();

		bool AutoPlay() const
		{
			return winrt::unbox_value<bool>(GetValue(AutoPlayProperty()));
		}

		void AutoPlay(bool value)
		{
			SetValue(AutoPlayProperty(), winrt::box_value(value));
		}

		double Speed() const
		{
			return winrt::unbox_value<double>(GetValue(SpeedProperty()));
		}

		void Speed(double value)
		{
			SetValue(SpeedProperty(), winrt::box_value(value));
		}

		RepeatBehavior RepeatBehavior() const
		{
			return winrt::unbox_value<winrt::RepeatBehavior>(GetValue(RepeatBehaviorProperty()));
		}

		void RepeatBehavior(winrt::RepeatBehavior const& value)
		{
			SetValue(RepeatBehaviorProperty(), winrt::box_value(value));
		}

		MarqueeBehavior Behavior() const
		{
			return winrt::unbox_value<MarqueeBehavior>(GetValue(BehaviorProperty()));
		}

		void Behavior(MarqueeBehavior const& value)
		{
			SetValue(BehaviorProperty(), winrt::box_value(value));
		}

		MarqueeDirection Direction() const
		{
			return winrt::unbox_value<MarqueeDirection>(GetValue(DirectionProperty()));
		}

		void Direction(MarqueeDirection const& value)
		{
			SetValue(DirectionProperty(), winrt::box_value(value));
		}

	private:
		void PlayMarquee(bool fromStart = false);

		void UpdateMarquee(bool onTheFly);

		bool UpdateAnimation(winrt::TimeSpan& seekPoint);

		winrt::Storyboard CreateMarqueeStoryboardAnimation(double start, double end, winrt::TimeSpan duration, std::wstring_view targetProperty);

		void ClipMarquee(double width = 0, double height = 0);

		static void BehaviorPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void DirectionPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void PropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		bool IsTicker() const
		{
			return Behavior() == MarqueeBehavior::Ticker;
		}

		bool IsLooping() const
		{
			return Behavior() == MarqueeBehavior::Looping;
		}

		bool IsBouncing() const
		{
			return Behavior() == MarqueeBehavior::Bouncing;
		}

		bool IsDirectionHorizontal() const
		{
			return Direction() == MarqueeDirection::Left || Direction() == MarqueeDirection::Right;
		}

		bool IsDirectionInverse() const
		{
			return Direction() == MarqueeDirection::Up || Direction() == MarqueeDirection::Right;
		}

	public:
		wil::typed_event<class_type, IInspectable> MarqueeStarted;

		wil::typed_event<class_type, IInspectable> MarqueeStopped;

		wil::typed_event<class_type, IInspectable> MarqueeResumed;

		wil::typed_event<class_type, IInspectable> MarqueePaused;

		wil::typed_event<class_type, IInspectable> MarqueeCompleted;

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AutoPlayProperty =
			winrt::DependencyProperty::Register(
				L"AutoPlay", 
				winrt::xaml_typename<bool>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(false)));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> SpeedProperty =
			winrt::DependencyProperty::Register(
				L"Speed", 
				winrt::xaml_typename<double>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(32.0), &Marquee::PropertyChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> RepeatBehaviorProperty =
			winrt::DependencyProperty::Register(
				L"RepeatBehavior", 
				winrt::xaml_typename<winrt::RepeatBehavior>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(winrt::RepeatBehaviorHelper::FromCount(1)), &Marquee::PropertyChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> BehaviorProperty =
			winrt::DependencyProperty::Register(
				L"Behavior",
				winrt::xaml_typename<MarqueeBehavior>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(MarqueeBehavior::Ticker), &Marquee::BehaviorPropertyChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> DirectionProperty =
			winrt::DependencyProperty::Register(
				L"Direction", 
				winrt::xaml_typename<MarqueeDirection>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(MarqueeDirection::Left), &Marquee::DirectionPropertyChanged));

	private:
		winrt::FrameworkElement::Loaded_revoker _loadedRevoker;
		winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;
		winrt::FrameworkElement::SizeChanged_revoker _containerSizeChangedRevoker;
		winrt::FrameworkElement::SizeChanged_revoker _segmentSizeChangedRevoker;
		winrt::Timeline::Completed_revoker _storyBoardCompletedRevoker;

		void Marquee_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void Marquee_Unloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void Container_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		void Segment_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		void StoryBoard_Completed(winrt::IInspectable const& sender, winrt::IInspectable const& e);
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct Marquee : MarqueeT<Marquee, implementation::Marquee>
	{
	};
}
