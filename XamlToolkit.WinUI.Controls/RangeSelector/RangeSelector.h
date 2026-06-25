#pragma once

#include "RangeChangedEventArgs.h"
#include "RangeSelector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <chrono>
#include <string_view>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Dispatching;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Shapes;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using DispatcherQueueTimer = Microsoft::UI::Dispatching::DispatcherQueueTimer;
    using DispatcherQueue = Microsoft::UI::Dispatching::DispatcherQueue;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct RangeSelector : RangeSelectorT<RangeSelector>
    {
        static constexpr std::wstring_view CommonStates = L"CommonStates";
        static constexpr std::wstring_view NormalState = L"Normal";
        static constexpr std::wstring_view PointerOverState = L"PointerOver";
        static constexpr std::wstring_view DisabledState = L"Disabled";
        static constexpr std::wstring_view MinPressedState = L"MinPressed";
        static constexpr std::wstring_view MaxPressedState = L"MaxPressed";
        static constexpr std::wstring_view HorizontalState = L"Horizontal";
        static constexpr std::wstring_view VerticalState = L"Vertical";

        static constexpr std::wstring_view MinThumbPartName = L"MinThumb";
        static constexpr std::wstring_view MaxThumbPartName = L"MaxThumb";
        static constexpr std::wstring_view ContainerCanvasPartName = L"ContainerCanvas";
        static constexpr std::wstring_view ActiveRectanglePartName = L"ActiveRectangle";

        //static constexpr int32_t DefaultKeyBoardOffset = 12;   // Default offset for automatic tooltips opened by keyboard.
        static constexpr int32_t DefaultMouseOffset = 20;        // Default offset for automatic tooltips opened by mouse.
        //static constexpr int32_t DefaultTouchOffset = 44;      // Default offset for automatic tooltips opened by touch.

        static constexpr double Epsilon = 0.01;
        static constexpr double DefaultMinimum = 0.0;
        static constexpr double DefaultMaximum = 10.0;
        static constexpr double DefaultStepFrequency = 1;
        static constexpr std::chrono::seconds TimeToHideToolTipOnKeyUp = std::chrono::seconds{ 1 };

        RangeSelector();

        void OnApplyTemplate();

        wil::untyped_event<winrt::XamlToolkit::WinUI::Controls::RangeChangedEventArgs> ValueChanged;

        winrt::event_token ThumbDragStarted(winrt::DragStartedEventHandler const& handler);

        void ThumbDragStarted(winrt::event_token const& token) noexcept;

        winrt::event_token ThumbDragCompleted(winrt::DragCompletedEventHandler const& handler);

        void ThumbDragCompleted(winrt::event_token const& token) noexcept;

        virtual void OnThumbDragStarted(winrt::DragStartedEventArgs const& e);

        virtual void OnThumbDragCompleted(winrt::DragCompletedEventArgs const& e);

        virtual void OnValueChanged(winrt::XamlToolkit::WinUI::Controls::RangeChangedEventArgs const& e);

        static void MinimumChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void MaximumChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void RangeMinChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void RangeMaxChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OrientationChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static const wil::single_threaded_property<winrt::DependencyProperty> MinimumProperty;

        double Minimum() const
        {
            return winrt::unbox_value<double>(GetValue(MinimumProperty));
        }

        void Minimum(double const& value) const
        {
            SetValue(MinimumProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> MaximumProperty;

        double Maximum() const
        {
            return winrt::unbox_value<double>(GetValue(MaximumProperty));
        }

        void Maximum(double const& value) const
        {
            SetValue(MaximumProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> RangeStartProperty;

        double RangeStart() const
        {
            return winrt::unbox_value<double>(GetValue(RangeStartProperty));
        }

        void RangeStart(double const& value) const
        {
            SetValue(RangeStartProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> RangeEndProperty;

        double RangeEnd() const
        {
            return winrt::unbox_value<double>(GetValue(RangeEndProperty));
        }

        void RangeEnd(double const& value) const
        {
            SetValue(RangeEndProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> StepFrequencyProperty;

        double StepFrequency() const
        {
            return winrt::unbox_value<double>(GetValue(StepFrequencyProperty));
        }

        void StepFrequency(double const& value) const
        {
            SetValue(StepFrequencyProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

        winrt::Orientation Orientation() const
        {
            return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty));
        }

        void Orientation(winrt::Orientation const& value) const
        {
            SetValue(OrientationProperty, winrt::box_value(value));
        }

    private:
        void ContainerCanvas_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        void VerifyValues() const;

        void RangeMinToStepFrequency();

        void RangeMaxToStepFrequency();

        double MoveToStepFrequency(double rangeValue) const;

        void SyncThumbs(bool fromMinKeyDown = false, bool fromMaxKeyDown = false);

        void SyncActiveRectangle();

        void RangeSelector_IsEnabledChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

#pragma region Pointer Events
        void ContainerCanvas_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void ContainerCanvas_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void ContainerCanvas_PointerReleased(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void ContainerCanvas_PointerMoved(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void ContainerCanvas_PointerPressed(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
#pragma endregion

#pragma region Key Events
        double GetKeyDelta(winrt::Windows::System::VirtualKey key) const;

        void MinThumb_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

        void MaxThumb_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

        void Thumb_KeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);
#pragma endregion

#pragma region Drag Events
        void MinThumb_DragDelta(winrt::IInspectable const& sender, winrt::DragDeltaEventArgs const& e);

        void MaxThumb_DragDelta(winrt::IInspectable const& sender, winrt::DragDeltaEventArgs const& e);

        void MinThumb_DragStarted(winrt::IInspectable const& sender, winrt::DragStartedEventArgs const& e);

        void MaxThumb_DragStarted(winrt::IInspectable const& sender, winrt::DragStartedEventArgs const& e);

        void Thumb_DragCompleted(winrt::IInspectable const& sender, winrt::DragCompletedEventArgs const& e);

        double DragLength() const;

        double DragThumb(winrt::Thumb const& thumb, double min, double max, double nextPos);

        void Thumb_DragStarted(winrt::Thumb const& thumb);
#pragma endregion

        void Debounce();

        bool IsHorizontal() const;

        double GetPointerAxisPosition(winrt::PointerRoutedEventArgs const& e) const;

        void UpdateToolTip(winrt::Thumb const& thumb, double newValue);

        void AttachToolTip(winrt::Thumb const& thumb);

        void DetachToolTip(winrt::Thumb const& thumb);

    private:
        winrt::DispatcherQueueTimer _keyDebounceTimer = winrt::DispatcherQueue::GetForCurrentThread().CreateTimer();

        winrt::Rectangle _activeRectangle{ nullptr };
        winrt::Thumb _minThumb{ nullptr };
        winrt::Thumb _maxThumb{ nullptr };
        winrt::Canvas _containerCanvas{ nullptr };
        double _oldValue{ 0.0 };
        bool _valuesAssigned{ false };
        bool _minSet{ false };
        bool _maxSet{ false };
        bool _pointerManipulatingMin{ false };
        bool _pointerManipulatingMax{ false };
        double _absolutePosition{ 0.0 };
        winrt::ToolTip _toolTip{ nullptr };
        winrt::TextBlock _toolTipText{ nullptr };

        winrt::Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;

        winrt::Thumb::DragCompleted_revoker _minThumbDragCompletedRevoker;
        winrt::Thumb::DragDelta_revoker _minThumbDragDeltaRevoker;
        winrt::Thumb::DragStarted_revoker _minThumbDragStartedRevoker;
        winrt::UIElement::KeyDown_revoker _minThumbKeyDownRevoker;
        winrt::UIElement::KeyUp_revoker _minThumbKeyUpRevoker;

        winrt::Thumb::DragCompleted_revoker _maxThumbDragCompletedRevoker;
        winrt::Thumb::DragDelta_revoker _maxThumbDragDeltaRevoker;
        winrt::Thumb::DragStarted_revoker _maxThumbDragStartedRevoker;
        winrt::UIElement::KeyDown_revoker _maxThumbKeyDownRevoker;
        winrt::UIElement::KeyUp_revoker _maxThumbKeyUpRevoker;

        winrt::FrameworkElement::SizeChanged_revoker _canvasSizeChangedRevoker;
        winrt::UIElement::PointerEntered_revoker _canvasPointerEnteredRevoker;
        winrt::UIElement::PointerPressed_revoker _canvasPointerPressedRevoker;
        winrt::UIElement::PointerMoved_revoker _canvasPointerMovedRevoker;
        winrt::UIElement::PointerReleased_revoker _canvasPointerReleasedRevoker;
        winrt::UIElement::PointerExited_revoker _canvasPointerExitedRevoker;

        winrt::event<winrt::DragStartedEventHandler> _thumbDragStarted;
        winrt::event<winrt::DragCompletedEventHandler> _thumbDragCompleted;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct RangeSelector : RangeSelectorT<RangeSelector, implementation::RangeSelector>
    {
    };
}
