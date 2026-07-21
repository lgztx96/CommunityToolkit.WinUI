#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <chrono>
#include <cmath>
#endif
#include "Marquee.h"
#if __has_include("Marquee.g.cpp")
#include "Marquee.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	Marquee::Marquee()
		: _marqueeContainer(nullptr)
		, _segment1(nullptr)
		, _segment2(nullptr)
		, _marqueeTransform(nullptr)
		, _marqueeStoryboard(nullptr)
		, _isActive(false)
		, _isPaused(false)
		, _stoppedPosition(0.0)
		, _animationProperty(nullptr)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void Marquee::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		// Explicit casting throws early when parts are missing from the template
		_marqueeContainer = GetTemplateChild(MarqueeContainerPartName).try_as<winrt::Panel>();
		_segment1 = GetTemplateChild(Segment1PartName).try_as<winrt::ContentPresenter>();
		_segment2 = GetTemplateChild(Segment2PartName).try_as<winrt::ContentPresenter>();
		_marqueeTransform = GetTemplateChild(MarqueeTransformPartName).try_as<winrt::TranslateTransform>();

		// Swapping tabs in TabView caused errors where the control would unload and never reattach events.
		// Hotfix: Track the loaded event. This should be fine because the GC will handle detaching the Loaded
		// event on disposal. However, more research is required
		_loadedRevoker = Loaded(winrt::auto_revoke, { this, &Marquee::Marquee_Loaded });

		winrt::VisualStateManager::GoToState(*this, GetVisualStateName(Direction()), false);
		winrt::VisualStateManager::GoToState(*this, GetVisualStateName(Behavior()), false);
	}

	std::wstring_view Marquee::GetVisualStateName(MarqueeDirection direction)
	{
		switch (direction)
		{
		case MarqueeDirection::Left:
			return LeftwardsVisualStateName;
		case MarqueeDirection::Right:
			return RightwardsVisualStateName;
		case MarqueeDirection::Up:
			return UpwardsVisualStateName;
		case MarqueeDirection::Down:
			return DownwardsVisualStateName;
		default:
			return LeftwardsVisualStateName;
		}
	}

	std::wstring_view Marquee::GetVisualStateName(MarqueeBehavior behavior)
	{
		switch (behavior)
		{
		case MarqueeBehavior::Ticker:
			return TickerVisualStateName;
		case MarqueeBehavior::Looping:
			return LoopingVisualStateName;
		case MarqueeBehavior::Bouncing:
			return BouncingVisualStateName;
		default:
			return TickerVisualStateName;
		}
	}

	void Marquee::StartMarquee()
	{
		PlayMarquee(false);
	}

	void Marquee::RestartMarquee()
	{
		PlayMarquee(true);
	}

	void Marquee::ResumeMarquee()
	{
		// If not paused or not active, do nothing
		if (!_isPaused || !_isActive)
			return;

		// Resume the storyboard
		_isPaused = false;
		if (_marqueeStoryboard) _marqueeStoryboard.Resume();

		// Apply state transitions
		winrt::VisualStateManager::GoToState(*this, MarqueeActiveState, false);
		MarqueeResumed.invoke(*this, nullptr);
	}

	void Marquee::PauseMarquee()
	{
		// Log initial paused status
		bool wasPaused = _isPaused;

		// Ensure paused status
		if (_marqueeStoryboard) _marqueeStoryboard.Pause();
		_isPaused = true;

		if (!wasPaused)
		{
			// Apply state transitions
			winrt::VisualStateManager::GoToState(*this, MarqueePausedState, false);
			MarqueePaused.invoke(*this, nullptr);
		}
	}

	void Marquee::StopMarquee()
	{
		bool wasStopped = !_isActive;

		// Ensure stopped status
		if (_marqueeStoryboard) _marqueeStoryboard.Stop();
		_isActive = false;
		_isPaused = false;

		// Set the transform to the stopped position if provided.
		if (_animationProperty)
		{
			_marqueeTransform.SetValue(_animationProperty, winrt::box_value(_stoppedPosition));
		}

		if (!wasStopped)
		{
			// Apply state transitions
			winrt::VisualStateManager::GoToState(*this, MarqueeStoppedState, false);
			MarqueeStopped.invoke(*this, nullptr);
		}
	}

	void Marquee::PlayMarquee(bool fromStart)
	{
		// Resume if paused and not playing from start
		if (!fromStart && _isPaused && _isActive)
		{
			ResumeMarquee();
			return;
		}

		// Do nothing if storyboard is null or already playing and not from start.
		if (_marqueeStoryboard == nullptr || (_isActive && !fromStart))
			return;

		bool wasActive = _isActive;

		// Stop the storboard if it is already active and playing from start
		if (fromStart)
		{
			_marqueeStoryboard.Stop();
		}

		// Start the storyboard
		_marqueeStoryboard.Begin();

		// Update the status variables
		_isActive = true;
		_isPaused = false;

		if (!wasActive)
		{
			// Apply state transitions
			winrt::VisualStateManager::GoToState(*this, MarqueeActiveState, false);
			MarqueeStarted.invoke(*this, nullptr);
		}
	}

	void Marquee::UpdateMarquee(bool onTheFly)
	{
		// Check for crucial template parts
		if (_marqueeTransform == nullptr)
			return;

		// If the update cannot be made on the fly,
		// stop the marquee and reset the transform
		if (!onTheFly)
		{
			StopMarquee();
			_marqueeTransform.X(0);
			_marqueeTransform.Y(0);
		}

		// Apply the animation update
		winrt::TimeSpan seek;
		bool hasAnimation = UpdateAnimation(seek);

		// If updating on the fly, and there is an animation,
		// seek to the correct position
		if (onTheFly && hasAnimation && _isActive)
		{
			if (_marqueeStoryboard)
			{
				_marqueeStoryboard.Begin();
				_marqueeStoryboard.Seek(seek);
			}

			// Restore paused state if necessary
			if (_isPaused)
			{
				PauseMarquee();
			}
		}
	}

	bool Marquee::UpdateAnimation(winrt::TimeSpan& seekPoint)
	{
		seekPoint = winrt::TimeSpan{ 0 };

		// Check for crucial template parts
		if (_marqueeContainer == nullptr ||
			_marqueeTransform == nullptr ||
			_segment1 == nullptr ||
			_segment2 == nullptr)
		{
			// Crucial template parts are missing!
			// This can happen during initialization of certain properties.
			// Gracefully return when this happens. Proper checks for these template parts happen in OnApplyTemplate.
			return false;
		}

		// Unbind events from the old storyboard
		if (_marqueeStoryboard)
		{
			_storyBoardCompletedRevoker.revoke();
		}

		// Get the size of the container and segment, based on the orientation.
		// Also track the property to adjust, also based on the orientation.
		double containerSize;
		double segmentSize;
		double value;
		winrt::DependencyProperty dp{ nullptr };
		std::wstring_view targetProperty;

		if (IsDirectionHorizontal())
		{
			// The direction is horizontal, so the sizes, value, and properties
			// are defined by width and X coordinates.
			containerSize = _marqueeContainer.ActualWidth();
			segmentSize = _segment1.ActualWidth();
			value = _marqueeTransform.X();
			dp = winrt::TranslateTransform::XProperty();
			targetProperty = L"(TranslateTransform.X)";
		}
		else
		{
			// The direction is vertical, so the sizes, value, and properties
			// are defined by height and Y coordinates.
			containerSize = _marqueeContainer.ActualHeight();
			segmentSize = _segment1.ActualHeight();
			value = _marqueeTransform.Y();
			dp = winrt::TranslateTransform::YProperty();
			targetProperty = L"(TranslateTransform.Y)";
		}

		if (IsLooping() && segmentSize < containerSize)
		{
			// If the marquee is in looping mode and the segment is smaller
			// than the container, then the animation does not not need to play.

			// Reset the transform to 0 and hide the second segment
			_marqueeContainer.SetValue(dp, winrt::box_value(0));
			_segment2.Visibility(winrt::Visibility::Collapsed);

			if (_marqueeStoryboard) 
			{
				_marqueeStoryboard.Stop();
				_marqueeStoryboard = nullptr;
			}

			return false;
		}

		// The start position is offset 100% if in ticker mode
		// Otherwise it's 0
		double start = IsTicker() ? containerSize + 1 : 0;

		// The end is when the end of the text reaches the border if in bouncing mode
		// Otherwise it is when the first set of text is 100% out of view
		double end = IsBouncing() ? containerSize - segmentSize : -segmentSize;

		// The distance is used for calculating the duration and the previous
		// animation progress if resuming
		double distance = std::abs(start - end);

		// If the distance is zero, don't play an animation
		if (distance == 0)
		{
			if (_marqueeStoryboard)
			{
				_marqueeStoryboard.Stop();
				_marqueeStoryboard = nullptr;
			}
			return false;
		}

		// Swap the start and end to inverse direction for right or upwards
		if (IsDirectionInverse())
		{
			(start, end) = (end, start);
		}

		// The second segment of text should be hidden if the marquee is not in looping mode
		_segment2.Visibility(IsLooping() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);

		// Calculate the animation duration by dividing the distance by the speed
		winrt::TimeSpan duration{ static_cast<int64_t>((distance / Speed()) * 10'000'000) };

		// Create new storyboard and animation
		_marqueeStoryboard = CreateMarqueeStoryboardAnimation(start, end, duration, targetProperty);

		// Bind the storyboard completed event
		_marqueeStoryboard.Completed({ this, &Marquee::StoryBoard_Completed });

		// NOTE: Can this be optimized to remove or reduce the need for this callback?
		// Invalidate the segment measures when the transform changes.
		// This forces virtualized panels to re-measure the segments
		_marqueeTransform.RegisterPropertyChangedCallback(dp, [this]([[maybe_unused]] auto& sender, [[maybe_unused]] auto& dp)
		{
			_segment1.InvalidateMeasure();
			_segment2.InvalidateMeasure();
		});

		// Calculate the seek point for seamless animation updates
		double progress = std::abs(start - value) / distance;
		seekPoint = std::chrono::duration_cast<winrt::TimeSpan>(duration * progress);

		// Set the value of the transform to the start position if not active.
		// This puts the content in the correct starting position without using the animation.
		if (!_isActive)
		{
			_marqueeTransform.SetValue(dp, winrt::box_value(start));
		}

		// Set stopped position and animation property regardless of the active state.
		// This will be used when the animation stops.
		_stoppedPosition = start;
		_animationProperty = dp;

		return true;
	}

	winrt::Storyboard Marquee::CreateMarqueeStoryboardAnimation(double start, double end, winrt::TimeSpan duration, std::wstring_view targetProperty)
	{
		// Initialize the new storyboard
		winrt::Storyboard marqueeStoryboard;
		marqueeStoryboard.Duration(winrt::DurationHelper::FromTimeSpan(duration));
		marqueeStoryboard.RepeatBehavior(RepeatBehavior());
		marqueeStoryboard.AutoReverse(IsBouncing());

		// Create a new double animation, moving from [start] to [end] positions in [duration] time.
		winrt::DoubleAnimationUsingKeyFrames animation;
		animation.Duration(winrt::DurationHelper::FromTimeSpan(duration));
		animation.RepeatBehavior(RepeatBehavior());
		animation.AutoReverse(IsBouncing());

		// Create the key frames
		winrt::DiscreteDoubleKeyFrame frame1;
		frame1.KeyTime({ std::chrono::duration<int>::zero() });
		frame1.Value(start);

		winrt::EasingDoubleKeyFrame frame2;
		frame2.KeyTime({ duration });
		frame2.Value(end);


		// Add the key frames to the animation
		animation.KeyFrames().Append(frame1);
		animation.KeyFrames().Append(frame2);

		// Add the double animation to the storyboard
		marqueeStoryboard.Children().Append(animation);

		// Set the storyboard target and target property
		winrt::Storyboard::SetTarget(animation, _marqueeTransform);
		winrt::Storyboard::SetTargetProperty(animation, targetProperty);

		return marqueeStoryboard;
	}

	void Marquee::ClipMarquee(double width, double height)
	{
		if (_marqueeContainer == nullptr)
			return;

		width = width == 0 ? _marqueeContainer.ActualWidth() : width;
		height = height == 0 ? _marqueeContainer.ActualHeight() : height;

		// Clip the marquee within the bounds of the container
		winrt::RectangleGeometry geometry;
		geometry.Rect(winrt::Rect(0, 0, static_cast<float>(width), static_cast<float>(height)));
		_marqueeContainer.Clip(geometry);
	}

	void Marquee::BehaviorPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();

		if (control == nullptr)
		{
			return;
		}

		auto self = winrt::get_self<Marquee>(control)->get_strong();

		auto newBehavior = e.NewValue().try_as<MarqueeBehavior>();

		winrt::VisualStateManager::GoToState(control, GetVisualStateName(*newBehavior), true);

		// It is always impossible to perform an on the fly behavior change.
		self->UpdateMarquee(false);
		if (self->AutoPlay())
		{
			control.StartMarquee();
		}
	}

	void Marquee::DirectionPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();
		if (control == nullptr)
		{
			return;
		}

		auto self = winrt::get_self<Marquee>(control)->get_strong();
		auto oldDirection = e.OldValue().try_as<MarqueeDirection>();
		auto newDirection = e.NewValue().try_as<MarqueeDirection>();
		bool oldAxisX = oldDirection == MarqueeDirection::Left || oldDirection == MarqueeDirection::Right;
		bool newAxisX = newDirection == MarqueeDirection::Left || newDirection == MarqueeDirection::Right;

		winrt::VisualStateManager::GoToState(control, GetVisualStateName(*newDirection), true);

		// If the axis changed we cannot update the animation on the fly.
		// Otherwise, the animation can be updated and resumed seamlessly
		self->UpdateMarquee(oldAxisX == newAxisX);

		if (self->AutoPlay())
		{
			control.StartMarquee();
		}
	}

	void Marquee::PropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto self = winrt::get_self<Marquee>(control)->get_strong();

			// It is always possible to update these properties on the fly.
			// NOTE: The RepeatBehavior will reset its count though. Can this be fixed?
			self->UpdateMarquee(true);

			if (self->AutoPlay())
			{
				control.StartMarquee();
			}
		}
	}

	void Marquee::Marquee_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// While loaded, detach the loaded event and attach the unloaded event
		_loadedRevoker.revoke();
		_unloadedRevoker = Unloaded(winrt::auto_revoke, { this, &Marquee::Marquee_Unloaded });

		// Attach other events
		if (_marqueeContainer)
		{
			_containerSizeChangedRevoker = _marqueeContainer.SizeChanged(winrt::auto_revoke, { this, &Marquee::Container_SizeChanged });
		}

		if (_segment1)
		{
			_segmentSizeChangedRevoker = _segment1.SizeChanged(winrt::auto_revoke, { this, &Marquee::Segment_SizeChanged });
		}

		if (_marqueeStoryboard)
		{
			_storyBoardCompletedRevoker = _marqueeStoryboard.Completed(winrt::auto_revoke, { this, &Marquee::StoryBoard_Completed });
		}

		// The size may have channged while unloaded.
		// Clip the marquee
		ClipMarquee();

		// Setup the animation
		UpdateMarquee(false);

		// The marquee should run when loaded if auto play is enabled
		if (AutoPlay())
		{
			StartMarquee();
		}
	}

	void Marquee::Marquee_Unloaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// Restore the loaded event and detach the unloaded event 
		_loadedRevoker = Loaded(winrt::auto_revoke, { this, &Marquee::Marquee_Loaded });
		_unloadedRevoker.revoke();

		if (_marqueeContainer)
		{
			_containerSizeChangedRevoker.revoke();
		}

		if (_segment1)
		{
			_segmentSizeChangedRevoker.revoke();
		}

		if (_marqueeStoryboard)
		{
			_storyBoardCompletedRevoker.revoke();
		}
	}

	void Marquee::Container_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e)
	{
		if (_marqueeContainer == nullptr)
		{
			return;
		}

		// Clip the marquee
		auto size = e.NewSize();
		ClipMarquee(size.Width, size.Height);

		// Update animation on the fly
		UpdateMarquee(true);

		// The marquee should run when the size changes in case the text gets cutoff
		// and auto play is enabled.
		if (AutoPlay())
		{
			StartMarquee();
		}
	}

	void Marquee::Segment_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
	{
		if (_segment1 == nullptr)
		{
			return;
		}

		if (_marqueeContainer == nullptr)
		{
			return;
		}

		// Cap the height of the container to the segment height
		_marqueeContainer.Height(_segment1.ActualHeight());

		// If the segment size changes, we need to update the storyboard,
		// and seek to the correct position to maintain a smooth animation.
		UpdateMarquee(true);
	}

	void Marquee::StoryBoard_Completed([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& e)
	{
		StopMarquee();
		MarqueeCompleted.invoke(*this, nullptr);
	}
}
