#pragma once

#include "FontIconExtension.g.h"
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Text.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "TextIconExtension.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Text;
	using namespace winrt::Microsoft::UI::Xaml::Media;

	struct FontIconExtension : FontIconExtensionT<FontIconExtension, implementation::TextIconExtension>
	{
		FontIconExtension() = default;

		wil::single_threaded_rw_property<winrt::hstring> Glyph;

		wil::single_threaded_rw_property<FontFamily> FontFamily{ nullptr };

		winrt::Windows::Foundation::IInspectable ProvideValue() const;

		IInspectable ProvideValue(IXamlServiceProvider const& provider);
	};
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct FontIconExtension : FontIconExtensionT<FontIconExtension, implementation::FontIconExtension>
	{
	};
}
