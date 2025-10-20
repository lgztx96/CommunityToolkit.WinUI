#pragma once

#include <winrt/Windows.Foundation.Metadata.h>

namespace winrt::CommunityToolkit::WinUI::Controls
{
	struct ControlHelpers
	{
		static bool IsXamlRootAvailable()
		{
			return Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(winrt::name_of<Windows::UI::Xaml::UIElement>(), L"XamlRoot");
		}
	};
}