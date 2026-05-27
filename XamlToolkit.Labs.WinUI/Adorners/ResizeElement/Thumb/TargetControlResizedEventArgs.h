#pragma once

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TargetControlResizedEventArgs : winrt::implements<TargetControlResizedEventArgs, ITargetControlResizedEventArgs> 
	{
		wil::single_threaded_property<double> NewLeft;
		wil::single_threaded_property<double> NewTop;
		wil::single_threaded_property<double> NewWidth;
		wil::single_threaded_property<double> NewHeight;

		TargetControlResizedEventArgs(double newLeft, double newTop, double newWidth, double newHeight)
			: NewLeft(newLeft), NewTop(newTop), NewWidth(newWidth), NewHeight(newHeight) {}
	};
}
