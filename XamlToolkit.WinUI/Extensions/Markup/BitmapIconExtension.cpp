#include "pch.h"
#include "winrt_module_imports.h"
#include "BitmapIconExtension.h"
#if __has_include("BitmapIconExtension.g.cpp")
#include "BitmapIconExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::IInspectable BitmapIconExtension::ProvideValue() const
	{
		winrt::BitmapIcon icon;
		icon.ShowAsMonochrome(ShowAsMonochrome());
		icon.UriSource(Source());
		return icon;
	}

	winrt::IInspectable BitmapIconExtension::ProvideValue(
		[[maybe_unused]] winrt::IXamlServiceProvider const& provider) const
	{
		return ProvideValue();
	}
}
