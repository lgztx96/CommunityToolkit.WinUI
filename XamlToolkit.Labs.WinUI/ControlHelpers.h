#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <chrono>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation::Metadata;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct ControlHelpers
	{
		static bool IsXamlRootAvailable()
		{
			return winrt::ApiInformation::IsPropertyPresent(winrt::name_of<winrt::UIElement>(), L"XamlRoot");
		}
	};
}
