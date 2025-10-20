#include "pch.h"
#include "Marquee.h"
#if __has_include("Marquee.g.cpp")
#include "Marquee.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	Marquee::Marquee() : _marqueeContainer(nullptr), _segment1(nullptr), _segment2(nullptr), _marqueeTransform(nullptr), _marqueeStoryboard(nullptr), _isActive(false)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void Marquee::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        // Explicit casting throws early when parts are missing from the template
        _marqueeContainer = GetTemplateChild(MarqueeContainerPartName).try_as<Panel>();
        _segment1 = GetTemplateChild(Segment1PartName).try_as<ContentPresenter>();
        _segment2 = GetTemplateChild(Segment2PartName).try_as<ContentPresenter>();
        _marqueeTransform = GetTemplateChild(MarqueeTransformPartName).try_as<TranslateTransform>();

        _marqueeContainer.SizeChanged({ this, &Marquee::Container_SizeChanged });
        _segment1.SizeChanged({ this, &Marquee::Segment_SizeChanged });

        // Swapping tabs in TabView caused errors where the control would unload and never reattach events.
        // Hotfix: Track the loaded event. This should be fine because the GC will handle detaching the Loaded
        // event on disposal. However, more research is required
        _loadedToken = Loaded({ this, &Marquee::Marquee_Loaded });

        VisualStateManager::GoToState(*this, GetVisualStateName(Direction()), false);
        VisualStateManager::GoToState(*this, GetVisualStateName(Behavior()), false);
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
        bool initial = _isActive;
        _isActive = true;
        bool playing = UpdateAnimation(initial);

        // Invoke MarqueeBegan if Marquee is now playing and was not before
        if (playing && !initial)
        {
            MarqueeBegan.invoke(*this, nullptr);
        }
    }

    void Marquee::StopMarquee()
    {
        StopMarquee(_isActive);
    }

    void Marquee::StopMarquee(bool initialState)
    {
        // Set _isActive and update the animation to match
        _isActive = false;
        bool playing = UpdateAnimation(false);

        // Invoke MarqueeStopped if Marquee is not playing and was before
        if (!playing && initialState)
        {
            MarqueeStopped.invoke(*this, nullptr);
        }
    }

    bool Marquee::UpdateAnimation(bool resume)
    {
        // Crucial template parts are missing!
        // This can happen during initialization of certain properties.
        // Gracefully return when this happens. Proper checks for these template parts happen in OnApplyTemplate.
        if (_marqueeContainer == nullptr ||
            _marqueeTransform == nullptr ||
            _segment1 == nullptr ||
            _segment2 == nullptr)
        {
            return false;
        }

        // The marquee is stopped.
        // Update the animation to the stopped position.
        if (!_isActive)
        {
            VisualStateManager::GoToState(*this, MarqueeStoppedState, false);

            return false;
        }

        // Get the size of the container and segment, based on the orientation.
        // Also track the property to adjust, also based on the orientation.
        double containerSize;
        double segmentSize;
        double value;
        DependencyProperty dp{ nullptr };
        std::wstring_view targetProperty;

        if (IsDirectionHorizontal())
        {
            // The direction is horizontal, so the sizes, value, and properties
            // are defined by width and X coordinates.
            containerSize = _marqueeContainer.ActualWidth();
            segmentSize = _segment1.ActualWidth();
            value = _marqueeTransform.X();
            dp = TranslateTransform::XProperty();
            targetProperty = L"(TranslateTransform.X)";
        }
        else
        {
            // The direction is vertical, so the sizes, value, and properties
            // are defined by height and Y coordinates.
            containerSize = _marqueeContainer.ActualHeight();
            segmentSize = _segment1.ActualHeight();
            value = _marqueeTransform.Y();
            dp = TranslateTransform::YProperty();
            targetProperty = L"(TranslateTransform.Y)";
        }

        if (IsLooping() && segmentSize < containerSize)
        {
            // If the marquee is in looping mode and the segment is smaller
            // than the container, then the animation does not not need to play.

            // NOTE: Use resume as initial because _isActive is updated before
            // calling update animation. If _isActive were passed, it would allow for
            // MarqueeStopped to be invoked when the marquee was already stopped.
            StopMarquee(resume);
            _segment2.Visibility(Visibility::Collapsed);

            return false;
        }

        // The start position is offset 100% if in ticker mode
        // Otherwise it's 0
        double start = IsTicker() ? containerSize : 0;

        // The end is when the end of the text reaches the border if in bouncing mode
        // Otherwise it is when the first set of text is 100% out of view
        double end = IsBouncing() ? containerSize - segmentSize : -segmentSize;

        // The distance is used for calculating the duration and the previous
        // animation progress if resuming
        double distance = std::abs(start - end);

        // If the distance is zero, don't play an animation
        if (distance == 0)
        {
            return false;
        }

        // Swap the start and end to inverse direction for right or upwards
        if (IsDirectionInverse())
        {
            (start, end) = (end, start);
        }

        // The second segment of text should be hidden if the marquee is not in looping mode
        _segment2.Visibility(IsLooping() ? Visibility::Visible : Visibility::Collapsed);

        // Calculate the animation duration by dividing the distance by the speed
        TimeSpan duration{ static_cast<int64_t>((distance / Speed()) * 10'000'000) };
        // Unbind events from the old storyboard
        if (_marqueeStoryboard)
        {
            _marqueeStoryboard.Completed(_storyBoardCompletedToken);
        }

        // Create new storyboard and animation
        _marqueeStoryboard = CreateMarqueeStoryboardAnimation(start, end, duration, targetProperty);

        // Bind the storyboard completed event
        _storyBoardCompletedToken = _marqueeStoryboard.Completed({ this, &Marquee::StoryBoard_Completed });

        // Set the visual state to active and begin the animation
        VisualStateManager::GoToState(*this, MarqueeActiveState, true);
        _marqueeStoryboard.Begin();

        // If resuming, seek the animation so the text resumes from its current position.
        if (resume)
        {
            double progress = std::abs(start - value) / distance;
            _marqueeStoryboard.Seek(
                std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(duration* progress));
        }

        // NOTE: Can this be optimized to remove or reduce the need for this callback?
        // Invalidate the segment measures when the transform changes.
        // This forces virtualized panels to re-measure the segments
        _marqueeTransform.RegisterPropertyChangedCallback(dp, [this]([[maybe_unused]] auto& sender, [[maybe_unused]] auto& dp)
        {
            _segment1.InvalidateMeasure();
            _segment2.InvalidateMeasure();
        });

        return true;
    }

    Storyboard Marquee::CreateMarqueeStoryboardAnimation(double start, double end, TimeSpan duration, std::wstring_view targetProperty)
    {
        // Initialize the new storyboard
        Storyboard marqueeStoryboard;
        marqueeStoryboard.Duration(winrt::Microsoft::UI::Xaml::DurationHelper::FromTimeSpan(duration));
        marqueeStoryboard.RepeatBehavior(RepeatBehavior());
        marqueeStoryboard.AutoReverse(IsBouncing());

        // Create a new double animation, moving from [start] to [end] positions in [duration] time.
        DoubleAnimationUsingKeyFrames animation;
        animation.Duration(winrt::Microsoft::UI::Xaml::DurationHelper::FromTimeSpan(duration));
        animation.RepeatBehavior(RepeatBehavior());
        animation.AutoReverse(IsBouncing());

        // Create the key frames
        DiscreteDoubleKeyFrame frame1;
        frame1.KeyTime({ std::chrono::duration<int>::zero() });
        frame1.Value(start);

        EasingDoubleKeyFrame frame2;
        frame2.KeyTime({ duration });
        frame2.Value(end);
        

        // Add the key frames to the animation
        animation.KeyFrames().Append(frame1);
        animation.KeyFrames().Append(frame2);

        // Add the double animation to the storyboard
        marqueeStoryboard.Children().Append(animation);

        // Set the storyboard target and target property
        Storyboard::SetTarget(animation, _marqueeTransform);
        Storyboard::SetTargetProperty(animation, targetProperty);

        return marqueeStoryboard;
    }

    void Marquee::BehaviorPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        auto control = d.try_as<class_type>();
		
        if (control == nullptr)
        {
            return;
        }

        auto self = winrt::get_self<Marquee>(control)->get_strong();

        bool active = self->_isActive;
        auto newBehavior = e.NewValue().try_as<MarqueeBehavior>();

        VisualStateManager::GoToState(control, GetVisualStateName(*newBehavior), true);

        self->StopMarquee(false);
        if (active)
        {
            control.StartMarquee();
        }
    }

    void Marquee::DirectionPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        auto control = d.try_as<class_type>();
        if (control == nullptr)
        {
            return;
        }

        auto self = winrt::get_self<Marquee>(control)->get_strong();
        bool active = self->_isActive;
        auto oldDirection = e.OldValue().try_as<MarqueeDirection>();
        auto newDirection = e.NewValue().try_as<MarqueeDirection>();
        bool oldAxisX = oldDirection == MarqueeDirection::Left || oldDirection == MarqueeDirection::Right;
        bool newAxisX = newDirection == MarqueeDirection::Left || newDirection == MarqueeDirection::Right;

        VisualStateManager::GoToState(control, GetVisualStateName(*newDirection), true);

        if (oldAxisX != newAxisX)
        {
            self->StopMarquee(false);
        }

        if (active)
        {
            control.StartMarquee();
        }
    }

    void Marquee::PropertyChanged(DependencyObject d, [[maybe_unused]] DependencyPropertyChangedEventArgs e)
    {
        if (auto control = d.try_as<class_type>())
        {
            auto self = winrt::get_self<Marquee>(control)->get_strong();
            self->UpdateAnimation();
        }
    }

    void Marquee::Marquee_Loaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        // While loaded, detach the loaded event and attach the unloaded event
        Loaded(_loadedToken);
        _unloadedToken = Unloaded({ this, &Marquee::Marquee_Unloaded });

        // Attach other events
        if (_marqueeContainer)
        {
            _containerSizeChangedToken = _marqueeContainer.SizeChanged({ this, &Marquee::Container_SizeChanged });
        }

        if (_marqueeStoryboard)
        {
            _storyBoardCompletedToken = _marqueeStoryboard.Completed({ this, &Marquee::StoryBoard_Completed });
        }
    }

    void Marquee::Marquee_Unloaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        // Restore the loaded event and detach the unloaded event 
        _loadedToken = Loaded({ this, &Marquee::Marquee_Loaded });
        Unloaded(_unloadedToken);

        if (_marqueeContainer)
        {
            _marqueeContainer.SizeChanged(_containerSizeChangedToken);
        }

        if (_marqueeStoryboard)
        {
            _marqueeStoryboard.Completed(_storyBoardCompletedToken);
        }
    }

    void Marquee::Container_SizeChanged([[maybe_unused]] IInspectable const& sender, SizeChangedEventArgs const& e)
    {
        if (_marqueeContainer == nullptr)
        {
            return;
        }

        // Clip the marquee within its bounds
        RectangleGeometry geometry;
        geometry.Rect({ 0, 0, e.NewSize().Width, e.NewSize().Height });
        _marqueeContainer.Clip(geometry);

        // The marquee should run when the size changes in case the text gets cutoff
        StartMarquee();
    }

    void Marquee::Segment_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
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
        UpdateAnimation(true);
    }

    void Marquee::StoryBoard_Completed([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IInspectable const& e)
    {
        StopMarquee(true);
        MarqueeCompleted.invoke(*this, nullptr);
    }
}
