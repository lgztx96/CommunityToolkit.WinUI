#pragma once

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct ControlHelpers
	{
		static bool IsXamlRootAvailable()
		{
			return Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(winrt::name_of<winrt::Microsoft::UI::Xaml::UIElement>(), L"XamlRoot");
		}
	};
}
