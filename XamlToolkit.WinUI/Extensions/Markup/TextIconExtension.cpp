#include "pch.h"
#include "winrt_module_imports.h"
#include "TextIconExtension.h"
#if __has_include("TextIconExtension.g.cpp")
#include "TextIconExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::FontFamily TextIconExtension::SymbolThemeFontFamily()
	{
		if (!symbolThemeFontFamily)
		{
			symbolThemeFontFamily = winrt::FontFamily(L"Segoe Fluent Icons,Segoe MDL2 Assets");
		}

		return symbolThemeFontFamily;
	}

	winrt::IInspectable TextIconExtension::ProvideValue() { return nullptr; }

	winrt::IInspectable TextIconExtension::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
