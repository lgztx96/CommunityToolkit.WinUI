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
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Shapes;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RangeSelector : RangeSelectorT<RangeSelector>
	{
	private:
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

		Microsoft::UI::Dispatching::DispatcherQueueTimer keyDebounceTimer = Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread().CreateTimer();

		Rectangle _activeRectangle{ nullptr };
		Thumb _minThumb{ nullptr };
		Thumb _maxThumb{ nullptr };
		Canvas _containerCanvas{ nullptr };
		double _oldValue{ 0.0 };
		bool _valuesAssigned{ false };
		bool _minSet{ false };
		bool _maxSet{ false };
		bool _pointerManipulatingMin{ false };
		bool _pointerManipulatingMax{ false };
		double _absolutePosition{ 0.0 };
		ToolTip _toolTip{ nullptr };
		TextBlock _toolTipText{ nullptr };

		winrt::Microsoft::UI::Xaml::Controls::Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;

		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragCompleted_revoker _minThumbDragCompletedRevoker;
		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragDelta_revoker _minThumbDragDeltaRevoker;
		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragStarted_revoker _minThumbDragStartedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::KeyDown_revoker _minThumbKeyDownRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::KeyUp_revoker _minThumbKeyUpRevoker;

		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragCompleted_revoker _maxThumbDragCompletedRevoker;
		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragDelta_revoker _maxThumbDragDeltaRevoker;
		winrt::Microsoft::UI::Xaml::Controls::Primitives::Thumb::DragStarted_revoker _maxThumbDragStartedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::KeyDown_revoker _maxThumbKeyDownRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::KeyUp_revoker _maxThumbKeyUpRevoker;

		winrt::Microsoft::UI::Xaml::FrameworkElement::SizeChanged_revoker _canvasSizeChangedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::PointerEntered_revoker _canvasPointerEnteredRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::PointerPressed_revoker _canvasPointerPressedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::PointerMoved_revoker _canvasPointerMovedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::PointerReleased_revoker _canvasPointerReleasedRevoker;
		winrt::Microsoft::UI::Xaml::UIElement::PointerExited_revoker _canvasPointerExitedRevoker;

		winrt::event<winrt::Microsoft::UI::Xaml::Controls::Primitives::DragStartedEventHandler> _thumbDragStarted;
		winrt::event<winrt::Microsoft::UI::Xaml::Controls::Primitives::DragCompletedEventHandler> _thumbDragCompleted;

	public:
		RangeSelector();

		void OnApplyTemplate();

		wil::untyped_event<winrt::XamlToolkit::WinUI::Controls::RangeChangedEventArgs> ValueChanged;

		winrt::event_token ThumbDragStarted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragStartedEventHandler const& handler);

		void ThumbDragStarted(winrt::event_token const& token) noexcept;

		winrt::event_token ThumbDragCompleted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragCompletedEventHandler const& handler);

		void ThumbDragCompleted(winrt::event_token const& token) noexcept;

		virtual void OnThumbDragStarted(DragStartedEventArgs const& e);

		virtual void OnThumbDragCompleted(DragCompletedEventArgs const& e);

		virtual void OnValueChanged(winrt::XamlToolkit::WinUI::Controls::RangeChangedEventArgs const& e);

		static void MinimumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void MaximumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void RangeMinChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void RangeMaxChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OrientationChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MinimumProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Minimum",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMinimum), &RangeSelector::MinimumChangedCallback });

		double Minimum() const
		{
			return winrt::unbox_value<double>(GetValue(MinimumProperty));
		}

		void Minimum(double const& value) const
		{
			SetValue(MinimumProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MaximumProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Maximum",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMaximum), &RangeSelector::MaximumChangedCallback });

		double Maximum() const
		{
			return winrt::unbox_value<double>(GetValue(MaximumProperty));
		}

		void Maximum(double const& value) const
		{
			SetValue(MaximumProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RangeStartProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"RangeStart",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMinimum), &RangeSelector::RangeMinChangedCallback });

		double RangeStart() const
		{
			return winrt::unbox_value<double>(GetValue(RangeStartProperty));
		}

		void RangeStart(double const& value) const
		{
			SetValue(RangeStartProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RangeEndProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"RangeEnd",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMaximum), &RangeSelector::RangeMaxChangedCallback });

		double RangeEnd() const
		{
			return winrt::unbox_value<double>(GetValue(RangeEndProperty));
		}

		void RangeEnd(double const& value) const
		{
			SetValue(RangeEndProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> StepFrequencyProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"StepFrequency",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultStepFrequency) }
			);

		double StepFrequency() const
		{
			return winrt::unbox_value<double>(GetValue(StepFrequencyProperty));
		}

		void StepFrequency(double const& value) const
		{
			SetValue(StepFrequencyProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OrientationProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Orientation",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata
				{
					winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
					&RangeSelector::OrientationChangedCallback
				});

		winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty));
		}

		void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const
		{
			SetValue(OrientationProperty, winrt::box_value(value));
		}

	private:
		void ContainerCanvas_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void VerifyValues() const;

		void RangeMinToStepFrequency();

		void RangeMaxToStepFrequency();

		double MoveToStepFrequency(double rangeValue) const;

		void SyncThumbs(bool fromMinKeyDown = false, bool fromMaxKeyDown = false);

		void SyncActiveRectangle();

		void RangeSelector_IsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);

#pragma region Pointer Events
		void ContainerCanvas_PointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void ContainerCanvas_PointerExited(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void ContainerCanvas_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void ContainerCanvas_PointerMoved(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void ContainerCanvas_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);
#pragma endregion

#pragma region Key Events
		double GetKeyDelta(winrt::Windows::System::VirtualKey key) const;

		void MinThumb_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void MaxThumb_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void Thumb_KeyUp(IInspectable const& sender, KeyRoutedEventArgs const& e);
#pragma endregion

#pragma region Drag Events
		void MinThumb_DragDelta(IInspectable const& sender, DragDeltaEventArgs const& e);

		void MaxThumb_DragDelta(IInspectable const& sender, DragDeltaEventArgs const& e);

		void MinThumb_DragStarted(IInspectable const& sender, DragStartedEventArgs const& e);

		void MaxThumb_DragStarted(IInspectable const& sender, DragStartedEventArgs const& e);

		void Thumb_DragCompleted(IInspectable const& sender, DragCompletedEventArgs const& e);

		double DragLength() const;

		double DragThumb(Thumb const& thumb, double min, double max, double nextPos);

		void Thumb_DragStarted(Thumb const& thumb);
#pragma endregion

		void Debounce();

		bool IsHorizontal() const;

		double GetPointerAxisPosition(PointerRoutedEventArgs const& e) const;

		void UpdateToolTip(Thumb const& thumb, double newValue);

		void AttachToolTip(Thumb const& thumb);

		void DetachToolTip(Thumb const& thumb);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct RangeSelector : RangeSelectorT<RangeSelector, implementation::RangeSelector>
	{
	};
}
