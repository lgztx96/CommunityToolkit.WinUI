#include "pch.h"
#include "RangeSelector.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    void RangeSelector::MinThumb_DragDelta([[maybe_unused]] IInspectable const& sender, DragDeltaEventArgs const& e)
    {
        _absolutePosition += e.HorizontalChange();

        RangeStart(DragThumb(_minThumb, 0, DragWidth(), _absolutePosition));

        if (_toolTipText != nullptr)
        {
            UpdateToolTipText(*this, _toolTipText, RangeStart());
        }
    }

    void RangeSelector::MaxThumb_DragDelta([[maybe_unused]] IInspectable const& sender, DragDeltaEventArgs const& e)
    {
        _absolutePosition += e.HorizontalChange();

        RangeEnd(DragThumb(_maxThumb, 0, DragWidth(), _absolutePosition));

        if (_toolTipText != nullptr)
        {
            UpdateToolTipText(*this, _toolTipText, RangeEnd());
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
        OnValueChanged(sender == _minThumb ? winrt::make<RangeChangedEventArgs>(_oldValue, RangeStart(), RangeSelectorProperty::MinimumValue) : winrt::make<RangeChangedEventArgs>(_oldValue, RangeEnd(), RangeSelectorProperty::MaximumValue));
        SyncThumbs();

        if (_toolTip != nullptr)
        {
            _toolTip.Visibility(Visibility::Collapsed);
        }

        VisualStateManager::GoToState(*this, NormalState, true);
    }

    double RangeSelector::DragWidth()
    {
        return _containerCanvas.ActualWidth() - _maxThumb.Width();
    }

    double RangeSelector::DragThumb(Thumb const& thumb, double min, double max, double nextPos)
    {
        nextPos = std::max<double>(min, nextPos);
        nextPos = std::min<double>(max, nextPos);

        Canvas::SetLeft(thumb, nextPos);

        if (_toolTip != nullptr && thumb != nullptr)
        {
            auto thumbCenter = nextPos + (thumb.Width() / 2);
            _toolTip.Measure(Size(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
            auto ttWidth = _toolTip.ActualWidth() / 2;

            Canvas::SetLeft(_toolTip, thumbCenter - ttWidth);
        }

        return Minimum() + ((nextPos / DragWidth()) * (Maximum() - Minimum()));
    }

    void RangeSelector::Thumb_DragStarted(Thumb const& thumb)
    {
        auto useMin = thumb == _minThumb;
        auto otherThumb = useMin ? _maxThumb : _minThumb;

        _absolutePosition = Canvas::GetLeft(thumb);
        Canvas::SetZIndex(thumb, 10);
        Canvas::SetZIndex(otherThumb, 0);
        _oldValue = RangeStart();

        if (_toolTip != nullptr)
        {
            _toolTip.Visibility(Visibility::Visible);
            auto thumbCenter = _absolutePosition + (thumb.Width() / 2);
            _toolTip.Measure(Size(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
            auto ttWidth = _toolTip.ActualWidth() / 2;
            Canvas::SetLeft(_toolTip, thumbCenter - ttWidth);

            if (_toolTipText != nullptr)
                UpdateToolTipText(*this, _toolTipText, useMin ? RangeStart() : RangeEnd());
        }

        VisualStateManager::GoToState(*this, useMin ? MinPressedState : MaxPressedState, true);
    }
}