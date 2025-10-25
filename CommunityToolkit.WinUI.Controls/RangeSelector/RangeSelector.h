#pragma once

#include "RangeChangedEventArgs.h"
#include "RangeSelector.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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
		double _oldValue;
		bool _valuesAssigned;
		bool _minSet;
		bool _maxSet;
		bool _pointerManipulatingMin;
		bool _pointerManipulatingMax;
		double _absolutePosition;
		Grid _toolTip{ nullptr };
		TextBlock _toolTipText{ nullptr };

		winrt::event_token _isEnabledChanged;

		winrt::event_token _minThumbDragCompletedToken;
		winrt::event_token _minThumbDragDeltaToken;
		winrt::event_token _minThumbDragStartedToken;
		winrt::event_token _minThumbKeyDownToken;
		winrt::event_token _minThumbKeyUpToken;

		winrt::event_token _maxThumbDragCompletedToken;
		winrt::event_token _maxThumbDragDeltaToken;
		winrt::event_token _maxThumbDragStartedToken;
		winrt::event_token _maxThumbKeyDownToken;
		winrt::event_token _maxThumbKeyUpToken;

		winrt::event_token _canvasSizeChangedToken;
		winrt::event_token _canvasPointerEnteredToken;
		winrt::event_token _canvasPointerPressedToken;
		winrt::event_token _canvasPointerMovedToken;
		winrt::event_token _canvasPointerReleasedToken;
		winrt::event_token _canvasPointerExitedToken;

		winrt::event_token _keyDebounceTimerTickToken;

		winrt::event<winrt::Microsoft::UI::Xaml::Controls::Primitives::DragStartedEventHandler> _thumbDragStarted;
		winrt::event<winrt::Microsoft::UI::Xaml::Controls::Primitives::DragCompletedEventHandler> _thumbDragCompleted;

	public:
		RangeSelector();

		void OnApplyTemplate();

		wil::untyped_event<winrt::CommunityToolkit::WinUI::Controls::RangeChangedEventArgs> ValueChanged;

		winrt::event_token ThumbDragStarted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragStartedEventHandler const& handler);

		void ThumbDragStarted(winrt::event_token const& token) noexcept;

		winrt::event_token ThumbDragCompleted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragCompletedEventHandler const& handler);
		
		void ThumbDragCompleted(winrt::event_token const& token) noexcept;

		virtual void OnThumbDragStarted(DragStartedEventArgs const& e);

		virtual void OnThumbDragCompleted(DragCompletedEventArgs const& e);

		virtual void OnValueChanged(winrt::CommunityToolkit::WinUI::Controls::RangeChangedEventArgs const& e);

		static void MinimumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void MaximumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void RangeMinChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void RangeMaxChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MinimumProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Minimum",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMinimum), winrt::Microsoft::UI::Xaml::PropertyChangedCallback{ &RangeSelector::MinimumChangedCallback } }
			);

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
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMaximum), winrt::Microsoft::UI::Xaml::PropertyChangedCallback{ &RangeSelector::MaximumChangedCallback } }
			);

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
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMinimum), winrt::Microsoft::UI::Xaml::PropertyChangedCallback{ &RangeSelector::RangeMinChangedCallback } }
			);

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
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(DefaultMaximum), winrt::Microsoft::UI::Xaml::PropertyChangedCallback{ &RangeSelector::RangeMaxChangedCallback } }
			);

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

	private:
		static void UpdateToolTipText(class_type const& rangeSelector, TextBlock const& toolTip, double newValue);

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

		double DragWidth();

		double DragThumb(Thumb const& thumb, double min, double max, double nextPos);

		void Thumb_DragStarted(Thumb const& thumb);
#pragma endregion

		void Debounce();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct RangeSelector : RangeSelectorT<RangeSelector, implementation::RangeSelector>
	{
	};
}
