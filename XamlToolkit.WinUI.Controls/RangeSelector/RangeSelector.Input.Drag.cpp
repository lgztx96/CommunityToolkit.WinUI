#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RangeSelector.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RangeSelector::MinThumb_DragDelta([[maybe_unused]] IInspectable const& sender, DragDeltaEventArgs const& e)
    {
        double delta = IsHorizontal() ? e.HorizontalChange() : e.VerticalChange();

        _absolutePosition += delta;

        RangeStart(DragThumb(_minThumb, 0, DragLength(), _absolutePosition));

        if (_toolTip != nullptr)
        {
            UpdateToolTip(_minThumb, RangeStart());
        }
    }

    void RangeSelector::MaxThumb_DragDelta([[maybe_unused]] IInspectable const& sender, DragDeltaEventArgs const& e)
    {
        double delta = IsHorizontal() ? e.HorizontalChange() : e.VerticalChange();

        _absolutePosition += delta;

        RangeEnd(DragThumb(_maxThumb, 0, DragLength(), _absolutePosition));

        if (_toolTip != nullptr)
        {
			UpdateToolTip(_maxThumb, RangeEnd());
        }
    }

    void RangeSelector::MinThumb_DragStarted([[maybe_unused]] IInspectable const& sender, DragStartedEventArgs const& e)
    {
        OnThumbDragStarted(e);
        if (_minThumb != nullptr)
        {
            Thumb_DragStarted(_minThumb);
        }
    }

    void RangeSelector::MaxThumb_DragStarted([[maybe_unused]] IInspectable const& sender, DragStartedEventArgs const& e)
    {
        OnThumbDragStarted(e);
        if (_maxThumb != nullptr)
        {
            Thumb_DragStarted(_maxThumb);
        }
    }

    void RangeSelector::Thumb_DragCompleted(IInspectable const& sender, DragCompletedEventArgs const& e)
    {
        OnThumbDragCompleted(e);

        auto args = sender == _minThumb
            ? winrt::make_self<RangeChangedEventArgs>(_oldValue, RangeStart(), RangeSelectorProperty::MinimumValue)
            : winrt::make_self<RangeChangedEventArgs>(_oldValue, RangeEnd(), RangeSelectorProperty::MaximumValue);

        OnValueChanged(*args);

        SyncThumbs();

        if (_toolTip != nullptr)
        {
			DetachToolTip(sender.as<Thumb>());
        }

        VisualStateManager::GoToState(*this, NormalState, true);
    }

    double RangeSelector::DragLength() const
    {
        if (IsHorizontal()) 
        {
            return _containerCanvas.ActualWidth() - _minThumb.ActualWidth();
        }  
        else 
        {
            return _containerCanvas.ActualHeight() - _minThumb.ActualHeight();
        } 
    }

    double RangeSelector::DragThumb(Thumb const& thumb, double min, double max, double nextPos)
    {
        nextPos = std::max<double>(min, nextPos);
        nextPos = std::min<double>(max, nextPos);

        if (IsHorizontal())
        {
            Canvas::SetLeft(thumb, nextPos);
        }
        else
        {
            Canvas::SetTop(thumb, nextPos);
        }

        double percent = IsHorizontal() ? (nextPos / DragLength()) : (1.0 - nextPos / DragLength());

        return Minimum() + percent * (Maximum() - Minimum());
    }

    void RangeSelector::Thumb_DragStarted(Thumb const& thumb)
    {
        auto useMin = (thumb == _minThumb);
        const auto& otherThumb = useMin ? _maxThumb : _minThumb;

        _absolutePosition = IsHorizontal() ? Canvas::GetLeft(thumb) : Canvas::GetTop(thumb);

        Canvas::SetZIndex(thumb, 10);
        Canvas::SetZIndex(otherThumb, 0);

        _oldValue = useMin ? RangeStart() : RangeEnd();

        if (_toolTip)
        {
			AttachToolTip(thumb);
            UpdateToolTip(thumb, _oldValue);
        }

        VisualStateManager::GoToState(*this, useMin ? MinPressedState : MaxPressedState, true);
    }
}
