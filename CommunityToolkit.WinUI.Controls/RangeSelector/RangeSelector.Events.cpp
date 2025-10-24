#include "pch.h"
#include "RangeSelector.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	winrt::event_token RangeSelector::ThumbDragStarted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragStartedEventHandler const& handler)
	{
		return _thumbDragStarted.add(handler);
	}

	void RangeSelector::ThumbDragStarted(winrt::event_token const& token) noexcept
	{
		_thumbDragStarted.remove(token);
	}

	winrt::event_token RangeSelector::ThumbDragCompleted(winrt::Microsoft::UI::Xaml::Controls::Primitives::DragCompletedEventHandler const& handler)
	{
		return _thumbDragCompleted.add(handler);
	}

	void RangeSelector::ThumbDragCompleted(winrt::event_token const& token) noexcept
	{
		_thumbDragCompleted.remove(token);
	}

	void RangeSelector::OnThumbDragStarted(DragStartedEventArgs const& e)
	{
		if (_thumbDragStarted) _thumbDragStarted(*this, e);
	}

	void RangeSelector::OnThumbDragCompleted(DragCompletedEventArgs const& e)
	{
		if (_thumbDragCompleted) _thumbDragCompleted(*this, e);
	}

	void RangeSelector::OnValueChanged(winrt::CommunityToolkit::WinUI::Controls::RangeChangedEventArgs const& e)
	{
		ValueChanged.invoke(*this, e);
	}
}