#include "pch.h"
#include "RangeSelector.h"
#if __has_include("RangeSelector.g.cpp")
#include "RangeSelector.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	RangeSelector::RangeSelector()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void RangeSelector::OnApplyTemplate()
	{
		if (_minThumb != nullptr)
		{
			_minThumb.DragCompleted(_minThumbDragCompletedToken);
			_minThumb.DragDelta(_minThumbDragDeltaToken);
			_minThumb.DragStarted(_minThumbDragStartedToken);
			_minThumb.KeyDown(_minThumbKeyUpToken);
		}

		if (_maxThumb != nullptr)
		{
			_maxThumb.DragCompleted(_maxThumbDragCompletedToken);
			_maxThumb.DragDelta(_maxThumbDragDeltaToken);
			_maxThumb.DragStarted(_maxThumbDragStartedToken);
			_maxThumb.KeyDown(_maxThumbKeyDownToken);
			_maxThumb.KeyUp(_maxThumbKeyUpToken);
		}

		if (_containerCanvas != nullptr)
		{
			_containerCanvas.SizeChanged(_canvasSizeChangedToken);
			_containerCanvas.PointerPressed(_canvasPointerPressedToken);
			_containerCanvas.PointerMoved(_canvasPointerMovedToken);
			_containerCanvas.PointerReleased(_canvasPointerReleasedToken);
			_containerCanvas.PointerExited(_canvasPointerExitedToken);
		}

		IsEnabledChanged(_isEnabledChanged);

		// Need to make sure the values can be set in XAML and don't overwrite each other
		VerifyValues();
		_valuesAssigned = true;

		_activeRectangle = GetTemplateChild(L"ActiveRectangle").try_as<Rectangle>();
		_minThumb = GetTemplateChild(L"MinThumb").try_as<Thumb>();
		_maxThumb = GetTemplateChild(L"MaxThumb").try_as<Thumb>();
		_containerCanvas = GetTemplateChild(L"ContainerCanvas").try_as<Canvas>();
		_toolTip = GetTemplateChild(L"ToolTip").try_as<Grid>();
		_toolTipText = GetTemplateChild(L"ToolTipText").try_as<TextBlock>();

		if (_minThumb != nullptr)
		{
			_minThumbDragCompletedToken = _minThumb.DragCompleted({ this, &RangeSelector::Thumb_DragCompleted });
			_minThumbDragDeltaToken = _minThumb.DragDelta({ this, &RangeSelector::MinThumb_DragDelta });
			_minThumbDragStartedToken = _minThumb.DragStarted({ this, &RangeSelector::MinThumb_DragStarted });
			_minThumbKeyDownToken = _minThumb.KeyDown({ this, &RangeSelector::MinThumb_KeyDown });
			_minThumbKeyUpToken = _minThumb.KeyUp({ this, &RangeSelector::Thumb_KeyUp });
		}

		if (_maxThumb != nullptr)
		{
			_maxThumbDragCompletedToken = _maxThumb.DragCompleted({ this, &RangeSelector::Thumb_DragCompleted });
			_maxThumbDragDeltaToken = _maxThumb.DragDelta({ this, &RangeSelector::MaxThumb_DragDelta });
			_maxThumbDragStartedToken = _maxThumb.DragStarted({ this, &RangeSelector::MaxThumb_DragStarted });
			_maxThumbKeyDownToken = _maxThumb.KeyDown({ this, &RangeSelector::MaxThumb_KeyDown });
			_maxThumbKeyUpToken = _maxThumb.KeyUp({ this, &RangeSelector::Thumb_KeyUp });
		}

		if (_containerCanvas != nullptr)
		{
			_canvasSizeChangedToken = _containerCanvas.SizeChanged({ this, &RangeSelector::ContainerCanvas_SizeChanged });
			_canvasPointerEnteredToken = _containerCanvas.PointerEntered({ this, &RangeSelector::ContainerCanvas_PointerEntered });
			_canvasPointerPressedToken = _containerCanvas.PointerPressed({ this, &RangeSelector::ContainerCanvas_PointerPressed });
			_canvasPointerMovedToken = _containerCanvas.PointerMoved({ this, &RangeSelector::ContainerCanvas_PointerMoved });
			_canvasPointerReleasedToken = _containerCanvas.PointerReleased({ this, &RangeSelector::ContainerCanvas_PointerReleased });
			_canvasPointerExitedToken = _containerCanvas.PointerExited({ this, &RangeSelector::ContainerCanvas_PointerExited });
		}

		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, false);

		_isEnabledChanged = IsEnabledChanged({ this, &RangeSelector::RangeSelector_IsEnabledChanged });

		// Measure our min/max text longest value so we can avoid the length of the scrolling reason shifting in size during use.
		TextBlock tb;
		tb.Text(winrt::format(L"{}", Maximum()));
		tb.Measure(Size(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));

		base_type::OnApplyTemplate();
	}

	void RangeSelector::UpdateToolTipText([[maybe_unused]] class_type const& rangeSelector, TextBlock const& toolTip, double newValue)
	{
		if (toolTip != nullptr)
		{
			toolTip.Text(winrt::format(L"{:.2f}", newValue));
		}
	}

	void RangeSelector::ContainerCanvas_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		SyncThumbs();
	}

	void RangeSelector::VerifyValues() const
	{
		if (Minimum() > Maximum())
		{
			Minimum(Maximum());
			Maximum(Maximum());
		}

		if (Minimum() == Maximum())
		{
			Maximum(Maximum() + Epsilon);
		}

		if (!_maxSet)
		{
			RangeEnd(Maximum());
		}

		if (!_minSet)
		{
			RangeStart(Minimum());
		}

		if (RangeStart() < Minimum())
		{
			RangeStart(Minimum());
		}

		if (RangeEnd() < Minimum())
		{
			RangeEnd(Minimum());
		}

		if (RangeStart() > Maximum())
		{
			RangeStart(Maximum());
		}

		if (RangeEnd() > Maximum())
		{
			RangeEnd(Maximum());
		}

		if (RangeEnd() < RangeStart())
		{
			RangeStart(RangeEnd());
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
		double newValue = Minimum() + (((int)std::round((rangeValue - Minimum()) / StepFrequency())) * StepFrequency());

		if (newValue < Minimum())
		{
			return Minimum();
		}
		else if (newValue > Maximum() || Maximum() - newValue < StepFrequency())
		{
			return Maximum();
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

		auto relativeLeft = ((RangeStart() - Minimum()) / (Maximum() - Minimum())) * DragWidth();
		auto relativeRight = ((RangeEnd() - Minimum()) / (Maximum() - Minimum())) * DragWidth();

		Canvas::SetLeft(_minThumb, relativeLeft);
		Canvas::SetLeft(_maxThumb, relativeRight);

		if (fromMinKeyDown || fromMaxKeyDown)
		{
			DragThumb(
				fromMinKeyDown ? _minThumb : _maxThumb,
				fromMinKeyDown ? 0 : Canvas::GetLeft(_minThumb),
				fromMinKeyDown ? Canvas::GetLeft(_maxThumb) : DragWidth(),
				fromMinKeyDown ? relativeLeft : relativeRight);
			if (_toolTipText != nullptr)
			{
				UpdateToolTipText(*this, _toolTipText, fromMinKeyDown ? RangeStart() : RangeEnd());
			}
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

		auto relativeLeft = Canvas::GetLeft(_minThumb);
		Canvas::SetLeft(_activeRectangle, relativeLeft);
		Canvas::SetTop(_activeRectangle, (_containerCanvas.ActualHeight() - _activeRectangle.ActualHeight()) / 2);
		_activeRectangle.Width(std::max<double>(0, Canvas::GetLeft(_maxThumb) - Canvas::GetLeft(_minThumb)));
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
}
