#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls
{
	struct StaggeredItem
	{
		wil::single_threaded_rw_property<double> Height;
		wil::single_threaded_rw_property<double> Top;
		wil::single_threaded_rw_property<int> Index;
		wil::single_threaded_rw_property<winrt::Microsoft::UI::Xaml::UIElement> Element;

		StaggeredItem(int index) : Index(index), Height(0.0), Top(0.0), Element{ nullptr } {}
	};
}
