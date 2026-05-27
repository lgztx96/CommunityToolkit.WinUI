#pragma once

#include "FontIconExtension.g.h"
#include "TextIconExtension.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Text.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Text;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	
	struct FontIconExtension : FontIconExtensionT<FontIconExtension, implementation::TextIconExtension>
	{
		FontIconExtension() = default;

		wil::single_threaded_rw_property<winrt::hstring> Glyph;

		wil::single_threaded_rw_property<FontFamily> FontFamily{ nullptr };

		winrt::Windows::Foundation::IInspectable ProvideValue() const;

		IInspectable ProvideValue(IXamlServiceProvider const& provider);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct FontIconExtension : FontIconExtensionT<FontIconExtension, implementation::FontIconExtension>
	{
	};
}
