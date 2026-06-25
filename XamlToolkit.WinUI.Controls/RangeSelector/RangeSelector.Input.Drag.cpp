#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RangeSelector.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RangeSelector::MinThumb_DragDelta([[maybe_unused]] winrt::IInspectable const& sender, winrt::DragDeltaEventArgs const& e)
    {
        double delta = IsHorizontal() ? e.HorizontalChange() : e.VerticalChange();

        _absolutePosition += delta;

        RangeStart(DragThumb(_minThumb, 0, DragLength(), _absolutePosition));

        if (_toolTip)
        {
            UpdateToolTip(_minThumb, RangeStart());
        }
    }

    void RangeSelector::MaxThumb_DragDelta([[maybe_unused]] winrt::IInspectable const& sender, winrt::DragDeltaEventArgs const& e)
    {
        double delta = IsHorizontal() ? e.HorizontalChange() : e.VerticalChange();

        _absolutePosition += delta;

        RangeEnd(DragThumb(_maxThumb, 0, DragLength(), _absolutePosition));

        if (_toolTip)
        {
            UpdateToolTip(_maxThumb, RangeEnd());
        }
    }

    void RangeSelector::MinThumb_DragStarted([[maybe_unused]] winrt::IInspectable const& sender, winrt::DragStartedEventArgs const& e)
    {
        OnThumbDragStarted(e);
        if (_minThumb)
        {
            Thumb_DragStarted(_minThumb);
        }
    }

    void RangeSelector::MaxThumb_DragStarted([[maybe_unused]] winrt::IInspectable const& sender, winrt::DragStartedEventArgs const& e)
    {
        OnThumbDragStarted(e);
        if (_maxThumb)
        {
            Thumb_DragStarted(_maxThumb);
        }
    }

    void RangeSelector::Thumb_DragCompleted(winrt::IInspectable const& sender, winrt::DragCompletedEventArgs const& e)
    {
        OnThumbDragCompleted(e);

        auto args = sender == _minThumb
            ? winrt::make_self<RangeChangedEventArgs>(_oldValue, RangeStart(), RangeSelectorProperty::MinimumValue)
            : winrt::make_self<RangeChangedEventArgs>(_oldValue, RangeEnd(), RangeSelectorProperty::MaximumValue);

        OnValueChanged(*args);

        SyncThumbs();

        if (_toolTip)
        {
            DetachToolTip(sender.as<winrt::Thumb>());
        }

        winrt::VisualStateManager::GoToState(*this, NormalState, true);
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

    double RangeSelector::DragThumb(winrt::Thumb const& thumb, double min, double max, double nextPos)
    {
        nextPos = std::max<double>(min, nextPos);
        nextPos = std::min<double>(max, nextPos);

        if (IsHorizontal())
        {
            winrt::Canvas::SetLeft(thumb, nextPos);
        }
        else
        {
            winrt::Canvas::SetTop(thumb, nextPos);
        }

        double percent = IsHorizontal() ? (nextPos / DragLength()) : (1.0 - nextPos / DragLength());

        return Minimum() + percent * (Maximum() - Minimum());
    }

    void RangeSelector::Thumb_DragStarted(winrt::Thumb const& thumb)
    {
        const auto useMin = (thumb == _minThumb);
        const auto& otherThumb = useMin ? _maxThumb : _minThumb;

        _absolutePosition = IsHorizontal() ? winrt::Canvas::GetLeft(thumb) : winrt::Canvas::GetTop(thumb);

        winrt::Canvas::SetZIndex(thumb, 10);
        winrt::Canvas::SetZIndex(otherThumb, 0);

        _oldValue = useMin ? RangeStart() : RangeEnd();

        if (_toolTip)
        {
            AttachToolTip(thumb);
            UpdateToolTip(thumb, _oldValue);
        }

        winrt::VisualStateManager::GoToState(*this, useMin ? MinPressedState : MaxPressedState, true);
    }
}
