#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation::Metadata;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	struct ControlHelpers
	{
		static bool IsXamlRootAvailable()
		{
			return ApiInformation::IsPropertyPresent(winrt::name_of<UIElement>(), L"XamlRoot");
		}
	};
}
