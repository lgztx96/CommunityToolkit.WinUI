#pragma once

#include "StyleExtensionResourceDictionary.g.h"
#include <winrt/Microsoft.UI.Xaml.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct StyleExtensionResourceDictionary : StyleExtensionResourceDictionaryT<StyleExtensionResourceDictionary>
	{
		StyleExtensionResourceDictionary() = default;
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct StyleExtensionResourceDictionary : StyleExtensionResourceDictionaryT<StyleExtensionResourceDictionary, implementation::StyleExtensionResourceDictionary>
	{
	};
}
