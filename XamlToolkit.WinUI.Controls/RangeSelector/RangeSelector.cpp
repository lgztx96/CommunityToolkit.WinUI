#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <limits>
#endif
#include "RangeSelector.h"
#if __has_include("RangeSelector.g.cpp")
#include "RangeSelector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	RangeSelector::RangeSelector()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void RangeSelector::OnApplyTemplate()
	{
		if (_minThumb != nullptr)
		{
			_minThumbDragCompletedRevoker.revoke();
			_minThumbDragDeltaRevoker.revoke();
			_minThumbDragStartedRevoker.revoke();
			_minThumbKeyUpRevoker.revoke();
		}

		if (_maxThumb != nullptr)
		{
			_maxThumbDragCompletedRevoker.revoke();
			_maxThumbDragDeltaRevoker.revoke();
			_maxThumbDragStartedRevoker.revoke();
			_maxThumbKeyDownRevoker.revoke();
			_maxThumbKeyUpRevoker.revoke();
		}

		if (_containerCanvas != nullptr)
		{
			_canvasSizeChangedRevoker.revoke();
			_canvasPointerPressedRevoker.revoke();
			_canvasPointerMovedRevoker.revoke();
			_canvasPointerReleasedRevoker.revoke();
			_canvasPointerExitedRevoker.revoke();
		}

		// Need to make sure the values can be set in XAML and don't overwrite each other
		VerifyValues();
		_valuesAssigned = true;

		_activeRectangle = GetTemplateChild(ActiveRectanglePartName).try_as<Rectangle>();
		_minThumb = GetTemplateChild(MinThumbPartName).try_as<Thumb>();
		_maxThumb = GetTemplateChild(MaxThumbPartName).try_as<Thumb>();
		_containerCanvas = GetTemplateChild(ContainerCanvasPartName).try_as<Canvas>();

		if (_minThumb != nullptr)
		{
			_minThumbDragCompletedRevoker = _minThumb.DragCompleted(winrt::auto_revoke, { this, &RangeSelector::Thumb_DragCompleted });
			_minThumbDragDeltaRevoker = _minThumb.DragDelta(winrt::auto_revoke, { this, &RangeSelector::MinThumb_DragDelta });
			_minThumbDragStartedRevoker = _minThumb.DragStarted(winrt::auto_revoke, { this, &RangeSelector::MinThumb_DragStarted });
			_minThumbKeyDownRevoker = _minThumb.KeyDown(winrt::auto_revoke, { this, &RangeSelector::MinThumb_KeyDown });
			_minThumbKeyUpRevoker = _minThumb.KeyUp(winrt::auto_revoke, { this, &RangeSelector::Thumb_KeyUp });
		}

		if (_maxThumb != nullptr)
		{
			_maxThumbDragCompletedRevoker = _maxThumb.DragCompleted(winrt::auto_revoke, { this, &RangeSelector::Thumb_DragCompleted });
			_maxThumbDragDeltaRevoker = _maxThumb.DragDelta(winrt::auto_revoke, { this, &RangeSelector::MaxThumb_DragDelta });
			_maxThumbDragStartedRevoker = _maxThumb.DragStarted(winrt::auto_revoke, { this, &RangeSelector::MaxThumb_DragStarted });
			_maxThumbKeyDownRevoker = _maxThumb.KeyDown(winrt::auto_revoke, { this, &RangeSelector::MaxThumb_KeyDown });
			_maxThumbKeyUpRevoker = _maxThumb.KeyUp(winrt::auto_revoke, { this, &RangeSelector::Thumb_KeyUp });
		}

		if (_containerCanvas != nullptr)
		{
			_canvasSizeChangedRevoker = _containerCanvas.SizeChanged(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_SizeChanged });
			_canvasPointerEnteredRevoker = _containerCanvas.PointerEntered(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_PointerEntered });
			_canvasPointerPressedRevoker = _containerCanvas.PointerPressed(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_PointerPressed });
			_canvasPointerMovedRevoker = _containerCanvas.PointerMoved(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_PointerMoved });
			_canvasPointerReleasedRevoker = _containerCanvas.PointerReleased(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_PointerReleased });
			//_canvasPointerExitedRevoker = _containerCanvas.PointerExited(winrt::auto_revoke, { this, &RangeSelector::ContainerCanvas_PointerExited });
		}

		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, false);

		VisualStateManager::GoToState(*this, IsHorizontal() ? HorizontalState : VerticalState, true);

		_isEnabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &RangeSelector::RangeSelector_IsEnabledChanged });

		if (_toolTip == nullptr)
		{
			_toolTip = ToolTip();
			_toolTipText = TextBlock();
			_toolTip.Content(_toolTipText);
		}
		
		base_type::OnApplyTemplate();
	}

	void RangeSelector::ContainerCanvas_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		SyncThumbs();
	}

	void RangeSelector::VerifyValues() const
	{
		auto minimum = Minimum();
		auto maximum = Maximum();
		auto rangeStart = RangeStart();
		auto rangeEnd = RangeEnd();

		if (minimum > maximum)
		{
			Minimum(maximum);
			Maximum(maximum);
		}

		if (minimum == maximum)
		{
			// Do nothing
		}

		if (!_maxSet)
		{
			RangeEnd(maximum);
		}

		if (!_minSet)
		{
			RangeStart(minimum);
		}

		if (rangeStart < minimum)
		{
			RangeStart(minimum);
		}

		if (rangeEnd < minimum)
		{
			RangeEnd(minimum);
		}

		if (rangeStart > maximum)
		{
			RangeStart(maximum);
		}

		if (rangeEnd > maximum)
		{
			RangeEnd(maximum);
		}

		if (rangeEnd < rangeStart)
		{
			RangeStart(rangeEnd);
		}
	}

	void RangeSelector::RangeMinToStepFrequency()
	{
		RangeStart(MoveToStepFrequency(RangeStart()));
	}

	void RangeSelector::RangeMaxToStepFrequency()
	{
		RangeEnd(MoveToStepFrequency(RangeEnd()));
	}

	double RangeSelector::MoveToStepFrequency(double rangeValue) const
	{
		auto minimum = Minimum();
		auto maximum = Maximum();
		auto stepFrequency = StepFrequency();

		double newValue = minimum + ((static_cast<int>(std::round((rangeValue - minimum) / stepFrequency))) * stepFrequency);

		if (newValue < minimum)
		{
			return minimum;
		}
		else if (newValue > maximum || maximum - newValue < stepFrequency)
		{
			return maximum;
		}
		else
		{
			return newValue;
		}
	}

	void RangeSelector::SyncThumbs(bool fromMinKeyDown, bool fromMaxKeyDown)
	{
		if (_containerCanvas == nullptr)
		{
			return;
		}

		auto minimum = Minimum();
		auto maximum = Maximum();
		auto dragLength = DragLength();
		auto rangeStart = RangeStart();
		auto rangeEnd = RangeEnd();

		double relativeStart, relativeEnd;

		if (IsHorizontal())
		{
			relativeStart = ((rangeStart - minimum) / (maximum - minimum)) * dragLength;
			relativeEnd = ((rangeEnd - minimum) / (maximum - minimum)) * dragLength;
		}
		else
		{
			relativeStart = (1.0 - (rangeStart - minimum) / (maximum - minimum)) * dragLength;
			relativeEnd = (1.0 - (rangeEnd - minimum) / (maximum - minimum)) * dragLength;
		}

		if (IsHorizontal())
		{
			Canvas::SetLeft(_minThumb, relativeStart);
			Canvas::SetLeft(_maxThumb, relativeEnd);
		}
		else
		{
			Canvas::SetTop(_minThumb, relativeStart);
			Canvas::SetTop(_maxThumb, relativeEnd);
		}

		if (fromMinKeyDown || fromMaxKeyDown)
		{
			const auto& activeThumb = fromMinKeyDown ? _minThumb : _maxThumb;
			double initialPos = fromMinKeyDown ? relativeStart : relativeEnd;

			double otherPos = IsHorizontal()
				? (fromMinKeyDown ? Canvas::GetLeft(_maxThumb) : Canvas::GetLeft(_minThumb))
				: (fromMinKeyDown ? Canvas::GetTop(_maxThumb) : Canvas::GetTop(_minThumb));

			double dragMin = fromMinKeyDown ? 0.0 : otherPos;
			double dragMax = fromMinKeyDown ? otherPos : dragLength;

			DragThumb(activeThumb, dragMin, dragMax, initialPos);
		}

		SyncActiveRectangle();
	}

	void RangeSelector::SyncActiveRectangle()
	{
		if (_containerCanvas == nullptr)
		{
			return;
		}

		if (_minThumb == nullptr)
		{
			return;
		}

		if (_maxThumb == nullptr)
		{
			return;
		}

		if (IsHorizontal())
		{
			auto minLeft = Canvas::GetLeft(_minThumb);
			auto maxLeft = Canvas::GetLeft(_maxThumb);

			Canvas::SetLeft(_activeRectangle, minLeft);
			Canvas::SetTop(_activeRectangle, (_containerCanvas.ActualHeight() - _activeRectangle.ActualHeight()) / 2);

			_activeRectangle.Width(std::max<double>(0.0, maxLeft - minLeft));
		}
		else {
			auto minTop = Canvas::GetTop(_minThumb);
			auto maxTop = Canvas::GetTop(_maxThumb);
			Canvas::SetTop(_activeRectangle, maxTop);
			Canvas::SetLeft(_activeRectangle, (_containerCanvas.ActualWidth() - _activeRectangle.ActualWidth()) / 2);
			_activeRectangle.Height(std::max<double>(0.0, minTop - maxTop));
		}
	}

	void RangeSelector::RangeSelector_IsEnabledChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);
	}

	void RangeSelector::MinimumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();
		if (control == nullptr)
		{
			return;
		}

		auto rangeSelector = winrt::get_self<RangeSelector>(control);

		if (!rangeSelector->_valuesAssigned)
		{
			return;
		}

		auto newValue = winrt::unbox_value<double>(e.NewValue());
		auto oldValue = winrt::unbox_value<double>(e.OldValue());

		if (rangeSelector->Maximum() < newValue)
		{
			rangeSelector->Maximum(newValue + Epsilon);
		}

		if (rangeSelector->RangeStart() < newValue)
		{
			rangeSelector->RangeStart(newValue);
		}

		if (rangeSelector->RangeEnd() < newValue)
		{
			rangeSelector->RangeEnd(newValue);
		}

		if (newValue != oldValue)
		{
			rangeSelector->SyncThumbs();
		}
	}

	void RangeSelector::MaximumChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();
		if (control == nullptr)
		{
			return;
		}

		auto rangeSelector = winrt::get_self<RangeSelector>(control);

		if (!rangeSelector->_valuesAssigned)
		{
			return;
		}

		auto newValue = winrt::unbox_value<double>(e.NewValue());
		auto oldValue = winrt::unbox_value<double>(e.OldValue());

		if (rangeSelector->Minimum() > newValue)
		{
			rangeSelector->Minimum(newValue - Epsilon);
		}

		if (rangeSelector->RangeEnd() > newValue)
		{
			rangeSelector->RangeEnd(newValue);
		}

		if (rangeSelector->RangeStart() > newValue)
		{
			rangeSelector->RangeStart(newValue);
		}

		if (newValue != oldValue)
		{
			rangeSelector->SyncThumbs();
		}
	}

	void RangeSelector::RangeMinChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();
		if (control == nullptr)
		{
			return;
		}

		auto rangeSelector = winrt::get_self<RangeSelector>(control);

		rangeSelector->_minSet = true;

		if (!rangeSelector->_valuesAssigned)
		{
			return;
		}

		auto newValue = winrt::unbox_value<double>(e.NewValue());
		rangeSelector->RangeMinToStepFrequency();

		if (rangeSelector->_valuesAssigned)
		{
			if (newValue < rangeSelector->Minimum())
			{
				rangeSelector->RangeStart(rangeSelector->Minimum());
			}
			else if (newValue > rangeSelector->Maximum())
			{
				rangeSelector->RangeStart(rangeSelector->Maximum());
			}

			rangeSelector->SyncActiveRectangle();

			// If the new value is greater than the old max, move the max also
			if (newValue > rangeSelector->RangeEnd())
			{
				rangeSelector->RangeEnd(newValue);
			}
		}

		rangeSelector->SyncThumbs();
	}

	void RangeSelector::RangeMaxChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = d.try_as<class_type>();
		if (control == nullptr)
		{
			return;
		}

		auto rangeSelector = winrt::get_self<RangeSelector>(control);

		rangeSelector->_maxSet = true;

		if (!rangeSelector->_valuesAssigned)
		{
			return;
		}

		auto newValue = winrt::unbox_value<double>(e.NewValue());
		rangeSelector->RangeMaxToStepFrequency();

		if (rangeSelector->_valuesAssigned)
		{
			if (newValue < rangeSelector->Minimum())
			{
				rangeSelector->RangeEnd(rangeSelector->Minimum());
			}
			else if (newValue > rangeSelector->Maximum())
			{
				rangeSelector->RangeEnd(rangeSelector->Maximum());
			}

			rangeSelector->SyncActiveRectangle();

			// If the new max is less than the old minimum then move the minimum
			if (newValue < rangeSelector->RangeStart())
			{
				rangeSelector->RangeStart(newValue);
			}
		}

		rangeSelector->SyncThumbs();
	}

	void RangeSelector::OrientationChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto newValue = winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(e.NewValue());

			bool isHorizontal = newValue == winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal;

			winrt::Microsoft::UI::Xaml::VisualStateManager::GoToState(control, isHorizontal ? HorizontalState : VerticalState, false);

			auto rangeSelector = winrt::get_self<RangeSelector>(control);

			rangeSelector->SyncThumbs();
		}
	}

	bool RangeSelector::IsHorizontal() const 
	{ 
		return Orientation() == winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal;
	}

	double RangeSelector::GetPointerAxisPosition(PointerRoutedEventArgs const& e) const
	{
		auto point = e.GetCurrentPoint(_containerCanvas).Position();
		return IsHorizontal() ? point.X : point.Y;
	}

	void RangeSelector::AttachToolTip(Thumb const& thumb)
	{
		if (_toolTip != nullptr) 
		{
			ToolTipService::SetToolTip(thumb, _toolTip);
			auto placement = IsHorizontal() ? PlacementMode::Top : PlacementMode::Left;
			ToolTipService::SetPlacement(thumb, placement);
			
			_toolTip.IsOpen(true);
		}
	}

	void RangeSelector::DetachToolTip(Thumb const& thumb) 
	{
		if (_toolTip != nullptr)
		{
			_toolTip.IsOpen(false);
			ToolTipService::SetToolTip(thumb, nullptr);
		}
	}

	void RangeSelector::UpdateToolTip(Thumb const& thumb, double newValue)
	{
		_toolTipText.Text(winrt::format(L"{:.2f}", newValue));

		_toolTip.Measure(winrt::Windows::Foundation::Size{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() });
		auto desiredSize = _toolTip.DesiredSize();

		auto isHorizontal = IsHorizontal();
		double position = isHorizontal 
			? Canvas::GetLeft(thumb) + (thumb.ActualWidth() / 2.0)
			: Canvas::GetTop(thumb) + (thumb.ActualHeight() / 2.0);

		if (isHorizontal)
		{
			_toolTip.HorizontalOffset(position - desiredSize.Width / 2.0);
			_toolTip.VerticalOffset(DefaultMouseOffset);
		}
		else
		{
			_toolTip.HorizontalOffset(DefaultMouseOffset);
			_toolTip.VerticalOffset(position - desiredSize.Height / 2.0);
		}
	}
}
