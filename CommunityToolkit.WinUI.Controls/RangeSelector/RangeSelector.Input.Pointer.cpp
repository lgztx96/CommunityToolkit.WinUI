#include "pch.h"
#include "RangeSelector.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    void RangeSelector::ContainerCanvas_PointerEntered([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
    {
        VisualStateManager::GoToState(*this, PointerOverState, false);
    }

    void RangeSelector::ContainerCanvas_PointerExited([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        auto position = e.GetCurrentPoint(_containerCanvas).Position().X;
        auto normalizedPosition = ((position / DragWidth()) * (Maximum() - Minimum())) + Minimum();

        if (_pointerManipulatingMin)
        {
            _pointerManipulatingMin = false;
            OnValueChanged(winrt::make<RangeChangedEventArgs>(RangeStart(), normalizedPosition, RangeSelectorProperty::MinimumValue));
        }
        else if (_pointerManipulatingMax)
        {
            _pointerManipulatingMax = false;
            OnValueChanged(winrt::make<RangeChangedEventArgs>(RangeEnd(), normalizedPosition, RangeSelectorProperty::MaximumValue));
        }

        if (_containerCanvas != nullptr)
        {
            _containerCanvas.IsHitTestVisible(true);
        }

        if (_toolTip != nullptr)
        {
            _toolTip.Visibility(Visibility::Collapsed);
        }

        VisualStateManager::GoToState(*this, NormalState, false);
    }

    void RangeSelector::ContainerCanvas_PointerReleased([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        auto position = e.GetCurrentPoint(_containerCanvas).Position().X;
        auto normalizedPosition = ((position / DragWidth()) * (Maximum() - Minimum())) + Minimum();

        if (_pointerManipulatingMin)
        {
            _pointerManipulatingMin = false;
            OnValueChanged(winrt::make<RangeChangedEventArgs>(RangeStart(), normalizedPosition, RangeSelectorProperty::MinimumValue));
        }
        else if (_pointerManipulatingMax)
        {
            _pointerManipulatingMax = false;
            OnValueChanged(winrt::make<RangeChangedEventArgs>(RangeEnd(), normalizedPosition, RangeSelectorProperty::MaximumValue));
        }

        if (_containerCanvas != nullptr)
        {
            _containerCanvas.IsHitTestVisible(true);
        }

        SyncThumbs();

        if (_toolTip != nullptr)
        {
            _toolTip.Visibility(Visibility::Collapsed);
        }
    }

    void RangeSelector::ContainerCanvas_PointerMoved([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        auto position = e.GetCurrentPoint(_containerCanvas).Position().X;
        // auto normalizedPosition = ((position / DragWidth()) * (Maximum() - Minimum())) + Minimum();

        if (_pointerManipulatingMin)
        {
            RangeStart(DragThumb(_minThumb, 0, DragWidth(), position));
            if (_toolTipText != nullptr)
            {
                UpdateToolTipText(*this, _toolTipText, RangeStart());
            }
        }
        else if (_pointerManipulatingMax)
        {
            if (_toolTipText != nullptr)
            {
                RangeEnd(DragThumb(_maxThumb, 0, DragWidth(), position));
                UpdateToolTipText(*this, _toolTipText, RangeEnd());
            }
        }
    }

    void RangeSelector::ContainerCanvas_PointerPressed([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        auto position = e.GetCurrentPoint(_containerCanvas).Position().X;
        auto normalizedPosition = position * std::abs(Maximum() - Minimum()) / DragWidth();
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