#pragma once

#include "BitmapIconExtension.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct BitmapIconExtension : BitmapIconExtensionT<BitmapIconExtension>
	{
		BitmapIconExtension() = default;

		wil::single_threaded_rw_property<winrt::Uri> Source{ nullptr };

		wil::single_threaded_rw_property<bool> ShowAsMonochrome{ false };

		winrt::IInspectable ProvideValue() const;

		winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider) const;
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct BitmapIconExtension : BitmapIconExtensionT<BitmapIconExtension, implementation::BitmapIconExtension>
	{};
}
