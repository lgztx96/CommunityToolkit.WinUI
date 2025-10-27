#include "pch.h"
#include "ControlSizeTrigger.h"
#if __has_include("ControlSizeTrigger.g.cpp")
#include "ControlSizeTrigger.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::implementation
{
	// Handle event to get current values
	void ControlSizeTrigger::OnTargetElementSizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		UpdateTrigger();
	}

	void ControlSizeTrigger::UpdateTargetElement(FrameworkElement const& oldValue, FrameworkElement const& newValue)
	{
		if (oldValue != nullptr)
		{
			_sizeChangedRevoker.revoke();
		}

		if (newValue != nullptr)
		{
			_sizeChangedRevoker = newValue.SizeChanged(winrt::auto_revoke, { this, &ControlSizeTrigger::OnTargetElementSizeChanged });
		}

		UpdateTrigger();
	}

	// Logic to evaluate and apply trigger value
	void ControlSizeTrigger::UpdateTrigger()
	{
		if (TargetElement() == nullptr || !CanTrigger())
		{
			SetActive(false);
			return;
		}

		bool activate = MinWidth() <= TargetElement().ActualWidth() &&
			TargetElement().ActualWidth() < MaxWidth() &&
			MinHeight() <= TargetElement().ActualHeight() &&
			TargetElement().ActualHeight() < MaxHeight();

		IsActive = activate;
		SetActive(activate);
	}
}
