#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#endif
#include "RangeSelector.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RangeSelector::ContainerCanvas_PointerEntered([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        winrt::VisualStateManager::GoToState(*this, PointerOverState, false);
    }

    void RangeSelector::ContainerCanvas_PointerExited([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        double position = GetPointerAxisPosition(e);
        double percent = IsHorizontal() ? (position / DragLength()) : (1.0 - position / DragLength());
        double normalizedPosition = Minimum() + percent * (Maximum() - Minimum());

        if (_pointerManipulatingMin)
        {
            _pointerManipulatingMin = false;
            auto args = winrt::make_self<RangeChangedEventArgs>(RangeStart(), normalizedPosition, RangeSelectorProperty::MinimumValue);
            OnValueChanged(*args);
        }
        else if (_pointerManipulatingMax)
        {
            _pointerManipulatingMax = false;
            auto args = winrt::make_self<RangeChangedEventArgs>(RangeEnd(), normalizedPosition, RangeSelectorProperty::MaximumValue);
            OnValueChanged(*args);
        }

        if (_toolTip != nullptr)
        {
            _toolTip.IsOpen(false);
        }

        winrt::VisualStateManager::GoToState(*this, NormalState, false);
    }

    void RangeSelector::ContainerCanvas_PointerReleased([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        double position = GetPointerAxisPosition(e);
        double dragLength = DragLength();
        position = std::clamp(std::max<double>(0.0, position), 0.0, std::max<double>(0.0, dragLength));
        double percent = IsHorizontal() ? (position / dragLength) : (1.0 - position / dragLength);
        double normalizedPosition = Minimum() + percent * (Maximum() - Minimum());

        if (_pointerManipulatingMin)
        {
            _pointerManipulatingMin = false;
            auto args = winrt::make_self<RangeChangedEventArgs>(RangeStart(), normalizedPosition, RangeSelectorProperty::MinimumValue);
            OnValueChanged(*args);
            DetachToolTip(_minThumb);
        }
        else if (_pointerManipulatingMax)
        {
            _pointerManipulatingMax = false;
            auto args = winrt::make_self<RangeChangedEventArgs>(RangeEnd(), normalizedPosition, RangeSelectorProperty::MaximumValue);
            OnValueChanged(*args);
            DetachToolTip(_maxThumb);
        }

        SyncThumbs();

        if (_toolTip != nullptr)
        {
            _toolTip.IsOpen(false);
        }
    }

    void RangeSelector::ContainerCanvas_PointerMoved([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        double position = GetPointerAxisPosition(e);

        if (_pointerManipulatingMin)
        {
            RangeStart(DragThumb(_minThumb, 0, DragLength(), position));
            if (_toolTip != nullptr)
            {
                UpdateToolTip(_minThumb, RangeStart());
            }
        }
        else if (_pointerManipulatingMax)
        {
            if (_toolTipText != nullptr)
            {
                RangeEnd(DragThumb(_maxThumb, 0, DragLength(), position));
                UpdateToolTip(_maxThumb, RangeEnd());
            }
        }
    }

    void RangeSelector::ContainerCanvas_PointerPressed([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        double position = GetPointerAxisPosition(e);
        double dragLength = DragLength();
        position = std::clamp(std::max<double>(0.0, position), 0.0, std::max<double>(0.0, dragLength));
        double percent = IsHorizontal() ? (position / dragLength) : (1.0 - position / dragLength);
        double normalizedPosition = Minimum() + percent * (Maximum() - Minimum());

        double upperValueDiff = std::abs(RangeEnd() - normalizedPosition);
        double lowerValueDiff = std::abs(RangeStart() - normalizedPosition);

        if (upperValueDiff < lowerValueDiff)
        {
            RangeEnd(normalizedPosition);
            _pointerManipulatingMax = true;
            if (_maxThumb != nullptr)
            {
                Thumb_DragStarted(_maxThumb);
            }
        }
        else
        {
            RangeStart(normalizedPosition);
            _pointerManipulatingMin = true;
            if (_minThumb != nullptr)
            {
                Thumb_DragStarted(_minThumb);
            }
        }

        SyncThumbs();
    }
}
