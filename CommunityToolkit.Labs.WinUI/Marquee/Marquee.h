#pragma once

#include "Marquee.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;
	using namespace winrt::Microsoft::UI::Xaml::Media;
	using namespace winrt::Microsoft::UI::Xaml::Media::Animation;
	using namespace winrt::Windows::Foundation;

	struct Marquee : MarqueeT<Marquee>
	{
	private:
		static constexpr auto MarqueeContainerPartName = L"MarqueeContainer";
		static constexpr auto Segment1PartName = L"Segment1";
		static constexpr auto Segment2PartName = L"Segment2";
		static constexpr auto MarqueeTransformPartName = L"MarqueeTransform";

		static constexpr auto MarqueeActiveState = L"MarqueeActive";
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

		Panel _marqueeContainer;
		ContentPresenter _segment1;
		ContentPresenter _segment2;
		TranslateTransform _marqueeTransform;
		Storyboard _marqueeStoryboard;

		bool _isActive;

	private:
		static std::wstring_view GetVisualStateName(MarqueeDirection direction);

		static std::wstring_view GetVisualStateName(MarqueeBehavior behavior);

	public:
		Marquee();

		void OnApplyTemplate();

		void StartMarquee();

		void StopMarquee();

		double Speed()
		{
			return winrt::unbox_value<double>(GetValue(SpeedProperty()));
		}

		void Speed(double value)
		{
			SetValue(SpeedProperty(), winrt::box_value(value));
		}

		RepeatBehavior RepeatBehavior()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Animation::RepeatBehavior>(GetValue(RepeatBehaviorProperty()));
		}

		void RepeatBehavior(winrt::Microsoft::UI::Xaml::Media::Animation::RepeatBehavior const& value)
		{
			SetValue(RepeatBehaviorProperty(), winrt::box_value(value));
		}

		MarqueeBehavior Behavior()
		{
			return winrt::unbox_value<MarqueeBehavior>(GetValue(BehaviorProperty()));
		}

		void Behavior(MarqueeBehavior const& value)
		{
			SetValue(BehaviorProperty(), winrt::box_value(value));
		}

		MarqueeDirection Direction()
		{
			return winrt::unbox_value<MarqueeDirection>(GetValue(DirectionProperty()));
		}

		void Direction(MarqueeDirection const& value)
		{
			SetValue(DirectionProperty(), winrt::box_value(value));
		}

	private:
		void StopMarquee(bool initialState);

		bool UpdateAnimation(bool resume = true);

		Storyboard CreateMarqueeStoryboardAnimation(double start, double end, TimeSpan duration, std::wstring_view targetProperty);

		static void BehaviorPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e);

		static void DirectionPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e);

		static void PropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e);

		bool IsTicker()
		{
			return Behavior() == MarqueeBehavior::Ticker;
		}

		bool IsLooping()
		{
			return Behavior() == MarqueeBehavior::Looping;
		}

		bool IsBouncing()
		{
			return Behavior() == MarqueeBehavior::Bouncing;
		}

		bool IsDirectionHorizontal()
		{
			return Direction() == MarqueeDirection::Left || Direction() == MarqueeDirection::Right;
		}

		bool IsDirectionInverse()
		{
			return Direction() == MarqueeDirection::Up || Direction() == MarqueeDirection::Right;
		}

	public:
		wil::typed_event<class_type, IInspectable> MarqueeBegan;

		wil::typed_event<class_type, IInspectable> MarqueeStopped;

		wil::typed_event<class_type, IInspectable> MarqueeCompleted;

		static inline const wil::single_threaded_property<DependencyProperty> SpeedProperty =
			DependencyProperty::Register(L"Speed", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(32.0), &Marquee::PropertyChanged));

		static inline const wil::single_threaded_property<DependencyProperty> RepeatBehaviorProperty =
			DependencyProperty::Register(L"RepeatBehavior", winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::Animation::RepeatBehavior>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(winrt::Microsoft::UI::Xaml::Media::Animation::RepeatBehaviorHelper::FromCount(1)), &Marquee::PropertyChanged));

		static inline const wil::single_threaded_property<DependencyProperty> BehaviorProperty =
			DependencyProperty::Register(L"Behavior", winrt::xaml_typename<MarqueeBehavior>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0), &Marquee::BehaviorPropertyChanged));

		static inline const wil::single_threaded_property<DependencyProperty> DirectionProperty =
			DependencyProperty::Register(L"Direction", winrt::xaml_typename<MarqueeDirection>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(MarqueeDirection::Left), &Marquee::DirectionPropertyChanged));

	private:

		winrt::event_token _loadedToken;
		winrt::event_token _unloadedToken;
		winrt::event_token _containerSizeChangedToken;
		winrt::event_token _storyBoardCompletedToken;

		void Marquee_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void Marquee_Unloaded(IInspectable const& sender, RoutedEventArgs const& e);

		void Container_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void Segment_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void StoryBoard_Completed(IInspectable const& sender, IInspectable const& e);
	};
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
	struct Marquee : MarqueeT<Marquee, implementation::Marquee>
	{
	};
}
